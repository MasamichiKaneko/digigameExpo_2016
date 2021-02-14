#pragma once
#include <string>
#include <vector>
using namespace std;

/** ������u���� */

namespace CommonStr{

	/** ���� */
	namespace Common{
		static const string DecisionStr  = "����";
		static const string CancelStr    = "�L�����Z��";
		static const string SecondStr    = "�b";
		static const string TroutStr     = "�e";
		static const string GameTitleStr = "�ςƃI�R�W���̑K�푈";
	};

	/** �^�C�g���֘A */
	namespace Title{
		static const string StartModeStr  = "�͂��߂�";
		static const string OptionModeStr = "�I�v�V����";
		static const string EndModeStr    = "�����";
	};

	/** �Z���N�g�֘A */
	namespace Select{
		static const string StartModeStr = "�͂��߂�";
		static const string BuildModeStr = "���x���ݒ�";
		static const string TitleModeStr = "�^�C�g����";
		static const string InitAssetStr = "�������Y:";
		static const string StdLevelStr  = "�W�����x��:";
		static const string ScoreStr     = "�X�R�A:";
		static const string StageStr     = "�X�e�[�W:";
	};

	/** �X�e�[�^�X�֘A */
	namespace Status{
		static const string LvStr = "Lv ";
		static const string SpStr = "Sp ";
		static const string NeedExpStr = "��Lv�܂�";
		static const string NoStatusStr = "--";
		static const string PowerStr = "�Η�";
		static const string SpeedStr = "�U���Ԋu";
		static const string RangeStr = "�U���͈�";
	};

	/** �{�Ҋ֘A */
	namespace Main{
		static const string StartStr      = "�}���J�n";
		static const string PauseStr      = "�ꎞ��~";
		static const string EndStr        = "��߂�";
		static const string ContinueStr   = "�Â���";
		static const string YesStr        = "�͂�";
		static const string NoStr         = "������";
	};
};