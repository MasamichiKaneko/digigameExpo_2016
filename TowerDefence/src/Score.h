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
	
	/** ���̎擾 */
	static Score* getInstance(){ return instance; }

	/** �X�R�A�}�� */
	void insertScore( int stageNum, int scoreValue );

	/** �X�R�A�擾 */
	std::map<const int,std::vector<int> >* getScore() const{ return mScore; }
	/** ���փX�e�[�W���擾 */
	int getUnlockStageNum() const { return mUnlockStageNum; }
	/** ���փX�e�[�W���Z�b�g */
	void setUnlockStageNum( int aUnlockStageNum ) { mUnlockStageNum = aUnlockStageNum; }
private:

	Score();
	/**
	 * first: �X�e�[�WNo.
	 * second:�X�R�A�z��
	 */
	std::map<const int , std::vector<int> >* mScore;

	/** ���� */
	static Score* instance;

	/** ���փX�e�[�W�� */
	int mUnlockStageNum;

	/**
	 * �t�@�C���R�s�[�֐�
	 * first:�o�̓t�@�C���� second:�R�s�[���t�@�C���n���h��
	 */
	void fileCopy(const char* i_fileName,const char* o_fileName);
};