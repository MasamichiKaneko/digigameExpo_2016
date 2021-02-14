#pragma once

#include "Sequence\Child.h"
#include <vector>
#include <string>
#include <map>

/**
 * 育成画面
 */

namespace Game{
	class CharaStatusReader;
}

namespace Sequence{

	class CommonFunc;
	class Parent;
	class CharaBuild : public Child{
	public:
		CharaBuild( /* const char* mCharaDataFileName*/ );
		~CharaBuild();
		Base* update( Parent* );
	private:

		typedef struct{
			int x, y;
		}pos;

		/** 描画位置計算 */
		void culcDrawPos();

		/** 各種描画 */
		void draw() const;
		void mDrawCharaIcon() const;
		void mDrawCharaStatus() const;

		/** 入力ステータス */
		enum InputStatus{
			DECIDE,  //決定
			CANCEL,  //キャンセル
			NONE,    //無し
		};

		/** 入力受付 */
		void mInputPrompt( Base** nextMode, Parent** parent );

		/** データ読み書き */
		void mReadData( const char* mCharaDataFileName );
		void mWriteData( const char* mCharaDataFileName );

		/** ステータス表示値取得 */
		std::string getDisplayStatusValue( std::string typeName, int value ) const;

		/** ステータス */
		Game::CharaStatusReader* mReadStatus;

		/** 変更値保持用 */
		/**      ↓キャラid  ↓各ステータス変更値 */
		std::map<int,       std::map<std::string, int> > mAssignValues;

		/** 画像ハンドル */
		int mBackGroundHandle;
		int mCursorHandle;
		int mStatusBackHandle;
		int mStatusBarHandle;
		int mMeterHandle;
		int mPlusIconHandle;
		int mMinusIconHandle;
		int mWritingBackHandle;

		std::vector< int > mPlusMinusIconHandles;
		std::vector< int > mStandImgHandles;
		std::vector< int > mCharaIconHandles;

		/** フォントハンドル */
		int mFontHandle;

		CommonFunc* commonFunc;

		std::vector< std::string > mModeName;

		/** 描画位置定義 */
		std::vector< pos > mCharaIconPos;
		std::vector<pos> mStatusBarPos;
		pos mPlusIconPos;
		pos mMinusIconPos;
		pos mMeterDrawPos;
		
		/** ステータス表示名 */
		std::map< std::string, std::string > mDisplayName;

		/** 書き込みフラグ */
		bool mWriteFlag;

		/** 変更前変更後レベル */
		int beforeLevel;
		int afterLevel;
		int maxLevel;

		/** 定数定義 */
		/** バー表示数 */
		static const int DrawBarNum;
		/** フォント関連 */
		static const int FontSize;
		static const int FontColor;
		static const char* FontStyle;

		/** キャラアイコン描画開始位置 */
		static const int CharaIconDrawX;
		static const int CharaIconDrawY;
		/** キャラアイコン描画間隔 */
		static const int CharaIconIntervalX;
		static const int CharaIconIntervalY;

		/** キャラ立ち絵描画位置 */
		static const int CharaStandDrawX;
		static const int CharaStandDrawY;

		/** 選択フレーム描画位置 */
		static const int SelectPosX;
		static const int SelectPosY;

		/** ステータス表示フレーム描画位置 */
		static const int CharaStatusBackPosX;
		static const int CharaStatusBackPosY;

		/** 選択フレーム描画間隔 */
		static const int SelectIntervalX;
		static const int SelectIntervalY;

		/** ステータスバー描画開始位置 */
		static const int StatusBarPosX;
		static const int StatusBarPosY;

		/** ステータス描画間隔 */
		static const int StatusBarIntervalX;
		static const int StatusBarIntervalY;

		/** プラマイアイコン */
		static const int PlusMinusIconSizeX;
		static const int PlusMinusIconSizeY;
		static const int PlusMinusIconNum;

		/** レベル表示位置 */
		static const int LevelPosX;
		static const int LevelPosY;

		/** sp表示位置 */
		static const int SpValuePosX;
		static const int SpValuePosY;

		/** ステータス表示位置 */
		static const int StatusDrawPosX;
		static const int StatusDrawIntervalX;

		/** 次のレベルまでのexp表示 */
		static const int ExpValueForNextLvPosX;
		static const int ExpValueForNextLvPosY;
		
	};
}