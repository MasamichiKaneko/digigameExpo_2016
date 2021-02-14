#pragma once


class Score;
namespace Sequence{
	class Child;

	class Parent{
	public:

		/* ゲームモード */
		/* オプションとかスコア表示とか */
		/* ゲーム内容によっては1Pとか2Pとか */
		enum Mode{
			MODE_PLAY,    //ゲーム本編

			MODE_NONE,    //何もなし(遷移エラー検出用)
		};
		void update();
		Mode mode() const;
		void setMode( Mode );

		static void create();
		static void destroy();
		static Parent* instance();
		Score* getScore(){ return mScore; }
		int getBGMHandle(){ return mBGMHandle; }
		int getDecisionHandle(){ return mDecisionSEHandle; }

		/** ゲーム終了フラグ */
		/** trueに変更されたらプログラムを閉じる */
		static bool isGameEsc;

	private:
		Parent();
		~Parent();

		Mode mMode;

		int mBGMHandle;
		int mDecisionSEHandle;

		Child* mChild;
		Score* mScore;
		static Parent* mInstance;
	};

} //namespace Sequence