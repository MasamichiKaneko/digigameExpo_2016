#include "define.h"
#include "DxLib.h"
#include "Sequence\Title.h"
#include "Sequence\Parent.h"
#include "Sequence\StageSelect.h"
#include "Sequence\Option.h"
#include "Configuration.h"
#include "Sequence\GameSequence\Parent.h"
#include "FileNames.h"
#include "utils.h"
#include "CommonString.h"
#include "Sequence\CommonFunc.h"

using namespace std;

namespace Sequence{

	int Title::mSelection = 0;

	/** 描画位置 */
	const int Title::StandImgPosX = 30;
	const int Title::StandImgPosY = 120;

	const int Title::LogoImgPosX = 30;
	const int Title::LogoImgPosY = 10;

	const int Title::SelectPosX = 260;
	const int Title::SelectPosY = 260;

	/* メニュー描画間隔定義 */
	const int Title::drawInterval = 65;

	const unsigned int colorWhite = GetColor( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );

	Title::Title(bool Production)
		: mBackGroundHandle( 0 ),
		mTitleLogoHandle( 0 ),
		mStandImageHandle( 0 ),
		mTime( 0 ),bright( 0 ),
		commonFunc( 0 ){

			/** 共通処理クラスインスタンスを参照させる */
			commonFunc = CommonFunc::getInstance();
			/* 画像読み込み */
			mBackGroundHandle = LoadGraph(FileNames::Image::TitleBack.c_str());
			mTitleLogoHandle  = LoadGraph(FileNames::Image::TitleLogo.c_str());

			/* 読込みエラーチェック */
			ASSERT( mTitleLogoHandle  + 1);
			ASSERT( mBackGroundHandle + 1);

			/** ウィンドウサイズ取得 */
			ASSERT( GetWindowSize( &windowSizeX, &windowSizeY ) + 1);

			/** タイトルではマウスカーソルは非表示 */
			ASSERT( SetMouseDispFlag( FALSE ) + 1 );

			/* 読込み立ち絵を乱数で決める */
			int StandImageNum = GetRand( static_cast<int>(FileNames::Image::StandImagesVector.size()) - 1 );
			mStandImageHandle = LoadGraph( FileNames::Image::StandImagesVector.at(StandImageNum).c_str() );
			ASSERT( mStandImageHandle + 1);

			/* タイトル演出 */
			bright = new FadeInFadeOut( 1.0, 0.5, 1.0, 0.0 );
			/* 演出OnOff */
			if ( !Production ) {
				bright->setMode( FadeInFadeOut::END );
			}

			/** 選択モード取得 */
			mModeName.push_back(CommonStr::Title::StartModeStr );
			mModeName.push_back(CommonStr::Title::OptionModeStr);
			mModeName.push_back(CommonStr::Title::EndModeStr   );
	}

	Title::~Title(){
		SAFE_DELETE( bright );
		ASSERT(DeleteGraph(mTitleLogoHandle  ) + 1);
		ASSERT(DeleteGraph(mBackGroundHandle ) + 1);
		ASSERT(DeleteGraph(mStandImageHandle ) + 1);
	}

	Base* Title::update( Parent* parent ){
		Base* next = this;

		/** タイトル描画フラグ */
		static bool drawFlag = false;
		/** bgm再生開始処理フラグ */
		static bool bgmStartFlag = false;

		if ( drawFlag ) {
			SetDrawBright(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);
			draw();
		}

		/** タイトル演出 */
		bright->UpdateParameter();
		switch( bright->getMode() ) {
		/** 徐々に白く */
		case FadeInFadeOut::FADE_IN:
		case FadeInFadeOut::WAIT_IN:
			SetDrawBright( bright->getParameter(), bright->getParameter(), bright->getParameter() );
			DrawBox( 0, 0, windowSizeX, windowSizeY, colorWhite,TRUE );
			break;
		/* 晴れてタイトル画面が現れる感じで */
		case FadeInFadeOut::FADE_OUT:
		case FadeInFadeOut::WAIT_OUT:
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,bright->getParameter());
			DrawBox( 0,0,windowSizeX, windowSizeY, colorWhite,TRUE );
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );
			drawFlag = true;
			if ( !bgmStartFlag ) {
				ASSERT( PlaySoundMem( parent->getBGMHandle(), DX_PLAYTYPE_LOOP, FALSE ) + 1 );
				bgmStartFlag = true;
			}

			break;
		case FadeInFadeOut::END:
			mInputPrompt(&next);
			break;
		default:
			break;
		}
		/* 演出飛ばし */
		if ( bright->getMode() != FadeInFadeOut::END ) {
			if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ){
				ASSERT( PlaySoundMem( parent->getBGMHandle(), DX_PLAYTYPE_LOOP, FALSE ) + 1 );
				drawFlag = true;
				bgmStartFlag = true;
				bright->setMode( FadeInFadeOut::END );
			}
		}
		return next;
	}

	/** タイトル描画 */
	void Title::draw() const{
		
		/** 背景描画 */
		DrawGraph(0,0,mBackGroundHandle,FALSE);
		/** タイトルロゴ表示 */
		DrawGraph( LogoImgPosX, LogoImgPosY, mTitleLogoHandle, TRUE);
		/** 立ち絵表示 */
		DrawGraph( StandImgPosX, StandImgPosY, mStandImageHandle, TRUE);
		/** 選択フレーム表示 */
		commonFunc->drawMultiSelectFrame(
			SelectPosX,
			SelectPosY,
			0,
			drawInterval,
			mModeName,
			true);
		/** マウスポインタ表示 */
		commonFunc->drawMousePointer();
	}

	/* 今どのモードを選択しているかをマウスポインタの位置から取得 */
	Title::NextMode Title::mGetSelectWhichMode() const{
		NextMode selectMode = NONE;

		for ( unsigned int i = 0; i < mModeName.size(); i++) {
			/** フレーム表示位置取得 */
			int framePosX = SelectPosX;
			int framePosY = i * drawInterval + SelectPosY;
			/** 判定部分 */
			if ( commonFunc->isMousePointOnFrame( framePosX,framePosY ) ) {
				selectMode = static_cast<NextMode>( i );
				break;
			}
		}
		return selectMode;
	}

	/** 入力受付部分 */
	void Title::mInputPrompt(Base** nextMode){

		/** 左クリック入力状態なら遷移判定 */
		if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
			switch( commonFunc->getSelectFrame() ) {
			case SELECT:
				ASSERT( PlaySoundMem( Parent::instance()->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				*nextMode = new StageSelect;
				break;
			case OPTION:
				ASSERT( PlaySoundMem( Parent::instance()->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				*nextMode = new Option;
				break;
			case END:
				/** ゲーム終了フラグを立てる */
				Parent::isGameEsc = true;
				break;
			case NONE:
				/** 何もしない */
				break;
			default:
				/** 何もしない */
				break;
			}
		}
	}


} //namespace Sequence