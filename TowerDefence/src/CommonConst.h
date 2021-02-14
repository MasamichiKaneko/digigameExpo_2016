#pragma once
#include <string>
#include <vector>
using namespace std;

/** 共通定数置き場 */

namespace CommonConst{

	/** マップチップ関連 */
	/** 分割数 */
	static const int MapChipDivNumAll = 14;
	static const int MapChipDivNumX   = 4;
	static const int MapChipDivNumY   = 4;
	/** サイズ */
	static const int MapChipSize = 32;

	/** 停止・再生ボタン分割数 */
	static const int PlayStopButtonDivNumAll = 2;
	static const int PlayStopButtonDivNumX   = 2;
	static const int PlayStopButtonDivNumY   = 1;
	/** サイズ */
	static const int PlayStopButtonSize      = 32;

	/** リザルトロゴ分割数 */
	static const int ResultLogoDivNumAll = 2;
	static const int ResultLogoDivNumX = 1;
	static const int ResultLogoDivNumY = 2;
	/** サイズ */
	static const int ResultLogoDivSizeX = 230;
	static const int ResultLogoDivSizeY = 100;

	/** 赤字黒字アイコン分割数 */
	static const int EconomicIconDivNumAll = 2;
	static const int EconomicIconDivNumX   = 1;
	static const int EconomicIconDivNumY   = 2;
	/** サイズ */
	static const int EconomicIconDivSizeX   = 60;
	static const int EconomicIconDivSizeY   = 30;

	/** キャラドット絵関連 */
	/** 分割数 */
	static const int CharaDotImgDivNumAll = 12;
	static const int CharaDotImgDivNumX   = 3;
	static const int CharaDotImgDivNumY   = 4;
	/** サイズ */
	static const int CharaDotImgSizeX = 40;
	static const int CharaDotImgSizeY = 45;

	/** 敵キャラドット絵関連 */
	/** 分割数 */
	static const int EnemyDotImgDivNumAll = 16;
	static const int EnemyDotImgDivNumX   = 4;
	static const int EnemyDotImgDivNumY   = 4;
	/** サイズ */
	static const int OcjDotImgSizeX   = 40;
	static const int OcjDotImgSizeY   = 45;
	static const int PizzaDotImgSizeX = 40;
	static const int PizzaDotImgSizeY = 45;
	static const int RoomDotImgSizeX  = 40;
	static const int RoomDotImgSizeY  = 45;
	static const int OcjDot2ImgSizeX   = 40;
	static const int OcjDot2ImgSizeY   = 45;
	static const int PizzaDot2ImgSizeX = 40;
	static const int PizzaDot2ImgSizeY = 45;
	static const int RoomDot2ImgSizeX  = 40;
	static const int RoomDot2ImgSizeY  = 45;

	static const int BigOcjDotImgSizeX  = 80;
	static const int BigOcjDotImgSizeY  = 90;
	static const int CharOcjDotImgSizeX  = 40;
	static const int CharOcjDotImgSizeY  = 45;
	static const int BlackOcjDotImgSizeX  = 40;
	static const int BlackOcjDotImgSizeY  = 45;


	static const int EnemyDotImgSizeXArray[] = {
		OcjDotImgSizeX,
		PizzaDotImgSizeX,
		RoomDotImgSizeX,
		OcjDot2ImgSizeX,
		PizzaDot2ImgSizeX,
		RoomDot2ImgSizeX,
		BigOcjDotImgSizeX,
		CharOcjDotImgSizeX,
		BlackOcjDotImgSizeX
	};
	static const vector<int> EnemyDotImgSizeXVector( EnemyDotImgSizeXArray, std::end( EnemyDotImgSizeXArray ) );

	static const int EnemyDotImgSizeYArray[] = {
		OcjDotImgSizeY,
		PizzaDotImgSizeY,
		RoomDotImgSizeY,
		OcjDot2ImgSizeY,
		PizzaDot2ImgSizeY,
		RoomDot2ImgSizeY,
		BigOcjDotImgSizeY,
		CharOcjDotImgSizeY,
		BlackOcjDotImgSizeY
	};

	static const vector<int> EnemyDotImgSizeYVector( EnemyDotImgSizeYArray, std::end( EnemyDotImgSizeYArray ) );
	/** 攻撃範囲の基本単位 */
	static const int RangeBasicUnit = 10;

	/** 爆破画像関連 */
	/** サイズ */
	static const int ExplosionSizeX = 700;
	static const int ExplosionSizeY = 700;
	
	/** 分割数 */
	static const int ExplosionDivNumAll = 6;
	static const int ExplosionDivNumX = 3;
	static const int ExplosionDivNumY = 2;

	/** ショット画像 */
	/** サイズ */
	static const int ShotSizeX = 8;
	static const int ShotSizeY = 8;

	/** 分割数 */
	static const int ShotDivNumAll = 1;
	static const int ShotDivNumX = 1;
	static const int ShotDivNumY = 1;

	/** 全ステージ数 */
	static const int TotalStageNum = 8;
}