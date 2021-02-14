#pragma once

#include "Sequence\Child.h"
#include <vector>
#include <string>



/**
 * ステージセレクト
 */
class Score;
namespace Sequence{
	
	class CommonFunc;
	class Parent;
	class StageSelect : public Child{
	public:
		/* TODO:ステージデータからステージ数を読み込む */
		StageSelect(/* const char* stageData */);
		~StageSelect();
		Base* update( Parent* );
		
	private:
		
		enum NextMode {
			STAGE,    //選択中ステージへ
			BUILD,    //育成へ
			TITLE,    //タイトルへ
			NONE,     //何も無し
		};

		/** 入力受付部分 */
		void mInputPrompt( Base** nextMode, Parent** parent );

		/** 全体の描画 */
		void draw() const;

		/** アイコン描画 */
		void mDrawStageIcon() const;

		/** スコア表示 */
		void mDrawScore() const;

		/** 選択カーソル描画 */
		void mDrawSelectCursor() const;

		/* 選択中ステージNo.保持 */
		static int mSelection;

		/** 画像ハンドル */
		int mBackGroundHandle;
		int mLockImgHandle;
		int mCursorImgHandle;
		int mShowScoreImgHandle;

		std::vector< int > mStageIconHandles;
		std::vector< int > mLockStageIconHandles;

		/** フォントハンドル */
		int mFontHandle;

		/** 解禁ステージ数 */
		int mUnlockStageNum;

		Score* mScore;

		/** モード表示名 */
		std::vector< std::string > mModeName;

		/* parentに持たせるか? */
		CommonFunc* commonFunc;
		
		/** 定数定義 */

		/** 描画位置 */
		/** 選択フレーム描画開始位置 */
		static const int SelectPosX;
		static const int SelectPosY;

		/** ステージアイコンサイズ */
		static const int StageIconSizeX;
		static const int StageIconSizeY;

		/** ステージアイコン描画開始位置 */
		static const int StageIconPosX;
		static const int StageIconPosY;

		/** ステージアイコン描画間隔 */
		static const int drawIntervalIconX;
		static const int drawIntervalIconY;

		/** アイコン配置数 */
		static const int StageIconRow;
		static const int StageIconColumn;

		/** スコア表示位置定義 */
		static const int ShowScoreFramePosX;
		static const int ShowScoreFramePosY;

		static const int ShowFirstScorePosX;
		static const int ShowFirstScorePosY;

		static const int ShowScoreStrPosX;
		static const int ShowScoreStrPosY;

		static const int ShowScoreInterval;

		static const int ShowStageNumPosX;
		static const int ShowStageNumPosY;

		/** フォントプロパティ定義 */
		static const int FontSize;
		static const int FontColor;
		static const char* FontStyle;

		/** スコア表示桁数 */
		static const int ShowScoreDigit;

		/** メニュー描画間隔定義 */
		static const int drawInterval;

	};

}