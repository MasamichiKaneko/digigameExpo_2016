#include "Configuration.h"
#include "define.h"
#include "utils.h"
#include "XMLNodeNameString.h"
#include "CommonString.h"
#include "utils.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::property_tree;
using namespace ConfigData;

Configuration* Configuration::instance = 0;

Configuration::Configuration(){
}

Configuration::~Configuration(){
}

void Configuration::create(){
	if ( instance != 0 ){
		ASSERT( false );
	} else {
		instance = new Configuration();
	}
}

/** �ϐݒ�ǂݍ��� */
bool Configuration::readVariableConfig( const char* fileName ){
	bool result = true;
	ptree pt;

	if ( !readXml( fileName, pt ) ) {
		result = false;
		goto FUNC_END;
	}

	for ( unsigned int i = 0; i < ValueNodeVector.size(); i++ ) {
		string targetNode = RootNode + "." + ValueNodeVector.at(i);
		if( !readAttr( targetNode, mSettingValue[ValueNodeVector.at(i)], pt, VariableConfigVector ) ) {
			result = false;
			goto FUNC_END;
		}
	}
FUNC_END:
	return result;
}

/** �Œ�ݒ�ǂݍ��� */
bool Configuration::readFixedConfig( const char* fileName ) {
	bool result = true;
	ptree pt;
	boost::optional<int> fpsBuf;
	boost::optional<bool> fullScreenBuf;

	if ( !readXml( fileName, pt ) ) {
		result = false;
		goto FUNC_END;
	}

	fpsBuf        = pt.get_optional<int >( RootNode + "." + FrameRateNode  );
	fullScreenBuf = pt.get_optional<bool>( RootNode + "." + ScreenModeNode );

	/** �ЂƂł��ǂ߂Ȃ���Ύ��s */
	if ( !( fpsBuf && fullScreenBuf) ) {
		result = false;
		goto FUNC_END;
	}

	mFrameRate    = *fpsBuf;
	mWindowMode   = *fullScreenBuf;

FUNC_END:
	return result;
}

void Configuration::writeVariableConfig( const char* fileName ){

	ptree pt;
	
	map< string, map<string, int> >::iterator It = mSettingValue.begin();

	for ( ; It != mSettingValue.end() ; It++ ) {
		ptree& child = pt.add( RootNode +"."+ It->first ,"" );
		map< string, int >::iterator childIt = It->second.begin();
		for ( ; childIt != It->second.end() ; childIt++ ){
			child.put( XmlCommon::attrStr + childIt->first, childIt->second );
		}		
	}
	write_xml( fileName, pt ,std::locale());
}

bool Configuration::setVariableValue( string valueType, int value ) {
	bool result = true;
	
	/** �ݒ�l���ǂݍ��܂�Ă��邩�`�F�b�N */
	if ( mSettingValue.empty() ) {
		result = false;
		goto FUNC_END;
	}

	/** �ݒ�l�A�ő�l�ǂ�������݂��邩�`�F�b�N */
	int existValueFlag = mSettingValue[MaxValueNode].count(valueType);
	existValueFlag *= mSettingValue[SetValueNode].count(valueType);
	if ( existValueFlag == 0 ) {
		result = false;
		goto FUNC_END;
	}

	/** �ݒ肵�悤�Ƃ��Ă���l���ő�l�𒴂��Ȃ����`�F�b�N */
	int maxValue = mSettingValue[MaxValueNode][valueType];
	if ( maxValue < value ){
		result = false;
		goto FUNC_END;
	}

	/** �ȏ�̃`�F�b�N��ʂ�����ݒ肷�� */
	mSettingValue[SetValueNode][valueType] = value;

FUNC_END:
	return result;
}

void Configuration::destroy(){
	SAFE_DELETE( instance );
}