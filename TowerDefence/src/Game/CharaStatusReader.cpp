#include "Game\CharaStatusReader.h"
#include "utils.h"
#include <sstream>
#include "XMLNodeNameString.h"
#include "DxLib.h"
#include "FileNames.h"
#include "define.h"
#include <sstream>
#include <algorithm>
#include "CommonString.h"
#include <sys\stat.h>
#include <numeric>

using namespace std;
using namespace boost::property_tree::xml_parser;
using namespace boost::property_tree;
using namespace CharaDataXml;

namespace Game{

	int map_acc(int lhs, const std::pair<string, int> & rhs)
	{
		return lhs + rhs.second;
	}

	CharaStatusReader::CharaStatusReader() : maxLevel( 0 ){
	}

	CharaStatusReader::CharaStatusReader( const char* CharaDataFileName ) : maxLevel( 0 ){
		readCharaData( CharaDataFileName );
	}

	CharaStatusReader::~CharaStatusReader(){
	}

	int CharaStatusReader::culcAssignedSpValue( int id ){
		int value = -1;
		vector<Status>::iterator it = find( mStatusValue.begin(), mStatusValue.end(), id );
		map< string, int > assignValue;

		if ( it == mStatusValue.end()){
			goto FUNC_END;
		}
		assignValue = it->assignSPValue;

		value = std::accumulate( assignValue.begin(), assignValue.end(), 0, map_acc );
FUNC_END:
		return value;
	}

	/** キャラデータxml読込み */
	bool CharaStatusReader::readCharaData( const char* CharaDataFileName){
		
		bool result = false;
		int fileHandle;

		/** ファイルが存在しなければ終了 */
		struct stat ifile_stat;
		if ( stat( CharaDataFileName, &ifile_stat ) != 0 ) {
			goto FUNC_END;
		}

		/** ファイル読み込み */
		fileHandle = FileRead_open( CharaDataFileName );
		result = readCharaData( fileHandle );
		FileRead_close( fileHandle );
FUNC_END:

		return result;
	}

	bool CharaStatusReader::readCharaData( int CharaDataFileHandle ) {

		mStatusValue.clear();
		bool result = true;
		ptree pt_status;
		/** 読み込みバッファ */
		boost::optional<string> nameBuf;
		boost::optional<int> idBuf;
		boost::optional<int> lvBuf;
		boost::optional<int> lvMaxBuf;

		/** キャラデータxml読み込み */
		if ( !readXml( CharaDataFileHandle, pt_status ) ){
			result = false;
			goto FUNC_END;
		}

		/** キャラデータ読込み */
		BOOST_FOREACH( ptree::value_type& child , pt_status.get_child(RootNode) ) {
			ptree& charaInfo = child.second;
			Status statusBuf;

			/** 名前取得 */
			nameBuf = charaInfo.get_optional<string>( CharaNameAttr );
			/** id取得 */
			idBuf = charaInfo.get_optional<int>( CharaIdAttr );
			/** レベル読み込み */
			lvBuf = charaInfo.get_optional<int>( CharaLvValueAttr );
			/** Maxレベル読み込み */
			lvMaxBuf = charaInfo.get_optional<int>( CharaLvMaxValueAttr );
			/** 1つでも読み込めなければ失敗 */
			if ( !( nameBuf && idBuf && lvMaxBuf && lvBuf ) ) {
				result = false;
				goto FUNC_END;
			}
			statusBuf.name    = *nameBuf;
			statusBuf.id      = *idBuf;
			statusBuf.lv      = *lvBuf;
			statusBuf.lvMax   = *lvMaxBuf;
			statusBuf.totalSp = statusBuf.lv * SPValueEachOneLv;

			/** 初期値読み込み */
			if ( !readAttr( InitNode, statusBuf.initValue, charaInfo, CharaDataXml::StatusStrVector )) {
				result = false;
				goto FUNC_END;
			}
			/** 上昇値読み込み */
			if ( !readAttr( SpRisingValueNode, statusBuf.riseValueEachOneSP , charaInfo, CharaDataXml::StatusStrVector ) ) {
				result = false;
				goto FUNC_END;
			}
			/** 育成データ読込み */
			if ( !readAttr( SPNode, statusBuf.assignSPValue, charaInfo, CharaDataXml::StatusStrVector ) ) {
				result = false;
				goto FUNC_END;
			}
			/** 各ステSP最大値読み込み */
			if ( !readAttr( SPMaxNode, statusBuf.SPMaxValue, charaInfo ,CharaDataXml::StatusStrVector ) ) {
				result = false;
				goto FUNC_END;
			}

			/** 各ステ合計値計算 */
			map< string, int > totalBuf;
			for ( unsigned int i = 0; i < CharaDataXml::StatusStrVector.size(); i++ ) {
				string valueType = CharaDataXml::StatusStrVector.at(i);
				/** 初期値取得 */
				int value = statusBuf.initValue.at(valueType);
				/** 加算分を追加 */
				value += statusBuf.assignSPValue.at(valueType) * statusBuf.riseValueEachOneSP.at(valueType);
				totalBuf[valueType] = value;
			}
			statusBuf.TotalValue = totalBuf;

			mStatusValue.push_back( statusBuf );
		}
FUNC_END:
		/** 失敗なら読み込んだ分を消す */
		if ( !result ){
			mStatusValue.clear();
		}

		return result;
	}

