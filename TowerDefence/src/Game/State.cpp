#include "Game\State.h"
#include "Charactor\Friend\FriendBase.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "Charactor\Enemy\Ocj.h"
#include "Game\StageMap.h"
#include "Game\CharaStatusReader.h"
#include "Game\EnemyStatusReader.h"
#include "Charactor\Friend\Plane.h"
#include "Charactor\Friend\Miko.h"
#include "Charactor\Friend\Majokko.h"
#include "Charactor\Friend\Yojo.h"
#include "Effect\Explosion.h"
#include "Effect\Shot.h"
#include "StageConfig.h"
#include "Loader.h"
#include "FileNames.h"
#include "DxLib.h"
#include "Configuration.h"
#include "utils.h"
#include "define.h"
#include "XMLNodeNameString.h"
#include "aes_encryption.h"
#include <algorithm>
#include "Sequence\CommonFunc.h"
#include "CommonConst.h"
#include "CommonString.h"
#include <sstream>
#include <boost\lexical_cast.hpp>

using namespace std;
using namespace FileNames;
using namespace Charactor;

namespace Game{

	/** 停止・再生ボタン描画位置 */
	const int State::PlayStopButtonPosX = 0;
	const int State::PlayStopButtonPosY = 0;

	/** 敵キャラHP表示縦幅 */
	const int State::GaugeDrawHeight = 5;

	/** アイコン描画位置 */
	const int State::CharaIconDrawX = 480;
	const int State::CharaIconDrawY = 0;

	/** アイコン描画間隔 */
	const int State::CharaIconDrawIntervalX = CommonConst::CharaDotImgSizeX;
	const int State::CharaIconDrawIntervalY = CommonConst::CharaDotImgSizeY;

	/** 状況描画位置 */
	const int State::AssetDrawPosX = 500;
	const int State::AssetDrawPosY = 290;
	const int State::ScoreDrawPosX = 500;
	const int State::ScoreDrawPosY = 330;
	const int State::EconomicDrawPosX = 530;
	const int State::EconomicDrawPosY = 235;

	/** ステータス描画位置 */
	const int State::StatusDrawPosX = 575;
	const int State::PowerValueDrawPosY = 120;
	const int State::SpeedValueDrawPosY = 140;
	const int State::RangeValueDrawPosY = 160;
	const int State::CostValueDrawPosY = 180;

	/** 状況表示開始位置 */
	const int State::StateDrawPosX = 480;
	const int State::StateDrawPosY = 0;

	/** ゲージカラー */
	const int State::EnemyHPGaugeColor     = GetColor( 0, UCHAR_MAX, 0);
	const int State::FriendSkillGaugeColor = GetColor( UCHAR_MAX, 0 ,0 );
	const int State::GaugeBackColor        = GetColor( 0, 0, 0 );
	const int State::GaugeFrameColor       = GetColor( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );

	/** 行ごとのキャラアイコン描画数 */
	const int State::CharaIconDrawNumRow = 4;

	/** キャラ描画位置補正 */
	const int State::FriendDrawCorr = -4;

	/** グリッドカラー */
	const int State::GridColor = GetColor( 0, 0, 0 );
	
	/** フォント関連 */
	const int State::FontSize = 16;
	const char* State::FontStyle = "HGS行書体";
	const int State::StringColor = GetColor( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );

	/** 表示桁数桁数 */
	const int State::AssetNumOfDisits = 8;
	const int State::ScoreNumOfDisits = 8;

	/** キャラ座標比較関数(並べ替え用) */
	bool CharactorPosCompare( const CharactorBase* right, const CharactorBase* left ){
		
		bool result = true;
		if ( right->getPoint().getY() != left->getPoint().getY() ) {
			result = right->getPoint().getY() < left->getPoint().getY();
		} else {
			result = right->getPoint().getX() < left->getPoint().getX();
		}
		
		return result;
	}

	/** 生存敵キャラ探索用 */
	bool FindSurviveEnemy( const Enemy::EnemyBase* enemy ) {
		return enemy->getSurvive();
	}

	/** 出現中敵キャラ探索用 */
	bool FindAppearanceEnemy( const Enemy::EnemyBase* enemy ){
		return enemy->getAppearance();
	}

	/** 描画対象敵キャラ探索用 */
	bool FindDrawTargetEnemy( const Enemy::EnemyBase* enemy ){
		return enemy->getSurvive() && enemy->getAppearance();
	}

