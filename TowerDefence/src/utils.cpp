#include "utils.h"
#include "DxLib.h"
#include "Configuration.h"
#include "define.h"
#include <sstream>
#include <fstream>
#include <iomanip>

/**
* �L�[��1�񂾂���������
* [����]
* int InputKey:�L�[�R�[�h
* [�߂�l]
* true :�����Ă��� 
* false:�����Ă��Ȃ�or�O�t���[�����牟�����ςȂ�
*/
bool inputKeyTriggered( int InputKey ) {
	static bool ret[ 256 ] = {false};
	char Buf[256];
	GetHitKeyStateAll(Buf);
	if ( Buf[InputKey] == 1 ) {
		if ( !ret[InputKey] ) {
			ret[InputKey] = true;
			return true;
		} else {
			return false;
		}
	} else {
		ret[InputKey] = false;
	}
	return false;
}

/**
* �}�E�X���͂�1�񂾂����鏈��
* [����]
* int InputMouse:�}�E�X����
* [�߂�l]
* true :���͂����m
* false:���͂Ȃ�or�O�t���[��������͂����ςȂ�
*/
bool inputMouseTriggered(int InputMouse) {
	static bool ret[ 0x0080 ] = {false};
	int mouseInput = GetMouseInput();
	if ( mouseInput == InputMouse ) {
		if ( !ret[ InputMouse ] ) {
			ret[InputMouse] = true;
			return true;
		} else {
			return false;
		}
	} else {
		ret[ InputMouse ] = false;
	}
	return false;
}

/**
 * �ǂݍ��񂾃e�L�X�g�t�@�C���̒��g��S���擾���鏈��
 * [����]
 * vector<string>* output :�ǂݍ��񂾃e�L�X�g�̒��g���i�[����̈�(1�v�f���Ƃ�1�s�ǂݍ���)
 * int FileHandle         :�t�@�C���n���h��
 * [�߂�l]
 * true : ����I��
 * false: �G���[����
 */
bool FileRead_getAll( std::vector<std::string>* output,int FileHandle ){
	output->clear();
	while ( FileRead_eof( FileHandle )  == 0 ) {
		int bufSize = 4096;
		char buf[4096];
		int status = FileRead_gets( buf, bufSize, FileHandle );
		/* �G���[�`�F�b�N */
		if ( status == -1 ) {
			/* ���r���[�ɓǂݍ��ނ̂͂悭�Ȃ� */
			output->clear();
			return false;
		}
		output->push_back( buf );
	}
	return true;
}

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
bool FileRead_getAllConnectedStr( std::string* output, int FileHandle ){
	output->clear();
	std::stringstream readStr;
	bool result = FileRead_getAllConnectedStr(&readStr, FileHandle);
	if ( result ) {
		output = &readStr.str();
	}
	return result;
}

bool FileRead_getAllConnectedStr( std::stringstream* output, int FileHandle ){

	output->clear();
	std::vector<std::string> readData;
	bool result = FileRead_getAll( &readData, FileHandle );

	if ( result ) {
		for ( unsigned int i = 0; i < readData.size(); i++ ) {
			*output << readData.at(i) << std::endl;
		}
	}
	return result;
}

/**
 * ������vector�̒��g��S���t�@�C���ɏo�͂��鏈��
 * [����]
 * vector<string>* output :�o�͂��镶����vector
 * const char* FileName         :�o�̓t�@�C����
 * [�߂�l]
 * �Ȃ�
 */
void FileWrite_vectorAll( std::vector<std::string> output, const char* FileName ){
	std::ofstream ofs( FileName );
	std::stringstream readStr;

	for ( unsigned int i = 0; i < output.size(); i++ ) {
		readStr << output.at( i ) << std::endl;
	}
	ofs << readStr.str();
	ofs.close();
	return;
}

/**
* int�^�ϐ���0�l�߂Ŏw�茅���ɂ��낦�鏈��
* [����]
* int aValue: �Ώۂ̕ϐ�
* int digit : �w�茅��
* [�߂�l]
* string�^ 0���l�߂���̕�����
*/
std::string getZeroPadding( int aValue, int digit ){
	std::ostringstream oss;
	
	oss << std::setw( digit ) << std::setfill( '0' ) << aValue;

	return oss.str();
}

/**
* xml�ǂݍ��݊֐�
* [����]
* const char* fileName:�t�@�C����
* ptree &o_pt: �ǂݍ��݃v���p�e�B�c���[
* [�߂�l]
* true:�ǂݍ��ݐ���
* false:���s
*/
bool readXml( const char* fileName, boost::property_tree::ptree &o_pt ){
	bool result = false;
	std::stringstream readStr;
	/** �t�@�C���ǂݍ��� */
	int txtHandle;
	txtHandle = FileRead_open( fileName );
	if ( txtHandle != 0 ) {
		result = readXml( txtHandle, o_pt );
		ASSERT( FileRead_close( txtHandle ) + 1 );
	}
	return result;
}

