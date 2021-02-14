#pragma once

#include "Sequence\Child.h"
#include <vector>
#include <string>

class FadeInFadeOut;


/**
 * タイトル
 */
namespace Sequence{
	class Parent;
	class CommonFunc;
	class Title : public Child{
	public:
		/**
		* [引数]
		* true :演出on
		* false:演出off
		*/
		Title(bool Production);
		~Title();
		Base* update( Parent* );
	private:

		enum NextMode{
			SELECT,
			OPTION,
			END,
			NONE,
		};
		
		/* モード表示名 */
		std::vector<std::string> mModeName;

		/* タイトル画面描画 */
		void draw() const;

		/* 選択フレーム描画 */
		void drawSelectFrame() const;

		/* 入力受付部分 */
		void mInputPrompt(Base** nextMode);

		/* 今どのモードを選択しているかをマウスポインタの位置から取得 */
		/** 思いついたら共通化するかも */
		NextMode mGetSelectWhichMode() const;

		/** ウィンドウサイズ */
		int windowSizeX;
		int windowSizeY;

		/* 画像ハンドル */
		int mBackGroundHandle;
		int mTitleLogoHandle;
		int mStandImageHandle;
		
		int mTime;
		static int mSelection;
		FadeInFadeOut* bright;
		CommonFunc* commonFunc;

		/** 定数定義 */
		/** 描画位置 */
		static const int StandImgPosX;
		static const int StandImgPosY;

		static const int LogoImgPosX;
		static const int LogoImgPosY;

		static const int SelectPosX;
		static const int SelectPosY;

		/* メニュー描画間隔定義 */
		static const int drawInterval;


		
	};

} //namespace Sequence
