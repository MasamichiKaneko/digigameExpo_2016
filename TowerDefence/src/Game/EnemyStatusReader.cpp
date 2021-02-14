#include "Game\EnemyStatusReader.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "Charactor\Enemy\Ocj.h"
#include "XMLNodeNameString.h"
#include "utils.h"
#include "Configuration.h"
#include "DxLib.h"
#include "Game\StageMap.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/exception/all.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
using namespace boost::property_tree::xml_parser;
using namespace boost::property_tree;
using namespace EnemyDataXml;

namespace Game{

	EnemyStatusReader::EnemyStatusReader( const char* EnemyStatusDataFileName, const char* EnemyAppearanceDataFileName, StageMap* aMap ){
		if ( !readEnemyStatusData( EnemyStatusDataFileName ) ) {
			goto FUNC_END;
		}

		if ( !readEnemyAppearanceData( EnemyAppearanceDataFileName, aMap ) ) {
			goto FUNC_END;
		}
FUNC_END:
		;
	}

	EnemyStatusReader::EnemyStatusReader( const int EnemyStatusDataFileHandle, const int EnemyAppearanceDataFileHandle, StageMap* aMap ){
		if ( !readEnemyStatusData( EnemyStatusDataFileHandle ) ) {
			goto FUNC_END;
		}

		if ( !readEnemyAppearanceData( EnemyAppearanceDataFileHandle, aMap ) ) {
			goto FUNC_END;
		}
FUNC_END:
		;
	}

	bool EnemyStatusReader::readEnemyStatusData( const char* EnemyStatusDataFileName ){
		int fileHandle = FileRead_open( EnemyStatusDataFileName );
		return readEnemyStatusData( fileHandle );
	}

	bool EnemyStatusReader::readEnemyStatusData( const int EnemyStatusDataFileHandle ) {
		bool result = false;
		ptree pt_status;
		boost::optional<int> idBuf;
		boost::optional<int> hpBuf;
		boost::optional<int> scoreBuf;
		boost::optional<int> assetBuf;
		boost::optional<int> speedBuf;
		boost::optional<int> assetDamageBuf;
		boost::optional<int> rangeBuf;
		boost::optional<double> stunRegistBuf;
		boost::optional<double> slowRegistBuf;

		/** ステータスデータXml読み込み */
		if ( !readXml( EnemyStatusDataFileHandle, pt_status ) ) {
			goto FUNC_END;
		}

		BOOST_FOREACH( ptree::value_type& child, pt_status.get_child( RootNode ) ){
			ptree& charaInfo = child.second;
			Status statusBuf;
			/** id読み込み */
			idBuf = charaInfo.get_optional<int>( EnemyIdAttr );
			/** その他ステータス読み込み */
			hpBuf          = charaInfo.get_optional<int   >( EnemyHitPointAttr    );
			speedBuf       = charaInfo.get_optional<int   >( EnemySpeedAttr       );
			scoreBuf       = charaInfo.get_optional<int   >( EnemyScoreAttr       );
			assetBuf       = charaInfo.get_optional<int   >( EnemyAssetAttr       );
			assetDamageBuf = charaInfo.get_optional<int   >( EnemyAssetDamageAttr );
			rangeBuf       = charaInfo.get_optional<int   >( EnemyRangeAttr       );
			stunRegistBuf  = charaInfo.get_optional<double>( EnemyStunRegistAttr  );
			slowRegistBuf  = charaInfo.get_optional<double>( EnemySlowRegistAttr  );
			/** 読み込みチェック */
			if ( !( idBuf && hpBuf && speedBuf && scoreBuf && assetBuf && assetDamageBuf && rangeBuf && stunRegistBuf && slowRegistBuf ) ) {
				goto FUNC_END;
			}
			/** 読み込み結果格納 */
			statusBuf.hitPoint    = *hpBuf;
			statusBuf.speed       = *speedBuf;
			statusBuf.score       = *scoreBuf;
			statusBuf.asset       = *assetBuf;
			statusBuf.assetDamage = *assetDamageBuf;
			statusBuf.range       = *rangeBuf;
			statusBuf.stunRegist  = *stunRegistBuf;
			statusBuf.slowRegist  = *slowRegistBuf;

			mReadStatusData[(*idBuf)] = statusBuf;
		}
		result = true;

FUNC_END:
		if ( !result ) {
			mReadStatusData.clear();
		}
		return result;
	}

	bool EnemyStatusReader::readEnemyAppearanceData( const char* EnemyAppearanceDataFileName, StageMap* aMap ) {
		int readHandle = FileRead_open( EnemyAppearanceDataFileName );
		return readEnemyAppearanceData( readHandle, aMap );
	}

	bool EnemyStatusReader::readEnemyAppearanceData( const int EnemyAppearanceDataFileHandle, StageMap* aMap ) {
		bool result = false;
		boost::char_separator<char> sep(",");

		vector<string> readStr;
		if ( !FileRead_getAll( &readStr, EnemyAppearanceDataFileHandle ) ) {
			goto FUNC_END;
		}

		for ( vector<string>::iterator it = readStr.begin(); it != readStr.end(); it++ ) {
			try{
				int idBuf;
				int appearanceBuf;
				vector<int> rootBuf;
				tokenizer< char_separator<char> > tokens( *it, sep );
				typedef tokenizer< char_separator<char> >::iterator tokenIt;
				tokenIt tIt = tokens.begin();
				/** id部読み込み */
				idBuf = lexical_cast< int >( *tIt );
				tIt++;
				/** 出現開始時間読み込み */
				appearanceBuf = Configuration::getInstance()->getFrameRate();
				appearanceBuf = static_cast< int >( appearanceBuf * lexical_cast< double >( *tIt ) );
				tIt++;
				/** ルート読み込み */
				for ( ;tIt != tokens.end(); tIt++ ) {
					rootBuf.push_back( lexical_cast< int >( *tIt ) );
				}
				/** 敵キャラデータ格納 */
				/** ステータス取得 */
				Status status = mReadStatusData.at( idBuf );
				Charactor::Enemy::EnemyBase* enemy;
				/** 敵キャラ追加 */
				enemy = new Charactor::Enemy::Ocj( 
					idBuf,
					0,
					status.hitPoint,
					status.speed,
					status.score,
					status.asset,
					status.assetDamage,
					status.range,
					appearanceBuf,
					rootBuf,
					aMap,
					status.stunRegist,
					status.slowRegist );

				mEnemys.push_back( enemy );
			} catch( boost::bad_lexical_cast& ) {
				goto FUNC_END;
			}
		}
		result = true;
FUNC_END:
		if ( !result ) {
			mEnemys.clear();
			mEnemys.shrink_to_fit();
		}

		return result;
	}
}