	/** 読み込みデータを取得 */
	State::State( FileLoader* loader , int stageNum) : 
	mGameOver( false ), mStageMap( 0 ), mFriendData( 0 ), mScore( 0 ), mAssets( 0 ), mInitAssets( 0 ), mStageNum( 0 ){

		/** ステージ数取得 */
		mStageNum = stageNum;

		/** 自キャラステータスデータ読み込み */
		mFriendData = new CharaStatusReader();
		decrypt( FileNames::CharaStatus.c_str(), PassWord::EncryptPass );
		//ASSERT( mFriendData->readExpTable( FileNames::Text::ExpTableDataName.c_str() ) );
		if( !mFriendData->readCharaData( FileNames::CharaStatus.c_str() ) ){
			/** 失敗ならテンプレ読み込み */
			int statusHandle = FileRead_open( FileNames::Text::CharaDataTemplateName.c_str() );
			vector<string> readData;
			FileRead_getAll( &readData, statusHandle );
			FileWrite_vectorAll(readData,FileNames::CharaStatus.c_str());
			ASSERT( mFriendData->readCharaData( FileNames::Text::CharaDataTemplateName.c_str() ) );
		}
		encrypt( FileNames::CharaStatus.c_str(), PassWord::EncryptPass );

		/** キャラステータス取得 */
		vector<Game::CharaStatusReader::Status>::iterator statusIt = mFriendData->getStatusValue()->begin();
		for ( ; statusIt != mFriendData->getStatusValue()->end() ; statusIt++ ) {
			mStatusValues[ statusIt->id ] = statusIt->TotalValue;
		}

		/** マップデータ読み込み */
		mStageMap = new StageMap( loader->getTextFileHandle()->at(Text::MapInfoFileVector.at( stageNum )) );
		/** 敵キャラステータス読み込み */
		mEnemyData = new EnemyStatusReader( loader->getTextFileHandle()->at( Text::EnemyStatusData ),
			loader->getTextFileHandle()->at( Text::EnemyAppearanceVector.at( stageNum ) ), mStageMap );
		/** 用済みのハンドル開放 */
		loader->deleteTextHandleAll();

		/** 敵キャラ出現情報読み込み */
		mEnemys = mEnemyData->getEnemysData();

		/** お賽銭額受け取り */
		mInitAssets = StageConfig::getInstance()->getConfigValue()->at( stageNum ).initAsset;
		mAssets = mInitAssets;

		/** ハンドル受け取り */
		mMapChip = loader->getDivImageFileHanlde()->at( Image::MapChipImg );
		mTempleHandle = loader->getImageFileHandle()->at( Image::TempleImg );
		mRangeHandle = loader->getImageFileHandle()->at( Image::RangeImg );
		mSelectionFrameHandle = loader->getImageFileHandle()->at( Image::CharaBuildCursor );
		mStateBackHandle = loader->getImageFileHandle()->at( Image::StateBackImage );
		mEconomicIconHandle = loader->getDivImageFileHanlde()->at( Image::EconomicIconImage );

		/** フォントハンドル設定 */
		mFontHandle = CreateFontToHandle( FontStyle, FontSize, -1, DX_FONTTYPE_ANTIALIASING);

		/** 自キャラドット絵 */
		for ( unsigned int i = 0; i < FileNames::Image::DotImagesVector.size(); i++ ) {
			string imageName = FileNames::Image::DotImagesVector.at( i );
			/** ドット絵ハンドル取得 */
			mCharaIconHandles[imageName] = loader->getDivImageFileHanlde()->at(imageName);
			/** 選択用キャラアイコンに追加 */
			mCharaSelectIconHandles.push_back( mCharaIconHandles.at(imageName).at( 0 ) );
		}
		/** 売却アイコン追加 */
		mCharaSelectIconHandles.push_back( loader->getImageFileHandle()->at( FileNames::Image::SaleIconImage ) ); 
		/** 覚醒アイコン追加 */
		mCharaSelectIconHandles.push_back( loader->getImageFileHandle()->at( FileNames::Image::AwakeIconImage ) );
		/** 描画位置計算 */
		for ( unsigned int i = 0; i < mCharaSelectIconHandles.size(); i++ ) {
			/** アイコン先頭ハンドルを受け取る */
			int posX = CharaIconDrawX + ( ( i % CharaIconDrawNumRow ) * CharaIconDrawIntervalX );
			int posY = CharaIconDrawY + ( ( i / CharaIconDrawNumRow ) * CharaIconDrawIntervalY );
			mIconDrawPos.push_back( Point( posX, posY ) );
		}

		/** 敵キャラドット絵 */
		for ( unsigned int i = 0; i < FileNames::Image::EnemyDotImageVector.size(); i++ ) {
			string imageName = FileNames::Image::EnemyDotImageVector.at( i );
			/** ドット絵ハンドル受け取り */
			mEnemyIconHandles[imageName] = loader->getDivImageFileHanlde()->at(imageName);
		}

		/** エフェクト画像取得 */
		for ( unsigned int i = 0; i < FileNames::Image::EffectImagesVector.size(); i++ ) {
			string imageName = FileNames::Image::EffectImagesVector.at( i );
			/** ハンドル受け取り */
			mEffectImageHandles[imageName] = loader->getDivImageFileHanlde()->at(imageName);
		}

		/** 音声ハンドル受け取り */
		mExplosionSEHandle = loader->getSoundFileHandle()->at( FileNames::Sound::ExplosionSEName );

		/** 最初は選択無しの状態 */
		mSelection = Plane;

		/** グリッド濃度計算 */
		int maxValue = Configuration::getInstance()->getValue().at(ConfigData::MaxValueNode).at(ConfigData::GridLevelStr);
		int setValue = Configuration::getInstance()->getValue().at(ConfigData::SetValueNode).at(ConfigData::GridLevelStr);
		float ratio  =  static_cast<float>( setValue ) / static_cast<float>(maxValue);
		mGridDipth   = static_cast<int>( UCHAR_MAX * ratio );
	}

