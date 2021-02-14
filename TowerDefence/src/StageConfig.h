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
	
	/** 実体取得 */
	static StageConfig* getInstance(){ return instance; }
	
	typedef struct{
		int initAsset;
		int stdLevelValue;
	} ConfigValue;

	std::map< const int , ConfigValue >* getConfigValue(){ return mStageConfig; }
	
private:
	StageConfig();

	/**
	 * first: ステージNo.
	 * second: 設定値
	 */
	std::map<const int , ConfigValue >* mStageConfig;

	/** 実体 */
	static StageConfig* instance;
};