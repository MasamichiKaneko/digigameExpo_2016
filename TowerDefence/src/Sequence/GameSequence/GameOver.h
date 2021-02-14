/** �Q�[���I�[�o��� */
#pragma once

#include "Sequence\GameSequence\Child.h"
#include <vector>

class FadeInFadeOut;
class Score;

namespace Game{
	class State;
}

namespace Sequence{
	namespace GameSequence{
		class Parent;
		class GameOver : public Child{
		public:
			GameOver( Parent* parent );
			~GameOver();
			Base* update( Parent* );
		private:
			enum Result{
				Win  = 0,
				Lose = 1
			};
			FadeInFadeOut* bright;

			bool checkRankIn( int aScoreValue );
			void draw( Parent* ) const;
			bool mIsWin;
			/** �摜�n���h�� */
			int mResultLogoHandle;
			int mScoreBoardHandle;
			int mEndMessageImgHandle;

			/** ���艹�n���h�� */
			int mDecisionSEHandle;

			/** �t�H���g�n���h�� */
			int mFontHandle;

			/** �X�R�A�{�� */
			double mScoreRatio;

			/** ���v�z */
			int mEarningsValue;

			/** �g�[�^���X�R�A */
			int mTotalScore;

			/** �E�B���h�E�T�C�Y */
			int windowSizeX;
			int windowSizeY;

			/** �J�ڃt���O */
			bool isMoveNextScene;

			int mRoopCount;

			static const float mFadeInTime;
			
			static const int mAssetBonusRatio;

			/** �`��ʒu */
			static const int mLogoPosX;
			static const int mLogoPosY;
			
			static const int mScoreBoardX;
			static const int mScoreBoardY;

			static const int mScorePosX;
			static const int mScorePosY;

			static const int mAssetBonusPosX;
			static const int mAssetBonusPosY;

			static const int mLevelRatioPosX;
			static const int mLevelRatioPosY;

			static const int mTotalScorePosX;
			static const int mTotalScorePosY;

			static const int mMessagePosX;
			static const int mMessagePosY;

			/** �t�H���g�v���p�e�B��` */
			static const int FontSize;
			static const char* FontStyle;
			
			/** �F��` */
			static const int colorBlack;
			static const int colorWhite;
		};
	}
}