	State::~State(){
		SAFE_DELETE( mFriendData );
		SAFE_DELETE( mEnemyData  );
		SAFE_DELETE( mStageMap   );
	}

	void State::update(){

		/** 自キャラ動作 */
		for ( unsigned int i = 0; i < mFriends.size(); i++ ) {
			mFriends.at(i)->update( &mEnemys, &mFriends, mStageMap );
		}

		/** 倒した敵キャラ削除 */
		deleteDeadEnemys();

		/** 敵キャラ動作 */
		for ( unsigned int i = 0; i < mEnemys.size(); i++ ) {
			mEnemys.at(i)->update( &mEnemys, &mFriends, mStageMap );
		}

		/** 到達敵キャラ削除 */
		deleteArrivalEnemys();

		/** エフェクト更新 */
		for ( unsigned int i = 0; i < mEnemyExplosions.size() ; i++ ) {
			mEnemyExplosions.at(i)->update();
		}

		deleteEnemyExplosions();

		/** ゲームオーバー判定 */
		/** 資金が尽きるか敵全滅で終了 */
		if ( mAssets <= 0 || mEnemys.size() == 0 ) {
			mGameOver = true;
		}
	}

	/** やられた敵削除 */
	void State::deleteDeadEnemys(){
		vector<Enemy::EnemyBase*>::iterator EnemyIt;
		EnemyIt = mEnemys.begin();
		for ( ; EnemyIt != mEnemys.end() ; ) {
			/** 生存中かつHP0以下の敵がいたら */
			if ( (*EnemyIt)->getHitPoint() <= 0 && (*EnemyIt)->getSurvive() ) {
				/** スコア追加 */
				mScore += (*EnemyIt)->getScore();
				/** 資金追加 */
				mAssets += (*EnemyIt)->getAssets();
				/** 生存フラグ回収 */
				(*EnemyIt)->setSurvive( false );
				/** 爆破エフェクトセット */
				int size = CommonConst::EnemyDotImgSizeXVector.at( (*EnemyIt)->getId() );
				Effect::Explosion* effect = new Effect::Explosion( (*EnemyIt)->getPoint() ,1 , size);
				mEnemyExplosions.push_back( effect );
				/** 爆発音再生 */
				ASSERT( PlaySoundMem( mExplosionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
				EnemyIt = mEnemys.erase( EnemyIt );
			} else {
				EnemyIt++;
			}
		}
	}

	/** 到達した敵削除 */
	void State::deleteArrivalEnemys(){
		vector<Enemy::EnemyBase*>::iterator EnemyIt;
		EnemyIt = mEnemys.begin();
		for ( ; EnemyIt != mEnemys.end(); ) {
			/** 生存中の敵が到達していたら */
			if ( (*EnemyIt)->getDistanceToGoal() == 0 && (*EnemyIt)->getSurvive() ) {
				/** 総資金減少 */
				mAssets -= (*EnemyIt)->getAssetDamage();
				if ( mAssets < 0 ) {
					mAssets = 0;
				}
				/** 生存フラグ回収 */
				(*EnemyIt)->setSurvive( false );
				EnemyIt = mEnemys.erase( EnemyIt );
			} else {
				EnemyIt++;
			}
		}
	}

	/** 爆発エフェクト削除 */
	void State::deleteEnemyExplosions(){
		vector<Effect::Explosion*>::iterator It;
		It = mEnemyExplosions.begin();
		for ( ;It != mEnemyExplosions.end(); ) {
			if ( !(*It)->getIsDraw() ) {
				It = mEnemyExplosions.erase( It );
			} else {
				It++;
			}
		}
	}

	/** 自キャラ配置関数 */
	Friend::FriendBase* State::getAddFriend( int posX, int posY ) const{

		int sizeX = mStageMap->getMapSizeX();
		int sizeY = mStageMap->getMapSizeY();

		Friend::FriendBase* retFriend = NULL;

		Point p(posX, posY);

		/** idチェック */
		vector< CharaStatusReader::Status >::iterator it;
		it = find(mFriendData->getStatusValue()->begin(),mFriendData->getStatusValue()->end(), mSelection);
		
		/** キャラデータから受け取る各値 */
		int necessaryCost, skill, power, speed, range, level;

		/** 存在するidならキャラの各設定値を取得 */
		if ( it != mFriendData->getStatusValue()->end() ) {
			necessaryCost = it->TotalValue.at( CharaDataXml::CostStr );
			skill = it->TotalValue.at( CharaDataXml::SkillStr );
			power = it->TotalValue.at( CharaDataXml::PowerStr );
			speed = it->TotalValue.at( CharaDataXml::SpeedStr );
			range = it->TotalValue.at( CharaDataXml::RangeStr );
			level = it->lv;
		} else {
			/** 存在しないidなら置けない */
			goto FUNC_END;
		}

		/** 資産が足りなければ置けない */
		if ( mAssets <= necessaryCost ) {
			goto FUNC_END;
		}
		
		/** マップチェック */
		/** 指定座標がマップの大きさ以内かチェック */
		bool coordinateCheck = true;
		coordinateCheck &= ( ( 0 <= posX ) && ( posX < sizeX ));
		coordinateCheck &= ( ( 0 <= posY ) && ( posY < sizeY ));
		if ( !coordinateCheck ) {
			goto FUNC_END;
		}

		/** 配置可能なマップチップidかチェック */
		if ( mStageMap->getMapInfo( posX, posY ).getId() != PlaneChip ) {
			goto FUNC_END;
		}

		/** 自キャラが置かれていないかチェック */
		if ( mStageMap->getMapInfo( posX, posY ).getIsOnCharactor() ) {
			goto FUNC_END;
		}

		/** 座標で置く位置を取得 */
		p.setX( p.getX() * mStageMap->getChipSize() );
		p.setY( p.getY() * mStageMap->getChipSize() );

		/** 攻撃範囲補正 */
		/** 立っているマス分を追加 */
		range += static_cast< int >( CommonConst::RangeBasicUnit / 2 );
		/** 基本単位ごとに1マス分に調節する */
		range = static_cast< int >( range * ( static_cast< double >( CommonConst::MapChipSize ) / CommonConst::RangeBasicUnit ) );

		/** 自キャラを追加 */
		switch( mSelection ){
		case Plane:
			retFriend = new Friend::Plane(skill, power, speed, range, mSelection, level, p);
			break;
		case Miko:
			retFriend = new Friend::Miko(skill, power, speed, range, mSelection, level, p);
			break;
		case Majokko:
			retFriend = new Friend::Majokko(skill, power, speed, range, mSelection, level, p);
			break;
		case Yojo:
			retFriend = new Friend::Yojo(skill, power, speed, range, mSelection, level, p);
			break;

		case Non:
		default:
			goto FUNC_END;
			break;
		}

FUNC_END:
		return retFriend;
	}

	/** 自キャラ売却関数 */
	bool State::mSaleFriend( int posX, int posY ){
		bool result = false;
		vector < Friend::FriendBase* >::iterator friendIt;
		/** マップ上指定マスにキャラが置かれているかの判定 */
		if ( !mStageMap->getMapInfo(posX, posY).getIsOnCharactor() ) {
			/** 置かれていなければ何もせず関数処理終了 */
			goto FUNC_END;
		}

		/** 座標と一致するキャラを探索 */
		for ( friendIt = mFriends.begin(); friendIt != mFriends.end(); friendIt++ ) {
			/** 座標が一致ならループを抜ける */
			if ( ( (*friendIt)->getPoint().getX() == ( posX * CommonConst::MapChipSize ) )
				&& ( (*friendIt)->getPoint().getY() == ( posY * CommonConst::MapChipSize ) ) ) {
				break;
			}
		}

		/** 売却処理 */
		if ( friendIt != mFriends.end() ) {
			/** コストの半分を資金に還元 */
			int cost = mFriendData->getStatusValue()->at( (*friendIt)->getId() ).TotalValue.at( CharaDataXml::CostStr );
			mAssets += (cost / 2);
			/** 指定マスをキャラが置かれていない状態にする */
			mStageMap->setOnFriendFlag(posX, posY, false);
			mFriends.erase( friendIt );
			result = false;
		}

FUNC_END:
		return result;
	}

	/** 自キャラ覚醒関数 */
	bool State::mAwakeFriend( int posX, int posY ){
		bool result = false;
		vector < Friend::FriendBase* >::iterator friendIt;
		/** マップ上指定マスにキャラが置かれているかの判定 */
		if ( !mStageMap->getMapInfo(posX, posY).getIsOnCharactor() ) {
			/** 置かれていなければ何もせず関数処理終了 */
			goto FUNC_END;
		}

		/** 座標と一致するキャラを探索 */
		for ( friendIt = mFriends.begin(); friendIt != mFriends.end(); friendIt++ ) {
			/** 座標が一致ならループを抜ける */
			if ( ( (*friendIt)->getPoint().getX() == ( posX * CommonConst::MapChipSize ) )
				&& ( (*friendIt)->getPoint().getY() == ( posY * CommonConst::MapChipSize ) ) ) {
				break;
			}
		}

		/** 覚醒フラグをセット */
		if ( friendIt != mFriends.end() ) {
			if ( ( (*friendIt)->getGauge() == (*friendIt)->getGaugeMax() ) && !(*friendIt)->getIsAwake() ) {
				(*friendIt)->setIsAwake( true );
			}
		}

FUNC_END:
		return result;
	}

	/** 全体の描画 */
	void State::draw( bool selectionDraw ) const{
		drawMap( selectionDraw );
		drawFriendsRange();
		
		
		/** キャラ描画 */
		drawCharactors( selectionDraw );

		/** エフェクト描画 */
		if ( mEnemyExplosions.size() > 0 ) {
			vector< Effect::EffectBase* > effect;
			effect.insert( effect.begin(), mEnemyExplosions.begin(), mEnemyExplosions.end() );
			drawEffect( effect );
		}
		
		for ( unsigned int i = 0 ; i < mFriends.size(); i++ ) {
			if ( mFriends.at(i)->getEffect().size() > 0 ) {
				drawEffect( mFriends.at(i)->getEffect() );
			}
		}
		/** ゲーム状況描画 */
		drawState();
		drawCharaIcon( selectionDraw );
	}

	/** マップ描画 */
	void State::drawMap( bool selectionDraw ) const{
		int mapSizeY  = mStageMap->getMapSizeY();
		int mapSizeX  = mStageMap->getMapSizeX();
		int chipSizeX = mStageMap->getChipSize();
		int chipSizeY = mStageMap->getChipSize();

		/** チップ描画 */
		for ( int y = 0 ; y < mapSizeY; y++ ){
			int posY = y *( mStageMap->getChipSize() );
			for ( int x = 0; x < mapSizeX; x++ ) {
				int posX = x * mStageMap->getChipSize();
				int mapChipId = mStageMap->getMapInfo( x, y ).getId();
				DrawGraph( posX, posY, mMapChip.at(mapChipId), TRUE );
			}
		}
		
		/** マウス位置取得 */
		int mousePosX, mousePosY;
		ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );
		

		/** マウスがマップ上にある場合は輝度を上げる */
		bool mousePointOnMap = isPointOnMap( mousePosX, mousePosY );

		if( mousePointOnMap && selectionDraw ) {
			/** マップチップの番号計算 */
			int mapChipX = mousePosX / mStageMap->getChipSize();
			int mapChipY = mousePosY / mStageMap->getChipSize();
			int posX = mapChipX * mStageMap->getChipSize();
			int posY = mapChipY * mStageMap->getChipSize();
			SetDrawBlendMode( DX_BLENDMODE_ADD,  CHAR_MAX / 2 );
			DrawBox( posX ,posY
				, posX + mStageMap->getChipSize()
				, posY + mStageMap->getChipSize()
				, GetColor( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX )
				, TRUE);
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );
		}

		/** 神社描画 */
		DrawGraph( 0, 0, mTempleHandle , TRUE );

		/** グリッド描画 */
		ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,mGridDipth ) + 1 );
		drawGrid( 0, 0, mapSizeY + 1, mapSizeX + 1, chipSizeY , chipSizeX, GridColor );
		ASSERT( SetDrawBlendMode( DX_BLENDMODE_NOBLEND ,UCHAR_MAX ) + 1 );
	}

	/** 自キャラ攻撃範囲描画 */
	void State::drawFriendsRange() const{

		/** 攻撃範囲描画 */
		for ( unsigned int i = 0; i < mFriends.size(); i++ ) {
			/** 範囲倍率計算 */
			int distance = mFriends.at(i)->getRange();
			/** 描画頂点位置計算 */

			int upperLeftX  = mFriends.at(i)->getPoint().getX() + (mStageMap->getChipSize() / 2);
			int upperLeftY  = mFriends.at(i)->getPoint().getY() + (mStageMap->getChipSize() / 2);
			int lowerRightX = mFriends.at(i)->getPoint().getX() + (mStageMap->getChipSize() / 2);
			int lowerRightY = mFriends.at(i)->getPoint().getY() + (mStageMap->getChipSize() / 2);
			upperLeftX  -= distance;
			upperLeftY  -= distance;
			lowerRightX += distance;
			lowerRightY += distance;
			/** 描画 */
			ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX / 10 ) + 1 );
			ASSERT( DrawExtendGraph( upperLeftX, upperLeftY, lowerRightX, lowerRightY, mRangeHandle, TRUE ) + 1 );
			ASSERT( SetDrawBlendMode( DX_BLENDMODE_NOBLEND ,UCHAR_MAX ) + 1 );

		}

	}

	/** キャラ描画 */
	void State::drawCharactors( bool selectionDraw ) const{

		/** キャラクタ配列を用意 */
		vector< CharactorBase* > charactors = getDrawTargets();
		
		/** 選択中のキャラがいてマウスポインタがマップ上にあればマップ上に表示させるために追加 */
		int mousePosX, mousePosY;
		ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );
		int throughFriendPosX = -1;
		int throughFriendPosY = -1;
		if ( isPointOnMap( mousePosX, mousePosY ) && selectionDraw ) {
			int mapPosX = mousePosX / mStageMap->getChipSize();
			int mapPosY = mousePosY / mStageMap->getChipSize();
			Friend::FriendBase* addFriend = getAddFriend( mapPosX, mapPosY );
			if ( addFriend != NULL ) {
				charactors.push_back( addFriend );
				throughFriendPosX = addFriend->getPoint().getX();
				throughFriendPosY = addFriend->getPoint().getY();
			}
		}

		/** 座標順にソート */
		sort( charactors.begin(), charactors.end(), CharactorPosCompare );

		/** 描画 */
		for ( vector< CharactorBase* >::iterator it = charactors.begin(); it != charactors.end() ;it++ ) {
			
			int handle = 0;
			int id = (*it)->getId();
			int motion = (*it)->getMotionId();
			int gaugeColor = 0;
			/** idとキャラタイプから描画するハンドル取得 */
			try{
				if ( (*it)->getCharactorType() == Friend::CharactorType ) {
					handle = mCharaIconHandles.at( FileNames::Image::DotImagesVector.at(id) ).at(motion);
					gaugeColor = FriendSkillGaugeColor;
				} else if( (*it)->getCharactorType() == Enemy::CharactorType ) {
					handle = mEnemyIconHandles.at( FileNames::Image::EnemyDotImageVector.at(id) ).at(motion);
					gaugeColor = EnemyHPGaugeColor;
				} else {
					ASSERT( false );
				}
				/** 描画位置計算 */
				int drawPosX = (*it)->getPoint().getX();
				int drawPosY = (*it)->getPoint().getY();
				int sizeX, sizeY;
				ASSERT( GetGraphSize( handle, &sizeX, &sizeY ) + 1 );
				drawPosX -= ( ( sizeX - mStageMap->getChipSize() ) / 2 );
				drawPosY -= ( ( sizeY - mStageMap->getChipSize() ) );
				drawPosY += FriendDrawCorr;
				/** キャラ描画 */
				if ( throughFriendPosX == (*it)->getPoint().getX() && throughFriendPosY == (*it)->getPoint().getY()
					&& (*it)->getCharactorType() == Friend::CharactorType ) {
						SetDrawBlendMode( DX_BLENDMODE_ALPHA, UCHAR_MAX/3 );
						ASSERT( DrawGraph( drawPosX, drawPosY, handle, TRUE ) + 1 );
						SetDrawBlendMode( DX_BLENDMODE_ALPHA, UCHAR_MAX );
				} else {
					ASSERT( DrawGraph( drawPosX, drawPosY, handle, TRUE ) + 1 );
					/** ゲージ描画 */
					drawPosX = (*it)->getPoint().getX();
					int drawPosLowerX = drawPosX + mStageMap->getChipSize();
					int drawPosLowerY = drawPosY;
					int drawPosUpperY = drawPosY - GaugeDrawHeight;
					double gaugePerce = static_cast< double >( (*it)->getGauge() );
					gaugePerce /= static_cast< double >( (*it)->getGaugeMax() );
					int geugeValuePos = drawPosX + static_cast< int >( mStageMap->getChipSize() * gaugePerce );
					ASSERT( DrawBox( drawPosX, drawPosUpperY, drawPosLowerX, drawPosLowerY, GaugeBackColor, TRUE ) + 1 );
					if ( (*it)->getCharactorType() == Friend::CharactorType && ( (*it)->getGauge() != (*it)->getGaugeMax() ) ) {
						int value = static_cast< int >( UCHAR_MAX * 0.8 );
						ASSERT( SetDrawBright( value, value ,value ) + 1 );
					}
					ASSERT( DrawBox( drawPosX, drawPosUpperY, geugeValuePos, drawPosLowerY, gaugeColor, TRUE ) + 1 );
					if ( (*it)->getCharactorType() == Friend::CharactorType && ( (*it)->getGauge() != (*it)->getGaugeMax() ) ) {
						ASSERT( SetDrawBright( UCHAR_MAX, UCHAR_MAX ,UCHAR_MAX ) + 1 );
					}
					ASSERT( DrawBox( drawPosX, drawPosUpperY, drawPosLowerX, drawPosLowerY, GaugeFrameColor, FALSE ) + 1 );
				}
			} catch( out_of_range& ) {
				/** TODO:原因不明のため、暫定対応 */
			}
#ifdef _DEBUG
			ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX / 5 ) + 1 );
			DrawCircle( (*it)->getPoint().getX(), (*it)->getPoint().getY(), (*it)->getRange() , GetColor(UCHAR_MAX, UCHAR_MAX, 0));
			ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX ) + 1 );
