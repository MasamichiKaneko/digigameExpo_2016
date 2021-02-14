#pragma once

/**
 * オプション画面
 */

#include <map>
#include <vector>
#include <string>
#include "Sequence\Child.h"

namespace Sequence{

	class CommonFunc;
	class Parent;

	class Option : public Child{
	public:
		Option();
		~Option();
		Base* update( Parent* );

	private:

		CommonFunc* commonFunc;

		typedef struct{
			int x, y;
		}pos;

		/** 入力ステータス */
		enum InputStatus{
			DECIDE,  //決定
			CANCEL,  //キャンセル
			NONE,    //無し
		};
		
		/** 描画関係 */
		void draw();
		void drawMeter( std::string aValueType );
		void drawPlusMinusIcon( std::string aValueType );
		void drawGridSample();

		/** 描画位置計算 */
		void culcDrawPos();
		
		/** 入力受付 */
		void mInputPrompt( Base** nextMode, Parent** parent ); 
		
		/** 設定を現在の設定値セット */
		void mSetConfigValue( std::string aValueType, Parent** parent );


		/** 画像ハンドル */
		int mBackGroundHandle;
		int mMeterHandle;
		int mDisplayBarHandle;
		int mPlusIconHandle;
		int mMinusIconHandle;
		int mMapChipSampleHandle;

		/** フォントハンドル */
		int mFontHandle;

		/** 設定値 */
		std::map< std::string, int > mSetValues;
		std::map< std::string, int > mMaxValues;

		/** 設定値テーブル */
		std::map< std::string, int > mValueTable;
		enum ValueTable{
			BGM,
			SE,
			Grid,
		};

		/** 設定値ごとの描画位置定義 */
		std::map< std::string, pos > mValueTypePos;
		std::map< std::string, pos > mValueRatioPos;
		std::map< std::string, pos > mMeterStartPos;
		std::map< std::string, pos > mMinusIconPos;
		std::map< std::string, pos > mPlusIconPos;

		/** グリッド濃度保持用 */
		int mGridDipth;

		/** 選択モード */
		std::vector< std::string > mModeName;

		/** 各種定数 */
		static const int DisplayBarDrawPosX;
		static const int DisplayBarDrawPosY;
		static const int DisplayBarIntervalX;
		static const int DisplayBarIntervalY;

		static const int SelectModePosX;
		static const int SelectModePosY;
		static const int SelectModeIntervalX;
		static const int SelectModeIntervalY;

		static const int PlusMinusIconSizeX;
		static const int PlusMinusIconSizeY;
		static const int PlusMinusIconNum;

		static const int GridDrawPosX;
		static const int GridDrawPosY;
		static const int DrawMapChipNum;
		static const int GridLineColor;

		static const int FontSize;
		static const int FontColor;

	};
}