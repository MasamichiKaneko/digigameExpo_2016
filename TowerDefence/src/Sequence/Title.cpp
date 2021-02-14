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

	/** �`��ʒu */
	const int Title::StandImgPosX = 30;
	const int Title::StandImgPosY = 120;

	const int Title::LogoImgPosX = 30;
	const int Title::LogoImgPosY = 10;

	const int Title::SelectPosX = 260;
	const int Title::SelectPosY = 260;

	/* ���j���[�`��Ԋu��` */
	const int Title::drawInterval = 65;

	const unsigned int colorWhite = GetColor( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );

	Title::Title(bool Production)
		: mBackGroundHandle( 0 ),
		mTitleLogoHandle( 0 ),
		mStandImageHandle( 0 ),
		mTime( 0 ),bright( 0 ),
		commonFunc( 0 ){

			/** ���ʏ����N���X�C���X�^���X���Q�Ƃ����� */
			commonFunc = CommonFunc::getInstance();
			/* �摜�ǂݍ��� */
			mBackGroundHandle = LoadGraph(FileNames::Image::TitleBack.c_str());
			mTitleLogoHandle  = LoadGraph(FileNames::Image::TitleLogo.c_str());

			/* �Ǎ��݃G���[�`�F�b�N */
			ASSERT( mTitleLogoHandle  + 1);
			ASSERT( mBackGroundHandle + 1);

			/** �E�B���h�E�T�C�Y�擾 */
			ASSERT( GetWindowSize( &windowSizeX, &windowSizeY ) + 1);

			/** �^�C�g���ł̓}�E�X�J�[�\���͔�\�� */
			ASSERT( SetMouseDispFlag( FALSE ) + 1 );

			/* �Ǎ��ݗ����G�𗐐��Ō��߂� */
			int StandImageNum = GetRand( static_cast<int>(FileNames::Image::StandImagesVector.size()) - 1 );
			mStandImageHandle = LoadGraph( FileNames::Image::StandImagesVector.at(StandImageNum).c_str() );
			ASSERT( mStandImageHandle + 1);

			/* �^�C�g�����o */
			bright = new FadeInFadeOut( 1.0, 0.5, 1.0, 0.0 );
			/* ���oOnOff */
			if ( !Production ) {
				bright->setMode( FadeInFadeOut::END );
			}

			/** �I�����[�h�擾 */
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

		/** �^�C�g���`��t���O */
		static bool drawFlag = false;
		/** bgm�Đ��J�n�����t���O */
		static bool bgmStartFlag = false;

		if ( drawFlag ) {
			SetDrawBright(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);
			draw();
		}

		/** �^�C�g�����o */
		bright->UpdateParameter();
		switch( bright->getMode() ) {
		/** ���X�ɔ��� */
		case FadeInFadeOut::FADE_IN:
		case FadeInFadeOut::WAIT_IN:
			SetDrawBright( bright->getParameter(), bright->getParameter(), bright->getParameter() );
			DrawBox( 0, 0, windowSizeX, windowSizeY, colorWhite,TRUE );
			break;
		/* ����ă^�C�g����ʂ�����銴���� */
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
		/* ���o��΂� */
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

	/** �^�C�g���`�� */
	void Title::draw() const{
		
		/** �w�i�`�� */
		DrawGraph(0,0,mBackGroundHandle,FALSE);
		/** �^�C�g�����S�\�� */
		DrawGraph( LogoImgPosX, LogoImgPosY, mTitleLogoHandle, TRUE);
		/** �����G�\�� */
		DrawGraph( StandImgPosX, StandImgPosY, mStandImageHandle, TRUE);
		/** �I���t���[���\�� */
		commonFunc->drawMultiSelectFrame(
			SelectPosX,
			SelectPosY,
			0,
			drawInterval,
			mModeName,
			true);
		/** �}�E�X�|�C���^�\�� */
		commonFunc->drawMousePointer();
	}

	/* ���ǂ̃��[�h��I�����Ă��邩���}�E�X�|�C���^�̈ʒu����擾 */
	Title::NextMode Title::mGetSelectWhichMode() const{
		NextMode selectMode = NONE;

		for ( unsigned int i = 0; i < mModeName.size(); i++) {
			/** �t���[���\���ʒu�擾 */
			int framePosX = SelectPosX;
			int framePosY = i * drawInterval + SelectPosY;
			/** ���蕔�� */
			if ( commonFunc->isMousePointOnFrame( framePosX,framePosY ) ) {
				selectMode = static_cast<NextMode>( i );
				break;
			}
		}
		return selectMode;
	}

	/** ���͎�t���� */
	void Title::mInputPrompt(Base** nextMode){

		/** ���N���b�N���͏�ԂȂ�J�ڔ��� */
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
				/** �Q�[���I���t���O�𗧂Ă� */
				Parent::isGameEsc = true;
				break;
			case NONE:
				/** �������Ȃ� */
				break;
			default:
				/** �������Ȃ� */
				break;
			}
		}
	}


} //namespace Sequence