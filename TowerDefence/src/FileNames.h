#pragma once

/**
 * ゲーム本編で使われるファイル名定数定義
 */

#include "Loader.h"
#include <string>

using namespace std;


/** パスワード定義 */
namespace PassWord{
	static const char* EncryptPass = "ocjocj";
	static const char* ArchivesPass = "ocjocj";
};


namespace FileNames{
	/* データ入れるフォルダのルートフォルダ名 */
	static const string DataFolderName = "data\\";

	/** 記録データ */
	/** アーカイブに絡められない変動データだからText空間に入れない */
	/** スコアデータ */
	static const string ScoreDataName = "score.dat";
	/* キャラクタデータ */
	static const string CharaStatus = "charaData.dat";
	
	namespace Text{

		/* テキストフォルダ入れるフォルダ名 */
		static const string TxtFolderName = DataFolderName + "txt\\";
		
		/* マップ情報 */
		static const string MapInfoFile[] = { 
			TxtFolderName + "mapData1.dat",
			TxtFolderName + "mapData2.dat",
			TxtFolderName + "mapData3.dat",
			TxtFolderName + "mapData4.dat",
			TxtFolderName + "mapData5.dat",
			TxtFolderName + "mapData6.dat",
			TxtFolderName + "mapData7.dat",
			TxtFolderName + "mapData8.dat"
		};

		static const vector<string> MapInfoFileVector(MapInfoFile, std::end( MapInfoFile ) );

		/* 敵の出現情報 */
		static const string EnemyAppearanceData[] = {
			TxtFolderName + "EnemyAppearanceData1.dat",
			TxtFolderName + "EnemyAppearanceData2.dat",
			TxtFolderName + "EnemyAppearanceData3.dat",
			TxtFolderName + "EnemyAppearanceData4.dat",
			TxtFolderName + "EnemyAppearanceData5.dat",
			TxtFolderName + "EnemyAppearanceData6.dat",
			TxtFolderName + "EnemyAppearanceData7.dat",
			TxtFolderName + "EnemyAppearanceData8.dat",
		};

		static const vector<string> EnemyAppearanceVector( EnemyAppearanceData, std::end( EnemyAppearanceData ) );

		/* 敵のステータス情報 */
		static const string EnemyStatusData  = TxtFolderName + "EnemyStatusData.dat";

		/* コンフィグ */
		static const string FixedConfig    = TxtFolderName + "FixedConfig.dat";
		static const string VariableConfig = "config.dat";

		/** ステージ設定 */
		static const string StageConfigFileName = TxtFolderName + "StageConfig.dat";

		/** 各記録データ初期状態 */
		static const string ScoreDataTemplateName  = TxtFolderName + "InitScore.dat";
		static const string CharaDataTemplateName  = TxtFolderName + "InitCharaData.dat";
		static const string ConfigDataTemplateName = TxtFolderName + "InitVariableConfig.dat";

		/** 経験値テーブル */
		static const string ExpTableDataName = TxtFolderName + "ExpTable.dat";
	}

	namespace Image{
		/* 画像入れるフォルダ名 */
		static const string ImgFolderName = DataFolderName + "";
		/* ロゴ */
		static const string Logo    =   ImgFolderName + "logo.png";
		/* タイトル背景 */
		static const string TitleBack = ImgFolderName + "title.png";
		/* オプション背景 */
		static const string OptionBack = ImgFolderName + "option.png";

		/* ロード中画面 */
		static const string LoadBack = ImgFolderName + "Loading.png";

		/** 書き込み中画面 */
		static const string WritingBack = ImgFolderName + "Writing.png";

		/* タイトルロゴ */
		static const string TitleLogo = ImgFolderName + "title_logo.png";
		/* モードセレクト用フレーム */
		static const string SelectFrame = ImgFolderName + "SelectFrame.png";
		/* マウスポインタ画像 */
		static const string MousePointer = ImgFolderName + "MousePointer.png";

		/* ステージセレクト背景 */
		static const string StageSelectBack = ImgFolderName + "stageSelect.png";
		/* ステージアイコン画像 */
		static const string StageIcon =     ImgFolderName + "StageIcon.png";
		static const string LockStageIcon = ImgFolderName + "LockStageIcon.png";
		/* ステージ選択用カーソル */
		static const string StageSelectCursor = ImgFolderName + "StageSelectCursor.png";
		/* スコア描画位置画像 */
		static const string ShowScoreArea = ImgFolderName + "showScoreArea.png";
		/* 未開放ステージキー画像 */
		static const string StageLock = ImgFolderName + "keyIcon.png";

		/* 育成画面背景 */
		static const string CharaBuildBack = ImgFolderName + "charaBuild.png";
		/* 育成画面カーソル */
		static const string CharaBuildCursor = ImgFolderName + "CharaBuildCursor.png";
		/** 育成ステータス表示バック */
		static const string CharaBuildStatusBack = ImgFolderName + "buildStatusBack.png";
		/** ステータス表示バー */
		static const string CharaBuildStatusBar = ImgFolderName + "statusBar.png";
		/** ±アイコン */
		static const string CharaBuildPlusMinusIcon = ImgFolderName + "plusminusIcon.png";
		/** メータ画像 */
		static const string CharaBuildMeter = ImgFolderName + "BuildMeter.png";
		/** オプション用メータ画像 */
		static const string OptionMeter = ImgFolderName + "OptionMeter.png";
		/** オプション用マップチップサンプル画像 */
		static const string OptionMapChip = ImgFolderName + "mapChipSample.png";

