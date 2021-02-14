/** �����z�u��� */

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
			/** BGM�Đ� */
			int bgmHandle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::StageBGMName );
			ASSERT( PlaySoundMem( bgmHandle, DX_PLAYTYPE_LOOP,FALSE ) + 1 );
			mDecisionSEHandle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::DecisionSEName  );
		}

		InitPlace::~InitPlace(){
		}

		Base* InitPlace::update( Parent* parent ){
			Base* next = this;

			/** ��Ԏ󂯎�� */
			Game::State* state = parent->state();
			
			/** ���͎�t */
			mInputPrompt( &next, &parent );

			/** �`�� */
			draw( state, true );

			return next;
		}

		void InitPlace::draw( Game::State* state, bool selectionDraw ) const{
			//GameMainBase::draw( state, selectionDraw);
			
			/** �S�̕`�� */
			state->draw( true );

			/** �J�n�{�^���`�� */
			if ( !CommonFunc::getInstance()->isMousePointOnFrame( StartButtonPosX, StartButtonPosY ) ) {
				SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
				CommonFunc::getInstance()->drawSelectFrame( StartButtonPosX, StartButtonPosY, CommonStr::Main::StartStr );
				SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
			} else {
				CommonFunc::getInstance()->drawSelectFrame( StartButtonPosX, StartButtonPosY, CommonStr::Main::StartStr );
			}
			/** �}�E�X�|�C���^�`�� */
			CommonFunc::getInstance()->drawMousePointer();
		}

		/** ���͎�t */
		void InitPlace::mInputPrompt(Base** nextMode, Parent** parent ){

			/** ��Ԏ󂯎�� */
			Game::State* state = (*parent)->state();
			/** �}�E�X���W�󂯎�� */
			int mousePosX, mousePosY;

			ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );

			/** ���N���b�N */
			if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
				/** �{�^���`�F�b�N */
				if ( CommonFunc::getInstance()->isMousePointOnFrame( StartButtonPosX, StartButtonPosY ) ) {
					ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
					*nextMode = new Play( (*parent) );
				}
				/** ��L�łȂ���Ώ�ԓ��̓`�F�b�N */
				(*parent)->state()->mInputPrompt( mousePosX, mousePosY );
			/** �E�N���b�N */
			} else if ( inputMouseTriggered( MOUSE_INPUT_RIGHT ) ) {
				/** �I������ */
				(*parent)->state()->mShiftSelection();
				//(*parent)->state()->mSetCharaType( Game::State::Non );
			}
		}

	}
}