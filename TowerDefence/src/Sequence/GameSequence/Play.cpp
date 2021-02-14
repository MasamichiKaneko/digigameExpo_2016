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
			/** ��Ԏ󂯎�� */
			Game::State* state = parent->state();

			/** ���͎�t */
			mInputPrompt( &next, &parent );
			
			/** �X�V */
			state->update();

			/** �`�� */
			draw( parent, true );

			/** �Q�[���I������ */
			if ( state->getGameOver() ) {
				/** �Q�[���I�[�o��ʂ� */
				next = new GameOver( parent );
			}

			return next;
		}

		/** ���͎�t */
		void Play::mInputPrompt(Base** nextMode, Parent** parent ){

			/** ��Ԏ󂯎�� */
			Game::State* state = (*parent)->state();
			/** �}�E�X���W�󂯎�� */
			int mousePosX, mousePosY;

			ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );

			/** ���N���b�N */
			if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
				/** �|�[�Y�{�^���`�F�b�N */
				if ( CommonFunc::getInstance()->isMousePointOnFrame( PauseButtonPosX , PauseButtonPosY ) ) {
					ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
					*nextMode = new Pause( *parent );
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

		void Play::draw( Parent* parent ,bool selectionDraw ) const {
			/** �S�̕`�� */
			parent->state()->draw( selectionDraw );

			/** �|�[�Y�{�^���`�� */
			if ( !CommonFunc::getInstance()->isMousePointOnFrame( PauseButtonPosX , PauseButtonPosY ) ) {
				SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
				CommonFunc::getInstance()->drawSelectFrame( PauseButtonPosX, PauseButtonPosY, CommonStr::Main::PauseStr );
				SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
			} else {
				CommonFunc::getInstance()->drawSelectFrame( PauseButtonPosX, PauseButtonPosY, CommonStr::Main::PauseStr );
			}

			/** �}�E�X�|�C���^�`�� */
			CommonFunc::getInstance()->drawMousePointer();
		}

	}
}