bool readXml( const int fileHandle, boost::property_tree::ptree &o_pt ){
	bool result = true;
	std::stringstream readStr;
	/** �ǂݍ��݃`�F�b�N */
	if ( fileHandle == -1 ) {
		result = false;
		goto FUNC_END;
	}
	
	/** ������ǂݍ��� */
	ASSERT( FileRead_getAllConnectedStr( &readStr,fileHandle ) );

	/** xml�ǂݍ��� */
	try{
		boost::property_tree::read_xml( readStr ,o_pt );
	} catch( boost::exception& ) {
		result = false;
	}

FUNC_END:
	return result;
}

/**
 * �O���b�h�`��֐�
 */
void drawGrid( 
	int posX,      int posY,      //�`���n�ߍ��W
	int numH,      int numV,      //�{��(H:���� , V:����)
	int intervalH, int intervalV, //�Ԋu
	int color   ){                //�F
		/** ���̒����v�Z */
		int lengthH = intervalV * ( numV - 1 );
		int lengthV = intervalH * ( numH - 1 );

		/** ���������̐��`�� */
		for ( int i = 0; i < numH; i++ ) {
			ASSERT( DrawLine( 
				posX, 
				posY + ( i * intervalH ), 
				posX + lengthH, 
				posY + ( i * intervalH ), 
				color ) + 1 );
		}
		/** ���������̐��`�� */
		for ( int i = 0; i < numV; i++ ) {
			ASSERT( DrawLine( 
				posX + ( i * intervalV ),
				posY, 
				posX + ( i * intervalV ),
				posY + lengthV,
				color ) + 1 );
		}
}

/**
* �摜�̃t�F�[�h�C���A�t�F�[�h�A�E�g�����N���X
*/
FadeInFadeOut::FadeInFadeOut(
	float aFadeInTime,
	float aWaitInTime,
	float aFadeOutTime,
	float aWaitOutTime ) :
mFadeInTime( aFadeInTime ),
	mWaitInTime( aWaitInTime ),
	mFadeOutTime( aFadeOutTime ),
	mWaitOutTime( aWaitOutTime ),
	mMode( FADE_IN ),
	mTime( 0 ),
	mParameter( 0 )
{}

FadeInFadeOut::~FadeInFadeOut(){
	byte brightMax = 255;
	SetDrawBright(brightMax, brightMax, brightMax);
}

void FadeInFadeOut::UpdateParameter(){
	/* �t���[�����[�g�擾 */
	int rate = Configuration::getInstance()->getFrameRate();
	
	const byte brightMax = 255;
	int drawStatus = 0;

	/* 1f���Ƃ̖��邳�㏸�����ݒ�l */
	float crr = 0.0;
	/* ���邳�ݒ�l */
	
	switch( mMode ){
	case FADE_IN:
		/* 1f���Ƃ̖��邳�㏸�����ݒ� */
		crr = ( static_cast<float>( mTime ) / static_cast<float>( rate ) )
			* static_cast<float>( 1.0 / mFadeInTime );
		/* ���邳�ݒ� */
		mParameter = static_cast<int>( brightMax * ( crr ) );
		if ( mTime == static_cast< int >( rate * mFadeInTime ) ) {
			mTime = 0;
			mMode = WAIT_IN;
		} else {
			mTime++;
		}
		break;
	case WAIT_IN:
		if (mTime == static_cast<int>( rate * mWaitInTime )) {
			mTime = 0;
			mMode = FADE_OUT;
		} else {
			mTime++;
		}
		break;
	case FADE_OUT:
		/* 1f���Ƃ̖��邳���������ݒ� */
		crr = ( static_cast<float>( mTime ) / static_cast<float>( rate ) )
			*static_cast<float>( 1.0 / mFadeOutTime );
		/* ���邳�ݒ� */
		mParameter = brightMax - static_cast<int>( brightMax * ( crr ) );
		if ( mTime == static_cast< int >( rate * mFadeOutTime ) ) {
			mTime = 0;
			mMode = WAIT_OUT;
		} else {
			mTime++;
		}
		break;
	case WAIT_OUT:
		drawStatus = SetDrawBright(0, 0, 0);
		if ( mTime == rate * mWaitOutTime ) {
			mTime = 0;
			mMode = END;
		} else {
			mTime++;
		}
		break;
	case END:
		break;
	default:
		break;
	}

}

void FadeInFadeOut::setMode( Mode aMode ){
	mMode = aMode;
}
