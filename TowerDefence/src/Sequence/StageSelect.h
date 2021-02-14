#pragma once

#include "Sequence\Child.h"
#include <vector>
#include <string>



/**
 * �X�e�[�W�Z���N�g
 */
class Score;
namespace Sequence{
	
	class CommonFunc;
	class Parent;
	class StageSelect : public Child{
	public:
		/* TODO:�X�e�[�W�f�[�^����X�e�[�W����ǂݍ��� */
		StageSelect(/* const char* stageData */);
		~StageSelect();
		Base* update( Parent* );
		
	private:
		
		enum NextMode {
			STAGE,    //�I�𒆃X�e�[�W��
			BUILD,    //�琬��
			TITLE,    //�^�C�g����
			NONE,     //��������
		};

		/** ���͎�t���� */
		void mInputPrompt( Base** nextMode, Parent** parent );

		/** �S�̂̕`�� */
		void draw() const;

		/** �A�C�R���`�� */
		void mDrawStageIcon() const;

		/** �X�R�A�\�� */
		void mDrawScore() const;

		/** �I���J�[�\���`�� */
		void mDrawSelectCursor() const;

		/* �I�𒆃X�e�[�WNo.�ێ� */
		static int mSelection;

		/** �摜�n���h�� */
		int mBackGroundHandle;
		int mLockImgHandle;
		int mCursorImgHandle;
		int mShowScoreImgHandle;

		std::vector< int > mStageIconHandles;
		std::vector< int > mLockStageIconHandles;

		/** �t�H���g�n���h�� */
		int mFontHandle;

		/** ���փX�e�[�W�� */
		int mUnlockStageNum;

		Score* mScore;

		/** ���[�h�\���� */
		std::vector< std::string > mModeName;

		/* parent�Ɏ������邩? */
		CommonFunc* commonFunc;
		
		/** �萔��` */

		/** �`��ʒu */
		/** �I���t���[���`��J�n�ʒu */
		static const int SelectPosX;
		static const int SelectPosY;

		/** �X�e�[�W�A�C�R���T�C�Y */
		static const int StageIconSizeX;
		static const int StageIconSizeY;

		/** �X�e�[�W�A�C�R���`��J�n�ʒu */
		static const int StageIconPosX;
		static const int StageIconPosY;

		/** �X�e�[�W�A�C�R���`��Ԋu */
		static const int drawIntervalIconX;
		static const int drawIntervalIconY;

		/** �A�C�R���z�u�� */
		static const int StageIconRow;
		static const int StageIconColumn;

		/** �X�R�A�\���ʒu��` */
		static const int ShowScoreFramePosX;
		static const int ShowScoreFramePosY;

		static const int ShowFirstScorePosX;
		static const int ShowFirstScorePosY;

		static const int ShowScoreStrPosX;
		static const int ShowScoreStrPosY;

		static const int ShowScoreInterval;

		static const int ShowStageNumPosX;
		static const int ShowStageNumPosY;

		/** �t�H���g�v���p�e�B��` */
		static const int FontSize;
		static const int FontColor;
		static const char* FontStyle;

		/** �X�R�A�\������ */
		static const int ShowScoreDigit;

		/** ���j���[�`��Ԋu��` */
		static const int drawInterval;

	};

}