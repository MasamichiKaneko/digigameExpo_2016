#include "DxLib.h"
#include "Sequence\StageSelect.h"
#include "Sequence\Parent.h"
#include "Sequence\GameSequence\Parent.h"
#include "Sequence\GameSequence\Ready.h"
#include "utils.h"
#include "Sequence\CommonFunc.h"
#include "FileNames.h"
#include "define.h"
#include "Sequence\Title.h"
#include "CommonString.h"
#include "CommonConst.h"
#include "Score.h"
#include <sstream>
#include "Sequence\CharaBuild.h"
#include "StageConfig.h"

namespace Sequence{

	/** 描画位置 */
	const int StageSelect::SelectPosX = 70;
	const int StageSelect:: SelectPosY = 420;

	/** ステージアイコンサイズ */
	const int StageSelect::StageIconSizeX = 60;
	const int StageSelect::StageIconSizeY = 60;

	/** ステージアイコン描画開始位置 */
	const int StageSelect::StageIconPosX = 80;
	const int StageSelect::StageIconPosY = 30;

	/** ステージアイコン描画間隔 */
	const int StageSelect::drawIntervalIconX = 140;
	const int StageSelect::drawIntervalIconY = 100;

	/** アイコン配置数 */
	const int StageSelect::StageIconRow = 4;
	const int StageSelect::StageIconColumn = 2;

	/** スコア表示位置定義 */
	const int StageSelect::ShowScoreFramePosX = 60;
	const int StageSelect::ShowScoreFramePosY = 260;

	const int StageSelect::ShowScoreStrPosX = ShowScoreFramePosX + 100;
	const int StageSelect::ShowScoreStrPosY = ShowScoreFramePosY + 40;

	const int StageSelect::ShowFirstScorePosX = ShowScoreFramePosX + 200;
	const int StageSelect::ShowFirstScorePosY = ShowScoreFramePosY + 40;

	const int StageSelect::ShowScoreInterval = 25;

	const int StageSelect::ShowStageNumPosX  = ShowScoreFramePosX + 100;
	const int StageSelect::ShowStageNumPosY  = ShowScoreFramePosY + 15;

	/** フォントプロパティ */
	const int StageSelect::FontSize = 20;
	const int StageSelect::FontColor = GetColor( 0, 0, 0 );  //black
	const char* StageSelect::FontStyle = "HGS行書体";

	/** スコア表示桁数 */
	const int StageSelect::ShowScoreDigit = 8;

	/** メニュー描画間隔定義 */
	const int StageSelect::drawInterval = 190;

	/** 選択ステージ */
	int StageSelect::mSelection = 0;

	StageSelect::StageSelect(/* const char* stageData */)
		: mBackGroundHandle( 0 )
		, mLockImgHandle( 0 )
		, mCursorImgHandle( 0 )
		, commonFunc( 0 )
		, mShowScoreImgHandle( 0 )
	{
		if ( CheckSoundMem( Parent::instance()->getBGMHandle() ) == 0 ) {
			ASSERT( PlaySoundMem( Parent::instance()->getBGMHandle(), DX_PLAYTYPE_LOOP ) + 1 );
		}

		/** 上位からのポインタを参照させる */
		commonFunc = CommonFunc::getInstance();
		mScore = Parent::instance()->getScore();

		/* 画像読込み */
		mBackGroundHandle   = LoadGraph( FileNames::Image::StageSelectBack.c_str() );
		mLockImgHandle      = LoadGraph( FileNames::Image::StageLock.c_str() );
		mCursorImgHandle    = LoadGraph( FileNames::Image::StageSelectCursor.c_str() );
		mShowScoreImgHandle = LoadGraph( FileNames::Image::ShowScoreArea.c_str() );

		/** フォントハンドル作成 */
		mFontHandle = CreateFontToHandle( FontStyle, FontSize, -1,  DX_FONTTYPE_ANTIALIASING );
		ASSERT( mFontHandle + 1 );

		/* ステージアイコンのみ分割読込み */
		int buf[256];
		ASSERT( ( LoadDivGraph( FileNames::Image::StageIcon.c_str(), 
			CommonConst::TotalStageNum , StageIconRow , StageIconColumn , StageIconSizeX, StageIconSizeY, buf ) + 1) );
		for ( int i = 0; i < CommonConst::TotalStageNum; i++ ) {
			mStageIconHandles.push_back( buf[i] );
		}
		ASSERT( ( LoadDivGraph( FileNames::Image::LockStageIcon.c_str(), 
			CommonConst::TotalStageNum , StageIconRow , StageIconColumn , StageIconSizeX, StageIconSizeY, buf ) + 1) );
		for ( int i = 0; i < CommonConst::TotalStageNum; i++ ) {
			mLockStageIconHandles.push_back( buf[i] );
		}

		/* 読込みチェック */
		ASSERT(mBackGroundHandle   + 1);
		ASSERT(mLockImgHandle      + 1);
		ASSERT(mCursorImgHandle    + 1);
		ASSERT(mShowScoreImgHandle + 1);


		/** モード表示名取得 */
		mModeName.push_back( CommonStr::Select::StartModeStr );
		mModeName.push_back( CommonStr::Select::BuildModeStr );
		mModeName.push_back( CommonStr::Select::TitleModeStr );

		/* 解禁ステージ数読込み */
		mUnlockStageNum = mScore->getUnlockStageNum();
	}

