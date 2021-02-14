#include <vector>
#include <map>
#include <string>
#include <XMLNodeNameString.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/exception/all.hpp>

/**
* �����u����
* ������Ƃ����֐��͂����ɒǉ�����
*/
#pragma once

/**
* �L�[��1�񂾂���������
* [����]
* int InputKey:�L�[�R�[�h
* [�߂�l]
* true :�����Ă��� 
* false:�����Ă��Ȃ�or�O�t���[�����牟�����ςȂ�
*/
bool inputKeyTriggered( int CheckHitKey );

/**
 * �ǂݍ��񂾃e�L�X�g�t�@�C���̒��g��S���擾���鏈��
 * [����]
 * vector<string>* output :�ǂݍ��񂾃e�L�X�g�̒��g���i�[����̈�(1�v�f���Ƃ�1�s�ǂݍ���)
 * int FileHandle         :�t�@�C���n���h��
 * [�߂�l]
 * true : ����I��
 * false: �G���[����
 */
bool FileRead_getAll( std::vector<std::string>* output,int FileHandle );

/**
 * �ǂݍ��񂾃e�L�X�g�t�@�C���̒��g��1�̕�����f�[�^�őS���擾���鏈��
 * DxLib�̃A�[�J�C�u�@�\�ɑΉ�
 * [����]
 * string* output :�ǂݍ��񂾃e�L�X�g�̒��g���i�[����̈�
 * int FileHandle         :�t�@�C���n���h��
 * [�߂�l]
 * true : ����I��
 * false: �G���[����
 */
bool FileRead_getAllConnectedStr( std::string* output, int FileHandle );
bool FileRead_getAllConnectedStr( std::stringstream* output, int FileHandle );

/**
 * ������vector�̒��g��S���t�@�C���ɏo�͂��鏈��
 * [����]
 * vector<string>* output :�o�͂��镶����vector
 * const char* FileName         :�o�̓t�@�C����
 * [�߂�l]
 * �Ȃ�
 */
void FileWrite_vectorAll( std::vector<std::string> output, const char* FileName );

/**
* �}�E�X���͂�1�񂾂����鏈��
* [����]
* int InputMouse:�}�E�X����
* [�߂�l]
* true :���͂����m
* false:���͂Ȃ�or�O�t���[��������͂����ςȂ�
*/
bool inputMouseTriggered(int InputMouse);

/**
* xml�ǂݍ��݊֐�
* [����]
* const char* fileName:�t�@�C����
* ptree &o_pt: �ǂݍ��݃v���p�e�B�c���[
* [�߂�l]
* true:�ǂݍ��ݐ���
* false:���s
*/
bool readXml( const char* fileName, boost::property_tree::ptree &o_pt );

/**
* [����]
* const int fileHandle:�t�@�C���n���h��
* ptree &o_pt: �ǂݍ��݃v���p�e�B�c���[
*/
bool readXml( const int fileHandle, boost::property_tree::ptree &o_pt );

/**
 * �O���b�h�`��֐�
 */
void drawGrid( 
	int posX,      int posY,      //�`���n�ߍ��W
	int numH,      int numV,      //�{��(H:���� , V:����)
	int intervalH, int intervalV, //�Ԋu
	int color   );                //���̐F


/**
* �m�[�h�̑�����ǂݍ��ފ֐�
* 
*/
template <typename T> bool readAttr( 
	std::string targetNodeName, 
	std::map<std::string, T>& o_attrValues,
	boost::property_tree::ptree &i_pt,
	std::vector<std::string> attrNames ){
		bool result = true;
		boost::optional<T> readBuf;

		for ( unsigned int i = 0; i < attrNames.size(); i++ ) {
			/** �ǂݍ��� */
			std::string readAttr = XmlCommon::attrStr + attrNames.at(i);
			readBuf = i_pt.get_optional<T>( targetNodeName + "."+ readAttr );

			if ( !readBuf ){
				o_attrValues.clear();
				result = false;
				goto FUNC_END;
			}

			o_attrValues[attrNames.at(i)] = *readBuf;
		}

FUNC_END:
		return result;
}


/**
* int�^�ϐ���0�l�߂Ŏw�茅���ɂ��낦�鏈��
* [����]
* int aValue: �Ώۂ̕ϐ�
* int digit : �w�茅��
* [�߂�l]
* string�^ 0���l�߂���̕�����
*/
std::string getZeroPadding( int aValue, int digit );

/**
* �t�F�[�h�C���A�t�F�[�h�A�E�g�̌��x�����N���X
* �ʃt�@�C���ɐ؂�o������
*/
class FadeInFadeOut{
public:
	FadeInFadeOut(
		float aFadefloatime,
		float aWaitfloatime,
		float aFadeOutTime,
		float aWaitOutTime );
	~FadeInFadeOut();

	enum Mode{
		FADE_IN,  //�t�F�[�h�C��
		WAIT_IN,  //�t�F�[�h�C��������҂�����
		FADE_OUT, //�t�F�[�h�A�E�g
		WAIT_OUT, //�t�F�[�h�A�E�g������҂�����
		END       //�I��
	};

	Mode getMode()const{	return mMode; }
	int getParameter() const { return mParameter; }

	/** ���f�p */
	void setMode( Mode aMode );

	/** �p�����[�^���X�V */
	void UpdateParameter();
private:
	FadeInFadeOut();
	Mode mMode;
	int mTime;
	int mParameter;
	float mFadeInTime;
	float mWaitInTime;
	float mFadeOutTime;
	float mWaitOutTime;
};




