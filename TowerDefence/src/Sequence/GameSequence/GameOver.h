/** ゲームオーバ画面 */
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
			/** 画像ハンドル */
			int mResultLogoHandle;
			int mScoreBoardHandle;
			int mEndMessageImgHandle;

			/** 決定音ハンドル */
			int mDecisionSEHandle;

			/** フォントハンドル */
			int mFontHandle;

			/** スコア倍率 */
			double mScoreRatio;

			/** 利益額 */
			int mEarningsValue;

			/** トータルスコア */
			int mTotalScore;

			/** ウィンドウサイズ */
			int windowSizeX;
			int windowSizeY;

			/** 遷移フラグ */
			bool isMoveNextScene;

			int mRoopCount;

			static const float mFadeInTime;
			
			static const int mAssetBonusRatio;

			/** 描画位置 */
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

			/** フォントプロパティ定義 */
			static const int FontSize;
			static const char* FontStyle;
			
			/** 色定義 */
			static const int colorBlack;
			static const int colorWhite;
		};
	}
}