	/** キャラデータxml書き込み */
	void CharaStatusReader::writeCharaData( const char* fileName ) {
		ptree pt;
		for ( unsigned int i = 0; i < mStatusValue.size(); i++ ) {
			ptree& child = pt.add( RootNode + "." + CharaNode , "" );
			child.put( CharaNameAttr, mStatusValue.at(i).name );
			child.put( CharaIdAttr  , mStatusValue.at(i).id );
			child.put( CharaLvMaxValueAttr , mStatusValue.at(i).lvMax );
			child.put( CharaLvValueAttr  , mStatusValue.at(i).lv );

			addWriteStatus( InitNode         , mStatusValue.at(i).initValue         , child );
			addWriteStatus( SpRisingValueNode, mStatusValue.at(i).riseValueEachOneSP, child );
			addWriteStatus( SPNode           , mStatusValue.at(i).assignSPValue     , child );
			addWriteStatus( SPMaxNode        , mStatusValue.at(i).SPMaxValue        , child );
		}
		write_xml(fileName, pt, std::locale());
	}

	/** 書き込みステータス追加関数 */
	void CharaStatusReader::addWriteStatus( string targetNode, map<string, int>& i_status, ptree& pt ){
		for ( unsigned int i = 0; i < CharaDataXml::StatusStrVector.size(); i++ ) {
			/** 属性名設定 */
			string writeAttr = targetNode+ "." + XmlCommon::attrStr + CharaDataXml::StatusStrVector.at(i);
			pt.put( writeAttr, i_status[CharaDataXml::StatusStrVector.at(i)] );
		}
	}


	void CharaStatusReader::setLevel( int level ) {
		if ( level < 0 || mStatusValue.at(0).lvMax < level ) {
			return;
		}
		for ( unsigned int i = 0; i < mStatusValue.size(); i++ ) {
			mStatusValue.at( i ).lv = level;
			for ( unsigned int j = 0; j < VariableStatusStrVector.size(); j++ ) {
				string typeName = VariableStatusStrVector.at( j );
				mStatusValue.at(i).assignSPValue[typeName] = level;
			}
		}
	}

	/** SP割り振り関数 */
	bool CharaStatusReader::setAssignSPValue( int id, map<string, int> assignStatus ){
		bool result = true;
		vector<Status>::iterator it = find( mStatusValue.begin(), mStatusValue.end(), id );

		if ( it == mStatusValue.end() ) {
			result = false;
			goto FUNC_END;
		}

		for ( unsigned int i = 0; i < CharaDataXml::StatusStrVector.size(); i++ ) {
			it->assignSPValue[CharaDataXml::StatusStrVector.at(i)] 
			= assignStatus[CharaDataXml::StatusStrVector.at(i)];
		}

FUNC_END:
		return result;
	}
}