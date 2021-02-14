#include "DxLib.h"
#include "utils.h"
#include "Sequence\GameSequence\Play.h"
#include "Sequence\GameSequence\Parent.h"
#include "Game\State.h"
#include "Sequence\CommonFunc.h"
#include "Sequence\GameSequence\Pause.h"
#include "define.h"
#include "Sequence\GameSequence\GameOver.h"
#include "FileNames.h"
#include "CommonConst.h"
#include "CommonString.h"

namespace Sequence{
	namespace GameSequence{

		const int Play::PauseButtonPosX = 500;
		const int Play::PauseButtonPosY = 375;

		Play::Play( Parent* parent ) : mDecisionSEHandle( 0 ) {
			mDecisionSEHandle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::DecisionSEName  );
		}

		Play::~Play(){
		}

		Base* Play::update( Parent* parent ){
			Base* next = this;
			/** 状態受け取り */
			Game::State* state = parent->state();

			/** 入力受付 */
			mInputPrompt( &next, &parent );
			
			/** 更新 */
			state->update();

			/** 描画 */
			draw( parent, true );

			/** ゲーム終了判定 */
			if ( state->getGameOver() ) {
				/** ゲームオーバ画面へ */
				next = new GameOver( parent );
			}

			return next;
		}

		/** 入力受付 */
		void Play::mInputPrompt(Base** nextMode, Parent** parent ){

			/** 状態受け取り */
			Game::State* state = (*parent)->state();
			/** マウス座標受け取り */
			int mousePosX, mousePosY;

			ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );

			/** 左クリック */
			if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
				/** ポーズボタンチェック */
				if ( CommonFunc::getInstance()->isMousePointOnFrame( PauseButtonPosX , PauseButtonPosY ) ) {
					ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
					*nextMode = new Pause( *parent );
				}
				/** 上記でなければ状態入力チェック */
				(*parent)->state()->mInputPrompt( mousePosX, mousePosY );
			/** 右クリック */
			} else if ( inputMouseTriggered( MOUSE_INPUT_RIGHT ) ) {
				/** 選択解除 */
				(*parent)->state()->mShiftSelection();
				//(*parent)->state()->mSetCharaType( Game::State::Non );
			}
		}

		void Play::draw( Parent* parent ,bool selectionDraw ) const {
			/** 全体描画 */
			parent->state()->draw( selectionDraw );

			/** ポーズボタン描画 */
			if ( !CommonFunc::getInstance()->isMousePointOnFrame( PauseButtonPosX , PauseButtonPosY ) ) {
				SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
				CommonFunc::getInstance()->drawSelectFrame( PauseButtonPosX, PauseButtonPosY, CommonStr::Main::PauseStr );
				SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
			} else {
				CommonFunc::getInstance()->drawSelectFrame( PauseButtonPosX, PauseButtonPosY, CommonStr::Main::PauseStr );
			}

			/** マウスポインタ描画 */
			CommonFunc::getInstance()->drawMousePointer();
		}

	}
}