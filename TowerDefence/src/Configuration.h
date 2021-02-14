#pragma once

#include <map>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

/**
*  コンフィグレーションクラス
*/

class Configuration{
public:
	
	~Configuration();
	static void create();
	static void destroy();
	/** 可変コンフィグ読み込み */
	bool readVariableConfig( const char* fileName );
	/** 固定コンフィグ読み込み */
	bool readFixedConfig( const char* fileName );
	/** 可変コンフィグ書き込み */
	void writeVariableConfig( const char* fileName );

	/** 設定値変更関数 */
	bool setVariableValue( std::string valueType , int value );

	static Configuration* getInstance(){ return instance; }

	int  getFrameRate() { return mFrameRate;  }
	bool getWindowMode(){ return mWindowMode; }
	
	std::map< std::string, std::map<std::string, int> > getValue() const {
		return mSettingValue;
	}

private:
	Configuration();
	static Configuration* instance;
	
	/** 固定コンフィグ用 */
	int mFrameRate;
	bool mWindowMode;

	/** 可変値用 */
	std::map< std::string, std::map<std::string, int> > mSettingValue;
};