#pragma once

/**
 * �Q�[���{�҂Ŏg����t�@�C�����萔��`
 */

#include "Loader.h"
#include <string>

using namespace std;


/** �p�X���[�h��` */
namespace PassWord{
	static const char* EncryptPass = "ocjocj";
	static const char* ArchivesPass = "ocjocj";
};


namespace FileNames{
	/* �f�[�^�����t�H���_�̃��[�g�t�H���_�� */
	static const string DataFolderName = "data\\";

	/** �L�^�f�[�^ */
	/** �A�[�J�C�u�ɗ��߂��Ȃ��ϓ��f�[�^������Text��Ԃɓ���Ȃ� */
	/** �X�R�A�f�[�^ */
	static const string ScoreDataName = "score.dat";
	/* �L�����N�^�f�[�^ */
	static const string CharaStatus = "charaData.dat";
	
	namespace Text{

		/* �e�L�X�g�t�H���_�����t�H���_�� */
		static const string TxtFolderName = DataFolderName + "txt\\";
		
		/* �}�b�v��� */
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

		/* �G�̏o����� */
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

		/* �G�̃X�e�[�^�X��� */
		static const string EnemyStatusData  = TxtFolderName + "EnemyStatusData.dat";

		/* �R���t�B�O */
		static const string FixedConfig    = TxtFolderName + "FixedConfig.dat";
		static const string VariableConfig = "config.dat";

		/** �X�e�[�W�ݒ� */
		static const string StageConfigFileName = TxtFolderName + "StageConfig.dat";

		/** �e�L�^�f�[�^������� */
		static const string ScoreDataTemplateName  = TxtFolderName + "InitScore.dat";
		static const string CharaDataTemplateName  = TxtFolderName + "InitCharaData.dat";
		static const string ConfigDataTemplateName = TxtFolderName + "InitVariableConfig.dat";

		/** �o���l�e�[�u�� */
		static const string ExpTableDataName = TxtFolderName + "ExpTable.dat";
	}

	namespace Image{
		/* �摜�����t�H���_�� */
		static const string ImgFolderName = DataFolderName + "";
		/* ���S */
		static const string Logo    =   ImgFolderName + "logo.png";
		/* �^�C�g���w�i */
		static const string TitleBack = ImgFolderName + "title.png";
		/* �I�v�V�����w�i */
		static const string OptionBack = ImgFolderName + "option.png";

		/* ���[�h����� */
		static const string LoadBack = ImgFolderName + "Loading.png";

		/** �������ݒ���� */
		static const string WritingBack = ImgFolderName + "Writing.png";

		/* �^�C�g�����S */
		static const string TitleLogo = ImgFolderName + "title_logo.png";
		/* ���[�h�Z���N�g�p�t���[�� */
		static const string SelectFrame = ImgFolderName + "SelectFrame.png";
		/* �}�E�X�|�C���^�摜 */
		static const string MousePointer = ImgFolderName + "MousePointer.png";

		/* �X�e�[�W�Z���N�g�w�i */
		static const string StageSelectBack = ImgFolderName + "stageSelect.png";
		/* �X�e�[�W�A�C�R���摜 */
		static const string StageIcon =     ImgFolderName + "StageIcon.png";
		static const string LockStageIcon = ImgFolderName + "LockStageIcon.png";
		/* �X�e�[�W�I��p�J�[�\�� */
		static const string StageSelectCursor = ImgFolderName + "StageSelectCursor.png";
		/* �X�R�A�`��ʒu�摜 */
		static const string ShowScoreArea = ImgFolderName + "showScoreArea.png";
		/* ���J���X�e�[�W�L�[�摜 */
		static const string StageLock = ImgFolderName + "keyIcon.png";

		/* �琬��ʔw�i */
		static const string CharaBuildBack = ImgFolderName + "charaBuild.png";
		/* �琬��ʃJ�[�\�� */
		static const string CharaBuildCursor = ImgFolderName + "CharaBuildCursor.png";
		/** �琬�X�e�[�^�X�\���o�b�N */
		static const string CharaBuildStatusBack = ImgFolderName + "buildStatusBack.png";
		/** �X�e�[�^�X�\���o�[ */
		static const string CharaBuildStatusBar = ImgFolderName + "statusBar.png";
		/** �}�A�C�R�� */
		static const string CharaBuildPlusMinusIcon = ImgFolderName + "plusminusIcon.png";
		/** ���[�^�摜 */
		static const string CharaBuildMeter = ImgFolderName + "BuildMeter.png";
		/** �I�v�V�����p���[�^�摜 */
		static const string OptionMeter = ImgFolderName + "OptionMeter.png";
		/** �I�v�V�����p�}�b�v�`�b�v�T���v���摜 */
		static const string OptionMapChip = ImgFolderName + "mapChipSample.png";

