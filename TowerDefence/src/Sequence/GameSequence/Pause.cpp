/** 初期配置画面 */

#include "Sequence\GameSequence\Pause.h"
#include "DxLib.h"
#include "utils.h"
#include "Sequence\GameSequence\Parent.h"
#include "Sequence\StageSelect.h"
#include "Game\State.h"
#include "Sequence\CommonFunc.h"
#include "define.h"
#include "Sequence\GameSequence\Play.h"
#include "FileNames.h"
#include "CommonConst.h"
#include "CommonString.h"

namespace Sequence{
	namespace GameSequence{

		const int Pause::ContinueButtonPosX = 500;
		const int Pause::ContinueButtonPosY = 375;

		const int Pause::PlayStopButtonPosX = 500;
		const int Pause::PlayStopButtonPosY = 430;

		const int Pause::ConfirmBackPosX = 50;
		const int Pause::ConfirmBackPosY = 150;

		const int Pause::YesButtonPosX = ConfirmBackPosX + 40;
		const int Pause::NoButtonPosX  = YesButtonPosX + 180;

		const int Pause::YesButtonPosY = ConfirmBackPosY + 90;
		const int Pause::NoButtonPosY  = YesButtonPosY;

		const int colorBlack = GetColor( 0, 0, 0 );

		Pause::Pause( Parent* parent ) : isSelectGameEnd( false ) , isGameEnd( false ) {
			bright = new FadeInFadeOut( 1.0 , 0.0, 0.0, 0.0 );
			/** ハンドル取得 */
			mBGMHandle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::StageBGMName );
			mDecisionSEHandle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::DecisionSEName );
			mConfirmBackHandle = parent->loader()->getImageFileHandle()->at( FileNames::Image::ConfirmBackImage );
			
			/** bgmを止める */
			ASSERT( StopSoundMem( mBGMHandle ) + 1 );
			
			/** ウィンドウサイズ取得 */
			ASSERT( GetWindowSize( &windowSizeX, &windowSizeY ) + 1);

		}

		Pause::~Pause(){
			SAFE_DELETE( bright );
		}

		Base* Pause::update( Parent* parent ){
			Base* next = this;
			
			if ( isGameEnd ) {
				/** フェードイン更新 */
				bright->UpdateParameter();
				switch ( bright->getMode() ) {
				case FadeInFadeOut::FADE_IN:
					/** なにもしない */
					break;
				case FadeInFadeOut::WAIT_IN:
				case FadeInFadeOut::FADE_OUT:
				case FadeInFadeOut::WAIT_OUT:
				default:
					next = new StageSelect;
					break;
				}
			} else {
				/** 入力受付 */
				mInputPrompt( &next, &parent );
			}
			/** 描画 */
			draw( parent, false );
			
			return next;
		}

		void Pause::draw( Parent* parent, bool selectionDraw ) const{
			parent->state()->draw( selectionDraw );

			/** ボタン描画 */
			if ( !CommonFunc::getInstance()->isMousePointOnFrame( ContinueButtonPosX, ContinueButtonPosY ) && !isSelectGameEnd ) {
				SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
				CommonFunc::getInstance()->drawSelectFrame( ContinueButtonPosX, ContinueButtonPosY, CommonStr::Main::ContinueStr );
				SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
			} else {
				CommonFunc::getInstance()->drawSelectFrame( ContinueButtonPosX, ContinueButtonPosY, CommonStr::Main::ContinueStr );
			}

			if ( !CommonFunc::getInstance()->isMousePointOnFrame( PlayStopButtonPosX, PlayStopButtonPosY ) && !isSelectGameEnd ) {
				SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
				CommonFunc::getInstance()->drawSelectFrame( PlayStopButtonPosX, PlayStopButtonPosY, CommonStr::Main::EndStr );
				SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
			} else {
				CommonFunc::getInstance()->drawSelectFrame( PlayStopButtonPosX, PlayStopButtonPosY, CommonStr::Main::EndStr );
			}

			if ( isSelectGameEnd && !isGameEnd ) {
				/** 暗転 */
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, UCHAR_MAX / 2 );
				DrawBox( 0, 0, windowSizeX, windowSizeY, colorBlack, TRUE );
				SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );

				/** 確認バック表示 */
				DrawGraph( ConfirmBackPosX, ConfirmBackPosY, mConfirmBackHandle, TRUE );

				/** ボタン表示 */
				if ( !CommonFunc::getInstance()->isMousePointOnFrame( YesButtonPosX, YesButtonPosY ) && isSelectGameEnd ) {
					SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
					CommonFunc::getInstance()->drawSelectFrame( YesButtonPosX, YesButtonPosY, CommonStr::Main::YesStr );
					SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
				} else {
					CommonFunc::getInstance()->drawSelectFrame( YesButtonPosX, YesButtonPosY, CommonStr::Main::YesStr );
				}
				if ( !CommonFunc::getInstance()->isMousePointOnFrame( NoButtonPosX, NoButtonPosY ) && isSelectGameEnd ) {
					SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
					CommonFunc::getInstance()->drawSelectFrame( NoButtonPosX, NoButtonPosY, CommonStr::Main::NoStr );
					SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
				} else {
					CommonFunc::getInstance()->drawSelectFrame( NoButtonPosX, NoButtonPosY, CommonStr::Main::NoStr );
				}
			}

			/** マウスポインタ描画 */
			CommonFunc::getInstance()->drawMousePointer();

			/** 終了選択なら全体を徐々に暗転 */
			if ( isGameEnd ) {
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, bright->getParameter() );
				DrawBox( 0, 0, windowSizeX, windowSizeY, colorBlack,TRUE );
				SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );
			}
		}

		/** 入力受付 */
		void Pause::mInputPrompt(Base** nextMode, Parent** parent ){

			/** 状態受け取り */
			Game::State* state = (*parent)->state();
			/** マウス座標受け取り */
			int mousePosX, mousePosY;

			ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );

			/** 左クリック */
			if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) && !isGameEnd ) {
				/** ボタンチェック */
				if ( !isSelectGameEnd ) {
					/** 続けるボタン */
					if ( CommonFunc::getInstance()->isMousePointOnFrame( ContinueButtonPosX, ContinueButtonPosY ) ) {
						ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
						ASSERT( PlaySoundMem( mBGMHandle, DX_PLAYTYPE_LOOP,FALSE ) + 1 );
						*nextMode = new Play( (*parent) );
					/** やめるボタン */
					} else if ( CommonFunc::getInstance()->isMousePointOnFrame( PlayStopButtonPosX, PlayStopButtonPosY ) ) {
						ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
						isSelectGameEnd = true;
					}
				/** 確認ボタン */
				} else {
					if ( CommonFunc::getInstance()->isMousePointOnFrame( YesButtonPosX, YesButtonPosY ) ) {
						ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
						isGameEnd = true;
					} else if ( CommonFunc::getInstance()->isMousePointOnFrame( NoButtonPosX, NoButtonPosY ) ) {
						ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
						isSelectGameEnd = false;
					}
				}
			} 
		}
	}
}