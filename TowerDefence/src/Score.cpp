#include "DxLib.h"
#include "Score.h"
#include "define.h"
#include "aes_encryption.h"
#include "utils.h"
#include <sys\stat.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/exception/all.hpp>
#include "XMLNodeNameString.h"

using namespace boost::property_tree::xml_parser;
using namespace boost::property_tree;
using namespace ScoreXml;
using namespace std;


Score* Score::instance = 0;

Score::Score() : mScore( 0 ),mUnlockStageNum( 0 ){
	mScore = new map< const int, vector<int> >();
}

Score::~Score(){
	SAFE_DELETE( mScore );
}

void Score::create(){
	if ( instance != 0 ){
		ASSERT( false );
	} else {
		instance = new Score();
	}
}

void Score::destroy(){
	SAFE_DELETE( instance );
}

/** 読込み */
bool Score::readScore( const char* fileName ) {

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

	result = readScore( fileHandle );

	FileRead_close( fileHandle );

FUNC_END:
	return result;
}

bool Score::readScore( int fileHandle ) {
	struct stat ifile_stat;
	bool result = true;
	int stageNum;
	vector< int > scores;
	ptree pt;
	/** 読み込みバッファ */
	boost::optional<int> numReadBuf;
	boost::optional<int> firstReadBuf;
	boost::optional<int> secondReadBuf;
	boost::optional<int> thirdReadBuf;

	/** xml読込み */
	if ( !readXml( fileHandle, pt ) ) {
		result = false;
		goto FUNC_END;
	}

	mScore->clear();

	/** 解禁ステージ数読込み */
	numReadBuf = pt.get_optional<int>(RootNode +"."+ UnlockNode +"."+ StageNumAttr);
	if ( !numReadBuf ) {
		result = false;
		goto FUNC_END;
	}
	mUnlockStageNum = *numReadBuf;

	/** 各ステージスコア読込み */
	BOOST_FOREACH( ptree::value_type& child , pt.get_child( RootNode+"."+StageNode ) ){
		const ptree& info = child.second;
		numReadBuf    = info.get_optional<int>( StageNumAttr );
		firstReadBuf  = info.get_optional<int>( FirstScoreAttr );
		secondReadBuf = info.get_optional<int>( SecondScoreAttr );
		thirdReadBuf  = info.get_optional<int>( ThirdScoreAttr );

		if ( numReadBuf && firstReadBuf && secondReadBuf && thirdReadBuf ) {
			stageNum = *numReadBuf;
			scores.push_back( *firstReadBuf );
			scores.push_back( *secondReadBuf );
			scores.push_back( *thirdReadBuf );
		} else {
			result = false;
			goto FUNC_END;
		}
		mScore->insert( pair< const int,vector<int> >( stageNum, scores ) );
		scores.clear();
	}

FUNC_END:
	return result;
}

/** スコア追加 */
void Score::insertScore( int stageNum, int scoreValue ){

	/** 存在ステージかをチェック */
	ASSERT ( ( mScore->count( stageNum ) != 0 ) );

	vector< int >scoreArray = mScore->at( stageNum );
	scoreArray.push_back( scoreValue );
	/** 降順ソート */
	sort( scoreArray.begin(), scoreArray.end(), greater<int>() );

	/** 記録上限を超えた場合は最小値を除去 */
	if ( scoreArray.size() > RankAttrVector.size() ) {
		scoreArray.erase( min_element( scoreArray.begin(), scoreArray.end() ) );
	}
	/** スコア上書き */
	(*mScore)[ stageNum ] = scoreArray;
}

/** ファイル出力 */
void Score::outputScoreFile( const char* fileName ) {
	
	ptree pt;
	
	map<const int, vector<int>>::iterator it;

	/** 解禁数書き込み */
	ptree& child = pt.add(RootNode +"."+ UnlockNode, "");
	child.put(StageNumAttr, mUnlockStageNum);

	/** スコア書き込み */
	for (it = mScore->begin(); it != mScore->end(); it++) {
		ptree& child = pt.add( RootNode +"."+ StageNode + "."+ ScoreNode, "");

		child.put( StageNumAttr, it->first );
		
		vector< int > scores = it->second;
		for ( unsigned int i = 0; i < RankAttrVector.size(); i++ ) {
			child.put( RankAttrVector.at(i), scores.at(i) );
		}	
	}
	write_xml(fileName, pt, std::locale() );
}