#endif
			}
		
	}

	/** 描画対象の敵取得関数 */
	vector< CharactorBase* > State::getDrawTargets() const{
	//	try{
		vector< CharactorBase* > charactors;
		unsigned int drawEnemyNum = count_if( mEnemys.begin(), mEnemys.end(), FindDrawTargetEnemy );
		charactors.reserve( drawEnemyNum );
		/** 描画対象の敵キャラ取得 */
		vector< Enemy::EnemyBase* >::const_iterator targetEnemy = mEnemys.begin();
		for (;;) {
			targetEnemy = find_if( targetEnemy, mEnemys.end(), FindDrawTargetEnemy );

			if ( targetEnemy == mEnemys.end() ) {
				break;
			} else {
				charactors.push_back( *targetEnemy );
				targetEnemy++;
			}
		}

		/** 対象の自キャラ取得 */
		charactors.insert(charactors.begin(), mFriends.begin(), mFriends.end());
		return charactors;
		/*} catch( out_of_range& e ) {
			e;
		}*/
	}

	/** TODO:状況描画 */
	void State::drawState() const{

		/** バック表示 */
		DrawGraph( StateDrawPosX, StateDrawPosY, mStateBackHandle, TRUE );

		/** 選択中のキャラステータス描画 */
		stringstream powerStr;
		stringstream speedStr;
		stringstream rangeStr;
		stringstream costStr;
		switch( mSelection ) {
		case Plane:
		case Miko:
		case Majokko:
		case Yojo:
			{
				/** ステータス値を文字列で取得 */
				vector< CharaStatusReader::Status >::iterator it;
				it = find(mFriendData->getStatusValue()->begin(),mFriendData->getStatusValue()->end(), mSelection);
				powerStr << boost::lexical_cast<string>( it->TotalValue.at( CharaDataXml::PowerStr ) );
				double speedValue = static_cast<double>( it->TotalValue.at( CharaDataXml::SpeedStr ) );
				speedValue /= static_cast<double>( Configuration::getInstance()->getFrameRate() );
				speedStr << fixed << setprecision(2) << speedValue << CommonStr::Common::SecondStr;
				double rangeValue = static_cast<double>( it->TotalValue.at( CharaDataXml::RangeStr ) );
				rangeValue /= static_cast<double>( CommonConst::RangeBasicUnit );
				rangeStr << fixed << setprecision(2) << rangeValue << CommonStr::Common::TroutStr;
				costStr << it->TotalValue.at( CharaDataXml::CostStr );
			}
			break;
		case Sale:
		case Awake:
		case Non:
			powerStr << CommonStr::Status::NoStatusStr;
			speedStr << CommonStr::Status::NoStatusStr;
			rangeStr << CommonStr::Status::NoStatusStr;
			costStr << CommonStr::Status::NoStatusStr;
			break;
		default:
			ASSERT( false );
			break;
		}
		DrawStringToHandle( StatusDrawPosX, PowerValueDrawPosY, powerStr.str().c_str(), StringColor, mFontHandle );
		DrawStringToHandle( StatusDrawPosX, SpeedValueDrawPosY, speedStr.str().c_str(), StringColor, mFontHandle );
		DrawStringToHandle( StatusDrawPosX, RangeValueDrawPosY, rangeStr.str().c_str(), StringColor, mFontHandle );
		DrawStringToHandle( StatusDrawPosX, CostValueDrawPosY , costStr.str().c_str() , StringColor, mFontHandle );

		/** 赤字黒字判定結果描画 */
		int imgNum = 0;
		if ( mAssets < mInitAssets ) {
			/** 赤字なら切り替え */
			imgNum = 1;
		}
		DrawGraph( EconomicDrawPosX, EconomicDrawPosY, mEconomicIconHandle.at( imgNum ) , TRUE );

		/** お賽銭額描画 */
		DrawStringToHandle( AssetDrawPosX, AssetDrawPosY, getZeroPadding( mAssets, AssetNumOfDisits ).c_str() , StringColor, mFontHandle );

		/** スコア描画   */
		DrawStringToHandle( ScoreDrawPosX, ScoreDrawPosY, getZeroPadding( mScore, ScoreNumOfDisits ).c_str() , StringColor, mFontHandle );

	}

	/** アイコン描画 */
	void State::drawCharaIcon( bool selectionDraw ) const{

		for ( unsigned int i = 0; i < mCharaSelectIconHandles.size(); i++ ) {
			/** アイコン先頭ハンドルを受け取る */
			int handle = mCharaSelectIconHandles.at( i );
			int posX = mIconDrawPos.at(i).getX();
			int posY = mIconDrawPos.at(i).getY();
			if ( ( i == mSelection ) || Sequence::CommonFunc::isMousePointOnImage( posX, posY, handle ) && selectionDraw ) {
				ASSERT( SetDrawBright( UCHAR_MAX , UCHAR_MAX , UCHAR_MAX ) + 1 );
			} else {
				ASSERT( SetDrawBright( UCHAR_MAX / 2, UCHAR_MAX / 2, UCHAR_MAX / 2 ) + 1 );
			}
			ASSERT( DrawGraph( mIconDrawPos.at(i).getX(), mIconDrawPos.at(i).getY(), handle,TRUE ) + 1 );
		}
		ASSERT( SetDrawBright( UCHAR_MAX , UCHAR_MAX , UCHAR_MAX ) + 1 );
		/** 選択中キャラを枠で囲う */
		if ( mSelection != Non ) {
			int posX = mIconDrawPos.at(mSelection).getX();
			int posY = mIconDrawPos.at(mSelection).getY();
			ASSERT( DrawGraph( posX, posY, mSelectionFrameHandle, TRUE ) + 1 );
		}
	}

	/** エフェクト描画 */
	void State::drawEffect( vector<Effect::EffectBase*> effect ) const{

		vector<Effect::EffectBase*>::const_iterator It = effect.begin();
		for ( ; It != effect.end(); It++ ) {
			if ( (*It)->getIsDraw() ) {
				string imageName = FileNames::Image::EffectImagesVector.at( (*It)->getId() );
				int panelId = (*It)->getPanel();
				int handle = mEffectImageHandles.at(imageName).at(panelId);
				/** 描画位置計算 */
				int sizeX = (*It)->getSize();
				int sizeY = (*It)->getSize();
				int upperPosX = (*It)->getPoint().getX() - ( ( ( sizeX - CommonConst::MapChipSize ) / 2 ) );
				int upperPosY = (*It)->getPoint().getY() - ( ( ( sizeY - CommonConst::MapChipSize ) / 2 ) );
				int lowerPosX = upperPosX + (*It)->getSize();
				int lowerPosY = upperPosY + (*It)->getSize();
				ASSERT(  DrawExtendGraph( upperPosX, upperPosY, lowerPosX, lowerPosY, handle, TRUE ) + 1 );
#ifdef _DEBUG
				ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX / 5 ) + 1 );
				DrawCircle( (*It)->getPoint().getX(), (*It)->getPoint().getY(), (*It)->getSize() / 2 , GetColor(UCHAR_MAX, UCHAR_MAX, 0));
				ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX ) + 1 );