		/** �_�Љ摜 */
		static const string TempleImg = ImgFolderName + "temple.png";

		/** �}�b�v�`�b�v */
		static const string MapChipImg = ImgFolderName + "mapChip.png";

		/** �U���͈� */
		static const string RangeImg = ImgFolderName + "rangeCircle.png";

		/** ��~�E�Đ��{�^�� */
		static const string PlayStopButtonImg = ImgFolderName + "playstopButton.png";

		/** ���p�A�C�R�� */
		static const string SaleIconImage = ImgFolderName + "sale.png";

		/** �o���A�C�R�� */
		static const string AwakeIconImage = ImgFolderName + "awake.png";

		/** �Ԏ������A�C�R�� */
		static const string EconomicIconImage = ImgFolderName + "economic.png";

		/** �󋵕\�� */
		static const string StateBackImage = ImgFolderName + "state.png";

		/** ���U���g���S */
		static const string ResultLogoImage = ImgFolderName + "result.png";

		/** �I���m�F�w�i */
		static const string ConfirmBackImage = ImgFolderName + "confirmBack.png";

		/** �X�R�A�{�[�h */
		static const string ScoreBoardImage = ImgFolderName + "scoreBoard.png";

		/** �I�����b�Z�[�W�摜 */
		static const string EndMessageImage = ImgFolderName + "endMessage.png";

		/** �G�t�F�N�g�֘A */
		/** �V���b�g�摜 */
		static const string ShotImage = ImgFolderName + "shot.png";
		/** ���j�摜 */
		static const string ExplosionImage = ImgFolderName + "explosion.png";
		/** �G�t�F�N�g�z�� */
		static const string EffectImagesArray[] = {
			ShotImage,
			ExplosionImage
		};
		static const vector<string> EffectImagesVector(EffectImagesArray, std::end(EffectImagesArray));

		/* �L���������G */
		static const string PlaneStandImage    = ImgFolderName + "plane.png";
		static const string MikoStandImage     = ImgFolderName + "miko.png";
		static const string MajokkoStandImage  = ImgFolderName + "majokko.png";
		static const string YojoStandImage     = ImgFolderName + "yojo.png";

		/* �����G���z�� */
		static const string StandImagesArray[] = { 
			PlaneStandImage,
			MikoStandImage,
			MajokkoStandImage,
			YojoStandImage    
		};
		static const vector<string> StandImagesVector(StandImagesArray, std::end(StandImagesArray));

		/** �L�����h�b�g�G */
		static const string PlaneDotImage   = ImgFolderName + "planedot.png";
		static const string MikoDotImage    = ImgFolderName + "mikodot.png";
		static const string MajokkoDotImage = ImgFolderName + "majokkodot.png";
		static const string YojoDotImage    = ImgFolderName + "yojodot.png";

		/** �h�b�g�G�z�� */
		static const string DotImagesArray[] = {
			PlaneDotImage,
			MikoDotImage,
			MajokkoDotImage,
			YojoDotImage    
		};
		static const vector<string> DotImagesVector( DotImagesArray, std::end( DotImagesArray ) );

		/** �G�L�����h�b�g�G */
		static const string OcjDotImage   = ImgFolderName + "ocjdot.png";
		static const string PizzaDotImage = ImgFolderName + "pizza.png";
		static const string RoomDotImage  = ImgFolderName + "room.png";
		static const string OcjDot2Image   = ImgFolderName + "ocjdot2.png";
		static const string PizzaDot2Image = ImgFolderName + "pizza2.png";
		static const string RoomDot2Image  = ImgFolderName + "room2.png";

		static const string BigOcjImage   = ImgFolderName + "bigocj.png";
		static const string CharOcjImage  = ImgFolderName + "charPizza.png";
		static const string BlackOcjImage = ImgFolderName + "blackRoom.png";

		/** �G�L�����h�b�g�G�z�� */
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
		/* ���f�ޓ����t�H���_�� */
		static const string SoundFolderName = DataFolderName + "";
		static const string TitleBGMName = SoundFolderName + "���m��ʓ�4.mp3";
		static const string StageBGMName = SoundFolderName + "�Q����n��(��).mp3";
		
		/** ������ */
		static const string ExplosionSEName = SoundFolderName + "explosion05.wav";
		/** �V���b�g�� */
		static const string ShotSEName = SoundFolderName + "shot.wav";
		/** ���艹 */
		static const string DecisionSEName = SoundFolderName + "���艹.wav";

		static const string SEFileNameArray[] = {
			ExplosionSEName,
			ShotSEName,
			DecisionSEName
		};
		static const vector<string> SEFileNameVector( SEFileNameArray, std::end( SEFileNameArray ) );

	}
}