#pragma once
#include <string>
#include <vector>
using namespace std;

/** ���ʒ萔�u���� */

namespace CommonConst{

	/** �}�b�v�`�b�v�֘A */
	/** ������ */
	static const int MapChipDivNumAll = 14;
	static const int MapChipDivNumX   = 4;
	static const int MapChipDivNumY   = 4;
	/** �T�C�Y */
	static const int MapChipSize = 32;

	/** ��~�E�Đ��{�^�������� */
	static const int PlayStopButtonDivNumAll = 2;
	static const int PlayStopButtonDivNumX   = 2;
	static const int PlayStopButtonDivNumY   = 1;
	/** �T�C�Y */
	static const int PlayStopButtonSize      = 32;

	/** ���U���g���S������ */
	static const int ResultLogoDivNumAll = 2;
	static const int ResultLogoDivNumX = 1;
	static const int ResultLogoDivNumY = 2;
	/** �T�C�Y */
	static const int ResultLogoDivSizeX = 230;
	static const int ResultLogoDivSizeY = 100;

	/** �Ԏ������A�C�R�������� */
	static const int EconomicIconDivNumAll = 2;
	static const int EconomicIconDivNumX   = 1;
	static const int EconomicIconDivNumY   = 2;
	/** �T�C�Y */
	static const int EconomicIconDivSizeX   = 60;
	static const int EconomicIconDivSizeY   = 30;

	/** �L�����h�b�g�G�֘A */
	/** ������ */
	static const int CharaDotImgDivNumAll = 12;
	static const int CharaDotImgDivNumX   = 3;
	static const int CharaDotImgDivNumY   = 4;
	/** �T�C�Y */
	static const int CharaDotImgSizeX = 40;
	static const int CharaDotImgSizeY = 45;

	/** �G�L�����h�b�g�G�֘A */
	/** ������ */
	static const int EnemyDotImgDivNumAll = 16;
	static const int EnemyDotImgDivNumX   = 4;
	static const int EnemyDotImgDivNumY   = 4;
	/** �T�C�Y */
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
	/** �U���͈͂̊�{�P�� */
	static const int RangeBasicUnit = 10;

	/** ���j�摜�֘A */
	/** �T�C�Y */
	static const int ExplosionSizeX = 700;
	static const int ExplosionSizeY = 700;
	
	/** ������ */
	static const int ExplosionDivNumAll = 6;
	static const int ExplosionDivNumX = 3;
	static const int ExplosionDivNumY = 2;

	/** �V���b�g�摜 */
	/** �T�C�Y */
	static const int ShotSizeX = 8;
	static const int ShotSizeY = 8;

	/** ������ */
	static const int ShotDivNumAll = 1;
	static const int ShotDivNumX = 1;
	static const int ShotDivNumY = 1;

	/** �S�X�e�[�W�� */
	static const int TotalStageNum = 8;
}