#pragma once

#include <map>
#include <vector>

class StageConfig{
public:
	~StageConfig();

	static void create();
	static void destroy();
	
	bool readStageConfig( const char* fileName );
	bool readStageConfig( int fileHandle );
	
	/** ���̎擾 */
	static StageConfig* getInstance(){ return instance; }
	
	typedef struct{
		int initAsset;
		int stdLevelValue;
	} ConfigValue;

	std::map< const int , ConfigValue >* getConfigValue(){ return mStageConfig; }
	
private:
	StageConfig();

	/**
	 * first: �X�e�[�WNo.
	 * second: �ݒ�l
	 */
	std::map<const int , ConfigValue >* mStageConfig;

	/** ���� */
	static StageConfig* instance;
};