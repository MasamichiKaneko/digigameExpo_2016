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

/** 可変設定読み込み */
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

/** 固定設定読み込み */
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

	/** ひとつでも読めなければ失敗 */
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
	
	/** 設定値が読み込まれているかチェック */
	if ( mSettingValue.empty() ) {
		result = false;
		goto FUNC_END;
	}

	/** 設定値、最大値どちらも存在するかチェック */
	int existValueFlag = mSettingValue[MaxValueNode].count(valueType);
	existValueFlag *= mSettingValue[SetValueNode].count(valueType);
	if ( existValueFlag == 0 ) {
		result = false;
		goto FUNC_END;
	}

	/** 設定しようとしている値が最大値を超えないかチェック */
	int maxValue = mSettingValue[MaxValueNode][valueType];
	if ( maxValue < value ){
		result = false;
		goto FUNC_END;
	}

	/** 以上のチェックを通ったら設定する */
	mSettingValue[SetValueNode][valueType] = value;

FUNC_END:
	return result;
}

void Configuration::destroy(){
	SAFE_DELETE( instance );
}