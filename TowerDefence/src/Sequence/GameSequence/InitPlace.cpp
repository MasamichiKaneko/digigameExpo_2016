/** 初期配置画面 */

#include "Sequence\GameSequence\InitPlace.h"
#include "DxLib.h"
#include "utils.h"
#include "Sequence\GameSequence\Parent.h"
#include "Game\State.h"
#include "CommonString.h"
#include "Sequence\CommonFunc.h"
#include "define.h"
#include "Sequence\GameSequence\Play.h"
#include "FileNames.h"
#include "CommonConst.h"

namespace Sequence{
	namespace GameSequence{

		const int InitPlace::StartButtonPosX = 500;
		const int InitPlace::StartButtonPosY = 375;

		InitPlace::InitPlace( Parent* parent ) : mDecisionSEHandle( 0 ) {
			/** BGM再生 */
			int bgmHandle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::StageBGMName );
			ASSERT( PlaySoundMem( bgmHandle, DX_PLAYTYPE_LOOP,FALSE ) + 1 );
			mDecisionSEHandle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::DecisionSEName  );
		}

		InitPlace::~InitPlace(){
		}

		Base* InitPlace::update( Parent* parent ){
			Base* next = this;

			/** 状態受け取り */
			Game::State* state = parent->state();
			
			/** 入力受付 */
			mInputPrompt( &next, &parent );

			/** 描画 */
			draw( state, true );

			return next;
		}

		void InitPlace::draw( Game::State* state, bool selectionDraw ) const{
			//GameMainBase::draw( state, selectionDraw);
			
			/** 全体描画 */
			state->draw( true );

			/** 開始ボタン描画 */
			if ( !CommonFunc::getInstance()->isMousePointOnFrame( StartButtonPosX, StartButtonPosY ) ) {
				SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
				CommonFunc::getInstance()->drawSelectFrame( StartButtonPosX, StartButtonPosY, CommonStr::Main::StartStr );
				SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
			} else {
				CommonFunc::getInstance()->drawSelectFrame( StartButtonPosX, StartButtonPosY, CommonStr::Main::StartStr );
			}
			/** マウスポインタ描画 */
			CommonFunc::getInstance()->drawMousePointer();
		}

		/** 入力受付 */
		void InitPlace::mInputPrompt(Base** nextMode, Parent** parent ){

			/** 状態受け取り */
			Game::State* state = (*parent)->state();
			/** マウス座標受け取り */
			int mousePosX, mousePosY;

			ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );

			/** 左クリック */
			if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
				/** ボタンチェック */
				if ( CommonFunc::getInstance()->isMousePointOnFrame( StartButtonPosX, StartButtonPosY ) ) {
					ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
					*nextMode = new Play( (*parent) );
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

	}
}