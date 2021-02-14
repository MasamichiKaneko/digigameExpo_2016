#include "StageConfig.h"
#include "define.h"
#include "FileNames.h"
#include "DxLib.h"
#include "utils.h"
#include <sys\stat.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/exception/all.hpp>
#include "XMLNodeNameString.h"

using namespace boost::property_tree::xml_parser;
using namespace boost::property_tree;
using namespace StageConfigData;
using namespace std;

StageConfig* StageConfig::instance = 0;

StageConfig::StageConfig() : mStageConfig( 0 ){
	mStageConfig = new map<const int, ConfigValue>();
}

StageConfig::~StageConfig(){
	SAFE_DELETE( mStageConfig );
}

void StageConfig::create(){
	if ( instance != 0 ){
		ASSERT( false );
	} else {
		instance = new StageConfig();
	}
}

void StageConfig::destroy(){
	SAFE_DELETE( instance );
}

/** 読込み */
bool StageConfig::readStageConfig( const char* fileName ) {

	struct stat ifile_stat;
	int fileHandle;
	bool result = false;

	/** ファイル存在チェック */
	if (stat( fileName,&ifile_stat ) != 0) {
		goto FUNC_END;
	}

	/** 読み込みチェック */
	fileHandle = FileRead_open( fileName );
	if ( fileHandle == -1 ) {
		goto FUNC_END;
	}

	result = readStageConfig( fileHandle );

	FileRead_close( fileHandle );

FUNC_END:
	return result;
}


bool StageConfig::readStageConfig( int fileHandle ) {
	struct stat ifile_stat;
	bool result = true;
	int stageNum;
	ConfigValue configBuf;
	ptree pt;

	/** 読み込みバッファ */
	boost::optional<int> numReadBuf;
	boost::optional<int> initAssetReadBuf;
	boost::optional<int> stdSpReadBuf;

	/** xml読込み */
	if ( !readXml( fileHandle, pt ) ) {
		result = false;
		goto FUNC_END;
	}
	mStageConfig->clear();

	/** 各ステージスコア読込み */
	BOOST_FOREACH( ptree::value_type& child , pt.get_child( RootNode ) ){
		const ptree& info = child.second;
		numReadBuf       = info.get_optional<int>( StageNumAttr       );
		initAssetReadBuf = info.get_optional<int>( StageInitAssetAttr );
		stdSpReadBuf     = info.get_optional<int>( StageStdSpAttr     );

		if ( numReadBuf && initAssetReadBuf && stdSpReadBuf ) {
			stageNum = *numReadBuf;
			configBuf.initAsset = *initAssetReadBuf;
			configBuf.stdLevelValue = *stdSpReadBuf;
		} else {
			mStageConfig->clear();
			result = false;
			goto FUNC_END;
		}

		(*mStageConfig)[*numReadBuf] = configBuf;
	}

FUNC_END:
	return result;
}