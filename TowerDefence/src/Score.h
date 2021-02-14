#pragma once

#include <map>
#include <vector>

class Score{
public:
	~Score();

	static void create();
	static void destroy();
	
	bool readScore( const char* fileName );
	bool readScore( int fileHandle );

	void outputScoreFile( const char* fileName );
	
	/** 実体取得 */
	static Score* getInstance(){ return instance; }

	/** スコア挿入 */
	void insertScore( int stageNum, int scoreValue );

	/** スコア取得 */
	std::map<const int,std::vector<int> >* getScore() const{ return mScore; }
	/** 解禁ステージ数取得 */
	int getUnlockStageNum() const { return mUnlockStageNum; }
	/** 解禁ステージ数セット */
	void setUnlockStageNum( int aUnlockStageNum ) { mUnlockStageNum = aUnlockStageNum; }
private:

	Score();
	/**
	 * first: ステージNo.
	 * second:スコア配列
	 */
	std::map<const int , std::vector<int> >* mScore;

	/** 実体 */
	static Score* instance;

	/** 解禁ステージ数 */
	int mUnlockStageNum;

	/**
	 * ファイルコピー関数
	 * first:出力ファイル名 second:コピー元ファイルハンドル
	 */
	void fileCopy(const char* i_fileName,const char* o_fileName);
};