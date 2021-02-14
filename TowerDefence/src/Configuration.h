#pragma once

#include <map>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

/**
*  �R���t�B�O���[�V�����N���X
*/

class Configuration{
public:
	
	~Configuration();
	static void create();
	static void destroy();
	/** �σR���t�B�O�ǂݍ��� */
	bool readVariableConfig( const char* fileName );
	/** �Œ�R���t�B�O�ǂݍ��� */
	bool readFixedConfig( const char* fileName );
	/** �σR���t�B�O�������� */
	void writeVariableConfig( const char* fileName );

	/** �ݒ�l�ύX�֐� */
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
	
	/** �Œ�R���t�B�O�p */
	int mFrameRate;
	bool mWindowMode;

	/** �ϒl�p */
	std::map< std::string, std::map<std::string, int> > mSettingValue;
};