		/** 神社画像 */
		static const string TempleImg = ImgFolderName + "temple.png";

		/** マップチップ */
		static const string MapChipImg = ImgFolderName + "mapChip.png";

		/** 攻撃範囲 */
		static const string RangeImg = ImgFolderName + "rangeCircle.png";

		/** 停止・再生ボタン */
		static const string PlayStopButtonImg = ImgFolderName + "playstopButton.png";

		/** 売却アイコン */
		static const string SaleIconImage = ImgFolderName + "sale.png";

		/** 覚醒アイコン */
		static const string AwakeIconImage = ImgFolderName + "awake.png";

		/** 赤字黒字アイコン */
		static const string EconomicIconImage = ImgFolderName + "economic.png";

		/** 状況表示 */
		static const string StateBackImage = ImgFolderName + "state.png";

		/** リザルトロゴ */
		static const string ResultLogoImage = ImgFolderName + "result.png";

		/** 終了確認背景 */
		static const string ConfirmBackImage = ImgFolderName + "confirmBack.png";

		/** スコアボード */
		static const string ScoreBoardImage = ImgFolderName + "scoreBoard.png";

		/** 終了メッセージ画像 */
		static const string EndMessageImage = ImgFolderName + "endMessage.png";

		/** エフェクト関連 */
		/** ショット画像 */
		static const string ShotImage = ImgFolderName + "shot.png";
		/** 爆破画像 */
		static const string ExplosionImage = ImgFolderName + "explosion.png";
		/** エフェクト配列 */
		static const string EffectImagesArray[] = {
			ShotImage,
			ExplosionImage
		};
		static const vector<string> EffectImagesVector(EffectImagesArray, std::end(EffectImagesArray));

		/* キャラ立ち絵 */
		static const string PlaneStandImage    = ImgFolderName + "plane.png";
		static const string MikoStandImage     = ImgFolderName + "miko.png";
		static const string MajokkoStandImage  = ImgFolderName + "majokko.png";
		static const string YojoStandImage     = ImgFolderName + "yojo.png";

		/* 立ち絵名配列 */
		static const string StandImagesArray[] = { 
			PlaneStandImage,
			MikoStandImage,
			MajokkoStandImage,
			YojoStandImage    
		};
		static const vector<string> StandImagesVector(StandImagesArray, std::end(StandImagesArray));

		/** キャラドット絵 */
		static const string PlaneDotImage   = ImgFolderName + "planedot.png";
		static const string MikoDotImage    = ImgFolderName + "mikodot.png";
		static const string MajokkoDotImage = ImgFolderName + "majokkodot.png";
		static const string YojoDotImage    = ImgFolderName + "yojodot.png";

		/** ドット絵配列 */
		static const string DotImagesArray[] = {
			PlaneDotImage,
			MikoDotImage,
			MajokkoDotImage,
			YojoDotImage    
		};
		static const vector<string> DotImagesVector( DotImagesArray, std::end( DotImagesArray ) );

		/** 敵キャラドット絵 */
		static const string OcjDotImage   = ImgFolderName + "ocjdot.png";
		static const string PizzaDotImage = ImgFolderName + "pizza.png";
		static const string RoomDotImage  = ImgFolderName + "room.png";
		static const string OcjDot2Image   = ImgFolderName + "ocjdot2.png";
		static const string PizzaDot2Image = ImgFolderName + "pizza2.png";
		static const string RoomDot2Image  = ImgFolderName + "room2.png";

		static const string BigOcjImage   = ImgFolderName + "bigocj.png";
		static const string CharOcjImage  = ImgFolderName + "charPizza.png";
		static const string BlackOcjImage = ImgFolderName + "blackRoom.png";

		/** 敵キャラドット絵配列 */
		static const string EnemyDotImageArray[] = {
			OcjDotImage,
			PizzaDotImage,
			RoomDotImage,
			OcjDot2Image,
			PizzaDot2Image,
			RoomDot2Image,
			BigOcjImage,
			CharOcjImage,
			BlackOcjImage
		};
		static const vector<string> EnemyDotImageVector( EnemyDotImageArray, std::end( EnemyDotImageArray ) );
	}

	namespace Sound{
		/* 音素材入れるフォルダ名 */
		static const string SoundFolderName = DataFolderName + "";
		static const string TitleBGMName = SoundFolderName + "見知らぬ道4.mp3";
		static const string StageBGMName = SoundFolderName + "参道を渡る(仮).mp3";
		
		/** 爆発音 */
		static const string ExplosionSEName = SoundFolderName + "explosion05.wav";
		/** ショット音 */
		static const string ShotSEName = SoundFolderName + "shot.wav";
		/** 決定音 */
		static const string DecisionSEName = SoundFolderName + "決定音.wav";

		static const string SEFileNameArray[] = {
			ExplosionSEName,
			ShotSEName,
			DecisionSEName
		};
		static const vector<string> SEFileNameVector( SEFileNameArray, std::end( SEFileNameArray ) );

	}
}