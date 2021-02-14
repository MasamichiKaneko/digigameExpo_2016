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

	/** �`��ʒu */
	const int StageSelect::SelectPosX = 70;
	const int StageSelect:: SelectPosY = 420;

	/** �X�e�[�W�A�C�R���T�C�Y */
	const int StageSelect::StageIconSizeX = 60;
	const int StageSelect::StageIconSizeY = 60;

	/** �X�e�[�W�A�C�R���`��J�n�ʒu */
	const int StageSelect::StageIconPosX = 80;
	const int StageSelect::StageIconPosY = 30;

	/** �X�e�[�W�A�C�R���`��Ԋu */
	const int StageSelect::drawIntervalIconX = 140;
	const int StageSelect::drawIntervalIconY = 100;

	/** �A�C�R���z�u�� */
	const int StageSelect::StageIconRow = 4;
	const int StageSelect::StageIconColumn = 2;

	/** �X�R�A�\���ʒu��` */
	const int StageSelect::ShowScoreFramePosX = 60;
	const int StageSelect::ShowScoreFramePosY = 260;

	const int StageSelect::ShowScoreStrPosX = ShowScoreFramePosX + 100;
	const int StageSelect::ShowScoreStrPosY = ShowScoreFramePosY + 40;

	const int StageSelect::ShowFirstScorePosX = ShowScoreFramePosX + 200;
	const int StageSelect::ShowFirstScorePosY = ShowScoreFramePosY + 40;

	const int StageSelect::ShowScoreInterval = 25;

	const int StageSelect::ShowStageNumPosX  = ShowScoreFramePosX + 100;
	const int StageSelect::ShowStageNumPosY  = ShowScoreFramePosY + 15;

	/** �t�H���g�v���p�e�B */
	const int StageSelect::FontSize = 20;
	const int StageSelect::FontColor = GetColor( 0, 0, 0 );  //black
	const char* StageSelect::FontStyle = "HGS�s����";

	/** �X�R�A�\������ */
	const int StageSelect::ShowScoreDigit = 8;

	/** ���j���[�`��Ԋu��` */
	const int StageSelect::drawInterval = 190;

	/** �I���X�e�[�W */
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

		/** ��ʂ���̃|�C���^���Q�Ƃ����� */
		commonFunc = CommonFunc::getInstance();
		mScore = Parent::instance()->getScore();

		/* �摜�Ǎ��� */
		mBackGroundHandle   = LoadGraph( FileNames::Image::StageSelectBack.c_str() );
		mLockImgHandle      = LoadGraph( FileNames::Image::StageLock.c_str() );
		mCursorImgHandle    = LoadGraph( FileNames::Image::StageSelectCursor.c_str() );
		mShowScoreImgHandle = LoadGraph( FileNames::Image::ShowScoreArea.c_str() );

		/** �t�H���g�n���h���쐬 */
		mFontHandle = CreateFontToHandle( FontStyle, FontSize, -1,  DX_FONTTYPE_ANTIALIASING );
		ASSERT( mFontHandle + 1 );

		/* �X�e�[�W�A�C�R���̂ݕ����Ǎ��� */
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

		/* �Ǎ��݃`�F�b�N */
		ASSERT(mBackGroundHandle   + 1);
		ASSERT(mLockImgHandle      + 1);
		ASSERT(mCursorImgHandle    + 1);
		ASSERT(mShowScoreImgHandle + 1);


		/** ���[�h�\�����擾 */
		mModeName.push_back( CommonStr::Select::StartModeStr );
		mModeName.push_back( CommonStr::Select::BuildModeStr );
		mModeName.push_back( CommonStr::Select::TitleModeStr );

		/* ���փX�e�[�W���Ǎ��� */
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
		/** �w�i�`�� */
		DrawGraph(0,0,mBackGroundHandle,FALSE);
		/** �X�e�[�W�A�C�R���`�� */
		mDrawStageIcon();
		/** �X�e�[�W�I���J�[�\���`�� */
		mDrawSelectCursor();
		/** TODO:�X�R�A�`�� */
		mDrawScore();

		/** ���[�h�I���t���[���`�� */
		commonFunc->drawMultiSelectFrame(
			SelectPosX,
			SelectPosY,
			drawInterval,
			0,
			mModeName,
			true);

		/** �J�[�\���`�� */
		commonFunc->drawMousePointer();
	}

	/** �X�R�A�`�� */
	void StageSelect::mDrawScore() const {
		
		/** �X�R�A�擾 */
		vector<int> scores;
		scores = mScore->getScore()->find(mSelection)->second;

		DrawGraph( ShowScoreFramePosX, ShowScoreFramePosY,mShowScoreImgHandle,TRUE );
		/** �I���X�e�[�W�ݒ�\�� */
		stringstream stageInfoStr;
		int initAsset = StageConfig::getInstance()->getConfigValue()->at( mSelection ).initAsset;
		int stdLevel  = StageConfig::getInstance()->getConfigValue()->at( mSelection ).stdLevelValue;
		
		stageInfoStr << CommonStr::Select::StageStr << mSelection + 1;
		stageInfoStr << "  " << CommonStr::Select::InitAssetStr << initAsset ;
		stageInfoStr << "  " << CommonStr::Select::StdLevelStr  << stdLevel;

		DrawStringToHandle( ShowStageNumPosX, ShowStageNumPosY, stageInfoStr.str().c_str(), FontColor, mFontHandle );
		
		/** �X�R�A�\������ */
		DrawStringToHandle( ShowScoreStrPosX, ShowScoreStrPosY, CommonStr::Select::ScoreStr.c_str(), FontColor, mFontHandle );
		for ( unsigned int i = 0;i < scores.size(); i++ ){
			/** ������ϊ� */
			string strScore = getZeroPadding( scores.at(i), ShowScoreDigit );
			stringstream drawStr;
			drawStr << i + 1 << ". " << strScore;
			DrawStringToHandle(ShowFirstScorePosX, ShowFirstScorePosY + i * ShowScoreInterval
				, drawStr.str().c_str(), FontColor , mFontHandle );
		}
	}

	/** ���͎�t */
	void StageSelect::mInputPrompt( Base** nextMode,Parent** parent ) {

		/** ���N���b�N���͏�ԂȂ�J�ڔ��� */
		if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
			switch( commonFunc->getSelectFrame() ) {
			case STAGE:
				/** �I�𒆃X�e�[�W�֑J�� */
				ASSERT( PlaySoundMem( Parent::instance()->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				ASSERT( StopSoundMem( (*parent)->getBGMHandle() ) + 1 );
				(*parent)->setMode(Parent::MODE_PLAY);
				*nextMode = new GameSequence::Parent( (*parent)->mode(),mSelection );
				break;
			/** �琬��ʂ� */
			case BUILD:
				ASSERT( PlaySoundMem( Parent::instance()->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				*nextMode = new CharaBuild();
				break;
			/** �^�C�g����ʂ� */
			case TITLE:
				ASSERT( PlaySoundMem( Parent::instance()->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				*nextMode = new Title(false);
				break;
			/** ��L�ȊO */
			default:
				/** �X�e�[�W�I�𔻒� */
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

	/** �X�e�[�W�A�C�R���`�� */
	void StageSelect::mDrawStageIcon() const{

		for ( int handleId = 0; handleId < CommonConst::TotalStageNum; handleId++) {
			int drawPosX = StageIconPosX + drawIntervalIconX * ( handleId % StageIconRow );
			int drawPosY = StageIconPosY + drawIntervalIconY * ( handleId / StageIconRow );
			int drawHandle = 0;

			/** �����փX�e�[�W�̓O���[�X�P�[���ňÂ��\�� */
			if ( mUnlockStageNum <= handleId ) {
				drawHandle = mLockStageIconHandles.at( handleId );
				/** ���`�� */
				SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
				int keySizeX,keySizeY;
				ASSERT( GetGraphSize(mLockImgHandle, &keySizeX, &keySizeY) + 1 );
				int keyPosX = drawPosX + ( ( StageIconSizeX - keySizeX ) / 2 );
				int keyPosY = drawPosY + ( StageIconSizeX + ( keySizeX / 2) );
				DrawGraph( keyPosX, keyPosY, mLockImgHandle, TRUE );
				SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2);
				/** ���փX�e�[�W�͑I�𒆂̏ꍇ�̂ݖ��邭 */
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

	/** �I���J�[�\���`�� */
	void StageSelect::mDrawSelectCursor() const {
		/** �`��ʒu�Z�o */
		int posX = StageIconPosX + ( ( mSelection % StageIconRow ) * drawIntervalIconX );
		int posY = StageIconPosY + ( ( mSelection / StageIconRow ) * drawIntervalIconY );
		
		/** �T�C�Y����␳�l������o�� */
		int sizeX, sizeY;
		ASSERT( GetGraphSize( mCursorImgHandle, &sizeX, &sizeY ) + 1 );
		posX -= (sizeX - StageIconSizeX) / 2;
		posY -= (sizeY - StageIconSizeY) / 2;

		DrawGraph( posX, posY, mCursorImgHandle, TRUE);
	}

}