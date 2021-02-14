#pragma once

/**
* �t�@�C�����[�_�[�N���X
* ���[�h��ʓ�������Ђɓǂݍ��ޏꍇ�͂�����
*/

#include<map>
#include<vector>
#include<string>

/**
 * �����Ǎ��ݗp�X�e�[�^�X�N���X
 * FileLoader�ɕ����摜�t�@�C����H�킹��Ƃ��p
 */
class DivStatus{
public:
	DivStatus(
		std::string aFileName,
		int aAllNum,
		int aXNum,
		int aYNum,
		int aXSize,
		int aYSize );
	~DivStatus();

	static int LoadDivGraph(DivStatus div,int* GraphHandle);
	static int LoadDivGraph(DivStatus* div,int* GraphHandle);

	/* FileLoader�̂�private�փA�N�Z�X�� */
	friend class FileLoader;

private:
	DivStatus();
	std::string FileName;  //�t�@�C����
	int AllNum;            //��������
	int XNum;              //��������
	int YNum;              //�c������
	int XSize;             //���T�C�Y
	int YSize;             //�c�T�C�Y
};

class FileLoader{
public:
	FileLoader(
		std::vector< std::string >* ImageFileNameList,   //�摜�t�@�C��
		std::vector< DivStatus >* DivImageFileNameList,  //�摜�t�@�C��(����)
		std::vector< std::string >* TextFileNameList,    //�e�L�X�g�t�@�C��
		std::vector< std::string >* SoundFileNameList);  //�T�E���h�t�@�C��

	~FileLoader();

	/**
	 * �t�@�C���ǂݍ��݌��ʂ�Ԃ�
	 * true:�S���ǂݍ��݊��� false:�܂�
	 */
	bool CheckFileRead();

	/**  
	 *  �摜�ǂݍ��݌��ʂ�Ԃ��A�Ȃ��Ƃ���null
	 *  first:�ǂݍ��݃t�@�C���� 
	 *  second:�摜�ǂݍ��݃n���h��
	 */
	std::map< std::string ,int >* getImageFileHandle(){ return mImageFileHandlers; }
	
	/**
	 * �摜�����Ǎ��݂̌��ʂ�Ԃ��A�Ȃ��Ƃ���null
	 *  first:�ǂݍ��݃t�@�C���� 
	 *  second:�摜�ǂݍ��݃n���h��
	 */
	std::map< std::string ,std::vector<int> >* getDivImageFileHanlde(){ return mDivImageFileHandlers; }

	/**  
	 *  �e�L�X�g�ǂݍ��݌��ʂ�Ԃ��A�Ȃ��Ƃ���null
	 *  first:�ǂݍ��݃t�@�C���� 
	 *  second:�e�L�X�g�ǂݍ��݃n���h��
	 */
	std::map< std::string,int >* getTextFileHandle(){ return mTextFileHandlers; }
	
	/**  
	 *  �T�E���h�ǂݍ��݌��ʂ�Ԃ��A�Ȃ��Ƃ���null
	 *  first:�ǂݍ��݃t�@�C���� 
	 *  second:�T�E���h�ǂݍ��݃n���h��
	 */
	std::map< std::string,int >* getSoundFileHandle(){ return mSoundFileHandlers; }

	/**
	 * ���Ǎ��ݏ�������Ԃ�
	 */
	int getLoadFunctionTotal(){ return mLoadFunction; }

	/**
	* �ǂݍ��݃n���h���폜�֐�
	*/
	bool deleteTextHandle( std::string aFileName );
	bool deleteImageHandle( std::string aFileName );
	bool deleteSoundHandle( std::string aFileName );
	bool deleteDivImageHandle( std::string aFileName );

	/** �S�n���h���폜 */
	void deleteTextHandleAll();
	void deleteImageHandleAll();
	void deleteSoundHandleAll();
	void deleteDivImageHandleAll();

private:
	int mLoadFunction;

	/* �n�����߂̃t�@�C���f�[�^ */
	std::map< std::string , int >* mImageFileHandlers;
	std::map< std::string , std::vector<int> >*mDivImageFileHandlers;
	std::map< std::string, int >* mTextFileHandlers;
	std::map< std::string, int >* mSoundFileHandlers;
};