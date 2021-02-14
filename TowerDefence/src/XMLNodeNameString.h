#pragma once
#include <string>
#include <vector>
using namespace std;

/** xml�m�[�h����` */

namespace XmlCommon{
	static const string attrStr = "<xmlattr>.";
}

/** �X�R�A�f�[�^ */
namespace ScoreXml{

	/** �m�[�h����` */
	static const string RootNode        = "ScoreData";
	static const string StageNode       = "Stages";
	static const string ScoreNode       = "Scores";
	static const string UnlockNode      = "UnlockStage";

	/** ��������` */
	static const string StageNumAttr    = XmlCommon::attrStr + "num";
	static const string FirstScoreAttr  = XmlCommon::attrStr + "first";
	static const string SecondScoreAttr = XmlCommon::attrStr + "second";
	static const string ThirdScoreAttr  = XmlCommon::attrStr + "third";
	
	/** �g���₷���悤�z��� */
	static const string RankAttrArray[] = {
		FirstScoreAttr,
		SecondScoreAttr,
		ThirdScoreAttr  };
	static const vector<string> RankAttrVector( RankAttrArray, std::end(RankAttrArray) );
}

/** �R���t�B�O�f�[�^ */
namespace ConfigData{
	/** �m�[�h��` */
	static const string RootNode = "Configuration";
	/** �Œ� */
	static const string FrameRateNode    = "FrameRate";
	static const string ScreenModeNode   = "FullScreen";
	static const string BGMMaxLevelNode  = "BGMMax";
	static const string SEMaxLevelNode   = "SEMax";
	static const string GridMaxLevelNode = "GridMax";

	/** �� */
	static const string MaxValueNode = "MaxValue";
	static const string SetValueNode = "SetValue";

	static const string ValueNodeArray[] = {
		MaxValueNode,
		SetValueNode
	};
	static const vector<string> ValueNodeVector( ValueNodeArray, std::end(ValueNodeArray) );

	static const string BGMLevelStr  = "BGM";
	static const string SELevelStr   = "SE";
	static const string GridLevelStr = "Grid";

	static const string VariableConfigArray[] = {
		BGMLevelStr,
		SELevelStr,
		GridLevelStr
	};

	static const vector<string> VariableConfigVector( VariableConfigArray, std::end( VariableConfigArray ) );
}

/** �X�e�[�W�ݒ� */
namespace StageConfigData{
	/** �m�[�h����` */
	static const string RootNode  = "StageConfig";
	static const string StageNode = "Stage";

	/** ��������` */
	static const string StageNumAttr       = XmlCommon::attrStr + "num";
	static const string StageInitAssetAttr = XmlCommon::attrStr + "initAsset";
	static const string StageStdSpAttr     = XmlCommon::attrStr + "stdLv";
}

/** �L�����N�^�[�X�e�[�^�X�f�[�^ */
namespace CharaDataXml{
	/** �m�[�h��` */
	static const string RootNode          = "CharaData";	
	static const string InitNode          = "InitStatus";
	static const string SpRisingValueNode = "SPRisingValue";
	static const string SPNode            = "SkillPoint";
	static const string SPMaxNode         = "SPMaxValue";
	static const string ExpNode           = "ExpPoint";
	static const string CharaNode         = "Charactor";
	static const string ExpTableNode      = "ExpTable";
	static const string LevelNode         = "Level";
	static const string LvEachSPNode      = "SPValueEachOneLevel";

	/** ��������` */
	static const string CharaNameAttr       = XmlCommon::attrStr + "name";
	static const string CharaIdAttr         = XmlCommon::attrStr + "id";
	static const string CharaLvValueAttr    = XmlCommon::attrStr + "Lv";
	static const string CharaLvMaxValueAttr = XmlCommon::attrStr + "MaxLv";
	
	static const string LvValueAttr   = XmlCommon::attrStr + "value";
	static const string nextValueAttr = XmlCommon::attrStr + "nextRequirement";

	static const string PowerStr = "POWER";
	static const string SpeedStr = "SPEED";
	static const string RangeStr = "RANGE";
	static const string SkillStr = "SKILL";
	static const string CostStr  = "COST";
	static const string StatusStrArray[] = {
		PowerStr,
		SpeedStr,
		RangeStr,
		SkillStr,
		CostStr 
	};
	static const vector<string> StatusStrVector( StatusStrArray, std::end(StatusStrArray) );

	/** �σX�e�[�^�X */
	static const string VariableStatusStrArray[] = {
		PowerStr,
		SpeedStr,
		RangeStr
	};
	static const vector<string> VariableStatusStrVector( VariableStatusStrArray, std::end(VariableStatusStrArray) );
}

/** �G�L�����X�e�[�^�X�f�[�^ */
namespace EnemyDataXml{
	/** �m�[�h��` */
	static const string RootNode  = "EnemyData";
	static const string EnemyNode = "Enemy";

	/** ��������` */
	static const string EnemyIdAttr          = XmlCommon::attrStr + "id";

	static const string EnemyHitPointAttr    = XmlCommon::attrStr + "hitPoint";
	static const string EnemySpeedAttr       = XmlCommon::attrStr + "speed";
	static const string EnemyScoreAttr       = XmlCommon::attrStr + "score";
	static const string EnemyAssetAttr       = XmlCommon::attrStr + "asset";
	static const string EnemyAssetDamageAttr = XmlCommon::attrStr + "assetDamage";
	static const string EnemyRangeAttr       = XmlCommon::attrStr + "range";
	static const string EnemyStunRegistAttr  = XmlCommon::attrStr + "stunRegist";
	static const string EnemySlowRegistAttr  = XmlCommon::attrStr + "slowRegist";

	static const string StatusStrArray[] ={
		EnemyHitPointAttr,
		EnemySpeedAttr,
		EnemyScoreAttr,
		EnemyAssetAttr,
		EnemyAssetDamageAttr,
		EnemyRangeAttr,
		EnemyStunRegistAttr,
		EnemySlowRegistAttr
	};
	static const vector<string> StatusStrVector( StatusStrArray, std::end(StatusStrArray) );
}