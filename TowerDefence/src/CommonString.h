#pragma once
#include <string>
#include <vector>
using namespace std;

/** 文字列置き場 */

namespace CommonStr{

	/** 共通 */
	namespace Common{
		static const string DecisionStr  = "決定";
		static const string CancelStr    = "キャンセル";
		static const string SecondStr    = "秒";
		static const string TroutStr     = "枡";
		static const string GameTitleStr = "狐とオコジョの銭戦争";
	};

	/** タイトル関連 */
	namespace Title{
		static const string StartModeStr  = "はじめる";
		static const string OptionModeStr = "オプション";
		static const string EndModeStr    = "おわる";
	};

	/** セレクト関連 */
	namespace Select{
		static const string StartModeStr = "はじめる";
		static const string BuildModeStr = "レベル設定";
		static const string TitleModeStr = "タイトルへ";
		static const string InitAssetStr = "初期資産:";
		static const string StdLevelStr  = "標準レベル:";
		static const string ScoreStr     = "スコア:";
		static const string StageStr     = "ステージ:";
	};

	/** ステータス関連 */
	namespace Status{
		static const string LvStr = "Lv ";
		static const string SpStr = "Sp ";
		static const string NeedExpStr = "次Lvまで";
		static const string NoStatusStr = "--";
		static const string PowerStr = "火力";
		static const string SpeedStr = "攻撃間隔";
		static const string RangeStr = "攻撃範囲";
	};

	/** 本編関連 */
	namespace Main{
		static const string StartStr      = "迎撃開始";
		static const string PauseStr      = "一時停止";
		static const string EndStr        = "やめる";
		static const string ContinueStr   = "つづける";
		static const string YesStr        = "はい";
		static const string NoStr         = "いいえ";
	};
};