	StageSelect::~StageSelect(){
		ASSERT(DeleteGraph(mBackGroundHandle)   + 1);
		ASSERT(DeleteGraph(mLockImgHandle)      + 1);
		ASSERT(DeleteGraph(mCursorImgHandle)    + 1);
		ASSERT(DeleteGraph(mShowScoreImgHandle) + 1);
		ASSERT(DeleteFontToHandle(mFontHandle)  + 1);
		for ( unsigned int i = 0; i < mStageIconHandles.size(); i++ ) {
			ASSERT(DeleteGraph( mStageIconHandles.at(i) ) + 1);
		}
		for ( unsigned int i = 0; i < mLockStageIconHandles.size(); i++ ) {
			ASSERT(DeleteGraph( mLockStageIconHandles.at(i) ) + 1);
		}
	}

	Base* StageSelect::update( Parent* parent ){
		Base* next = this;
		draw();
		mInputPrompt( &next, &parent );

		return next;
	}

	void StageSelect::draw() const {
		/** 背景描画 */
		DrawGraph(0,0,mBackGroundHandle,FALSE);
		/** ステージアイコン描画 */
		mDrawStageIcon();
		/** ステージ選択カーソル描画 */
		mDrawSelectCursor();
		/** TODO:スコア描画 */
		mDrawScore();

		/** モード選択フレーム描画 */
		commonFunc->drawMultiSelectFrame(
			SelectPosX,
			SelectPosY,
			drawInterval,
			0,
			mModeName,
			true);

		/** カーソル描画 */
		commonFunc->drawMousePointer();
	}

	/** スコア描画 */
	void StageSelect::mDrawScore() const {
		
		/** スコア取得 */
		vector<int> scores;
		scores = mScore->getScore()->find(mSelection)->second;

		DrawGraph( ShowScoreFramePosX, ShowScoreFramePosY,mShowScoreImgHandle,TRUE );
		/** 選択ステージ設定表示 */
		stringstream stageInfoStr;
		int initAsset = StageConfig::getInstance()->getConfigValue()->at( mSelection ).initAsset;
		int stdLevel  = StageConfig::getInstance()->getConfigValue()->at( mSelection ).stdLevelValue;
		
		stageInfoStr << CommonStr::Select::StageStr << mSelection + 1;
		stageInfoStr << "  " << CommonStr::Select::InitAssetStr << initAsset ;
		stageInfoStr << "  " << CommonStr::Select::StdLevelStr  << stdLevel;

		DrawStringToHandle( ShowStageNumPosX, ShowStageNumPosY, stageInfoStr.str().c_str(), FontColor, mFontHandle );
		
		/** スコア表示部分 */
		DrawStringToHandle( ShowScoreStrPosX, ShowScoreStrPosY, CommonStr::Select::ScoreStr.c_str(), FontColor, mFontHandle );
		for ( unsigned int i = 0;i < scores.size(); i++ ){
			/** 文字列変換 */
			string strScore = getZeroPadding( scores.at(i), ShowScoreDigit );
			stringstream drawStr;
			drawStr << i + 1 << ". " << strScore;
			DrawStringToHandle(ShowFirstScorePosX, ShowFirstScorePosY + i * ShowScoreInterval
				, drawStr.str().c_str(), FontColor , mFontHandle );
		}
	}