#endif
			}
		}
	}

	/** 座標がマップ上であるかどうかの判定 */
	bool State::isPointOnMap( int posX, int posY ) const{
		int mapSizeX = mStageMap->getChipSize() * mStageMap->getMapSizeX();
		int mapSizeY = mStageMap->getChipSize() * mStageMap->getMapSizeY();
		bool pointOnMap = ( 0 <= posX ) && ( 0 <= posY );
		pointOnMap &= ( posX < mapSizeX ) && ( posY < mapSizeY );
		
		return pointOnMap;
	}

	/** 入力受付 */
	/** posX,posY: マウス座標 */
	void State::mInputPrompt( int posX, int posY ){
		
		/** マップ部分判定 */
		if ( posX < ( mStageMap->getMapSizeX() * mStageMap->getChipSize() ) ){
			/** マップのどのマスかを取得 */
			int mapPosX = posX / mStageMap->getChipSize();
			int mapPosY = posY / mStageMap->getChipSize();
			
			/** 置けたかどうかの判定 */
			switch( mSelection ) {
			/** キャラ選択中なら設置 */
			case Plane:
			case Miko:
			case Majokko:
			case Yojo:
				{
					Friend::FriendBase* addFriend = getAddFriend( mapPosX, mapPosY );
					if ( addFriend != NULL ) {
						mFriends.push_back( addFriend );
						mStageMap->setOnFriendFlag( mapPosX, mapPosY, true );
						mAssets -= mFriendData->getStatusValue()->at( addFriend->getId() ).TotalValue.at( CharaDataXml::CostStr );
					}
				}
				break;
			/** 売却選択時 */
			case Sale:
				{
					bool isSale = false;
					isSale = mSaleFriend( mapPosX, mapPosY );
				}
				break;
			/** 覚醒 */
			case Awake:
				{
					bool isAwake = false;
					isAwake = mAwakeFriend( mapPosX, mapPosY );
				}
				break;
			case Non:
				break;
			default:
				break;
			}
			/** ここで置けたかどうかで描画判定するかも */
		/** 状況表示部分判定 */
		} else {
			/** キャラアイコン判定 */
			for ( unsigned int i = 0; i < mIconDrawPos.size(); i++ ) {
				/** ハンドル受け取り */
				int handle = mCharaSelectIconHandles.at(i);
				if ( Sequence::CommonFunc::isMousePointOnImage( mIconDrawPos.at(i).getX(), mIconDrawPos.at(i).getY(), handle ) ) {
					/** キャラidセット */
					mSetCharaType( i );
					break;
				}
			}
		}
	}

	void State::mShiftSelection() {
		
		if ( mSelection == Non ) {
			return;
		}
		
		int value = mSelection + 1;
		if ( value == Non ) {
			value = Plane;
		}
		mSetCharaType( value );
	}

	void State::mSetCharaType( int aCharaTypeId ) {
		switch( aCharaTypeId ) {
			/** 存在するidの場合 */
			case Plane:
			case Miko:
			case Majokko:
			case Yojo:
			case Sale:
			case Awake:
			case Non:
				mSelection = static_cast< FriendType >( aCharaTypeId );
				break;
			/** それ以外は何もしない */
			default:
				break;
			}
	}


}