/** �����z�u��� */

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
			/** �n���h���擾 */
			mBGMHandle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::StageBGMName );
			mDecisionSEHandle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::DecisionSEName );
			mConfirmBackHandle = parent->loader()->getImageFileHandle()->at( FileNames::Image::ConfirmBackImage );
			
			/** bgm���~�߂� */
			ASSERT( StopSoundMem( mBGMHandle ) + 1 );
			
			/** �E�B���h�E�T�C�Y�擾 */
			ASSERT( GetWindowSize( &windowSizeX, &windowSizeY ) + 1);

		}

		Pause::~Pause(){
			SAFE_DELETE( bright );
		}

		Base* Pause::update( Parent* parent ){
			Base* next = this;
			
			if ( isGameEnd ) {
				/** �t�F�[�h�C���X�V */
				bright->UpdateParameter();
				switch ( bright->getMode() ) {
				case FadeInFadeOut::FADE_IN:
					/** �Ȃɂ����Ȃ� */
					break;
				case FadeInFadeOut::WAIT_IN:
				case FadeInFadeOut::FADE_OUT:
				case FadeInFadeOut::WAIT_OUT:
				default:
					next = new StageSelect;
					break;
				}
			} else {
				/** ���͎�t */
				mInputPrompt( &next, &parent );
			}
			/** �`�� */
			draw( parent, false );
			
			return next;
		}

		void Pause::draw( Parent* parent, bool selectionDraw ) const{
			parent->state()->draw( selectionDraw );

			/** �{�^���`�� */
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
				/** �Ó] */
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, UCHAR_MAX / 2 );
				DrawBox( 0, 0, windowSizeX, windowSizeY, colorBlack, TRUE );
				SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );

				/** �m�F�o�b�N�\�� */
				DrawGraph( ConfirmBackPosX, ConfirmBackPosY, mConfirmBackHandle, TRUE );

				/** �{�^���\�� */
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

			/** �}�E�X�|�C���^�`�� */
			CommonFunc::getInstance()->drawMousePointer();

			/** �I���I���Ȃ�S�̂����X�ɈÓ] */
			if ( isGameEnd ) {
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, bright->getParameter() );
				DrawBox( 0, 0, windowSizeX, windowSizeY, colorBlack,TRUE );
				SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );
			}
		}

		/** ���͎�t */
		void Pause::mInputPrompt(Base** nextMode, Parent** parent ){

			/** ��Ԏ󂯎�� */
			Game::State* state = (*parent)->state();
			/** �}�E�X���W�󂯎�� */
			int mousePosX, mousePosY;

			ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );

			/** ���N���b�N */
			if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) && !isGameEnd ) {
				/** �{�^���`�F�b�N */
				if ( !isSelectGameEnd ) {
					/** ������{�^�� */
					if ( CommonFunc::getInstance()->isMousePointOnFrame( ContinueButtonPosX, ContinueButtonPosY ) ) {
						ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
						ASSERT( PlaySoundMem( mBGMHandle, DX_PLAYTYPE_LOOP,FALSE ) + 1 );
						*nextMode = new Play( (*parent) );
					/** ��߂�{�^�� */
					} else if ( CommonFunc::getInstance()->isMousePointOnFrame( PlayStopButtonPosX, PlayStopButtonPosY ) ) {
						ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
						isSelectGameEnd = true;
					}
				/** �m�F�{�^�� */
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