	/** 入力受付 */
	void StageSelect::mInputPrompt( Base** nextMode,Parent** parent ) {

		/** 左クリック入力状態なら遷移判定 */
		if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
			switch( commonFunc->getSelectFrame() ) {
			case STAGE:
				/** 選択中ステージへ遷移 */
				ASSERT( PlaySoundMem( Parent::instance()->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				ASSERT( StopSoundMem( (*parent)->getBGMHandle() ) + 1 );
				(*parent)->setMode(Parent::MODE_PLAY);
				*nextMode = new GameSequence::Parent( (*parent)->mode(),mSelection );
				break;
			/** 育成画面へ */
			case BUILD:
				ASSERT( PlaySoundMem( Parent::instance()->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				*nextMode = new CharaBuild();
				break;
			/** タイトル画面へ */
			case TITLE:
				ASSERT( PlaySoundMem( Parent::instance()->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				*nextMode = new Title(false);
				break;
			/** 上記以外 */
			default:
				/** ステージ選択判定 */
				for ( int i = 0; i < CommonConst::TotalStageNum; i++ ) {
					int posX = StageIconPosX + drawIntervalIconX * ( i % StageIconRow );
					int posY = StageIconPosY + drawIntervalIconY * ( i / StageIconRow );
					if ( commonFunc->isMousePointOnImage( posX, posY, mLockStageIconHandles.at( i ) )
						&& mUnlockStageNum > i) {
							ASSERT( PlaySoundMem( Parent::instance()->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
							mSelection = i;
							break;
					}
				}
				break;
			}
		}
	}

	/** ステージアイコン描画 */
	void StageSelect::mDrawStageIcon() const{

		for ( int handleId = 0; handleId < CommonConst::TotalStageNum; handleId++) {
			int drawPosX = StageIconPosX + drawIntervalIconX * ( handleId % StageIconRow );
			int drawPosY = StageIconPosY + drawIntervalIconY * ( handleId / StageIconRow );
			int drawHandle = 0;

			/** 未解禁ステージはグレースケールで暗く表示 */
			if ( mUnlockStageNum <= handleId ) {
				drawHandle = mLockStageIconHandles.at( handleId );
				/** 鍵描画 */
				SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
				int keySizeX,keySizeY;
				ASSERT( GetGraphSize(mLockImgHandle, &keySizeX, &keySizeY) + 1 );
				int keyPosX = drawPosX + ( ( StageIconSizeX - keySizeX ) / 2 );
				int keyPosY = drawPosY + ( StageIconSizeX + ( keySizeX / 2) );
				DrawGraph( keyPosX, keyPosY, mLockImgHandle, TRUE );
				SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2);
				/** 解禁ステージは選択中の場合のみ明るく */
			} else {
				drawHandle = mStageIconHandles.at( handleId );
				if ( commonFunc->isMousePointOnImage( drawPosX,drawPosY, mStageIconHandles.at(handleId))
					|| handleId == mSelection) {
					SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
				} else {
					SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2);
				}
			}
			DrawGraph( drawPosX, drawPosY, drawHandle,TRUE );
		}
		SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
	}

	/** 選択カーソル描画 */
	void StageSelect::mDrawSelectCursor() const {
		/** 描画位置算出 */
		int posX = StageIconPosX + ( ( mSelection % StageIconRow ) * drawIntervalIconX );
		int posY = StageIconPosY + ( ( mSelection / StageIconRow ) * drawIntervalIconY );
		
		/** サイズから補正値を割り出す */
		int sizeX, sizeY;
		ASSERT( GetGraphSize( mCursorImgHandle, &sizeX, &sizeY ) + 1 );
		posX -= (sizeX - StageIconSizeX) / 2;
		posY -= (sizeY - StageIconSizeY) / 2;

		DrawGraph( posX, posY, mCursorImgHandle, TRUE);
	}

}