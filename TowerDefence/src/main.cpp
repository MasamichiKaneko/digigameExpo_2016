#include "DxLib.h"
#include <math.h>
#include <Windows.h>
#include "define.h"
#include "Sequence\Parent.h"
#include "Configuration.h"
#include "FileNames.h"
#include "aes_encryption.h"
#include "StageConfig.h"
#include "CommonString.h"

class Fps{
private:
	unsigned int mStartTime;         //����J�n����
	unsigned int mCount;             //�J�E���^
	float mFps;             //fps
	static const int N = 60;//���ς����T���v����
	int FPS;	//�ݒ肵��FPS

public:
	Fps( int afps ){
		FPS = afps;
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	bool Update(){
		if( mCount == 0 ){ //1�t���[���ڂȂ玞�����L��
			mStartTime = static_cast< unsigned int >( GetNowCount() );
		}
		if( mCount == N ){ //60�t���[���ڂȂ畽�ς��v�Z����
			int t =  static_cast< unsigned int >( GetNowCount() );
			mFps = 1000.f / ( ( t - mStartTime )/(float)N);
			mCount = 0;
			mStartTime = t;
		}
		mCount++;
		return true;
	}

	void Draw(){
		DrawFormatString(0, 0, GetColor(255,255,255), "%.1f", mFps);
	}

	void Wait(){
		int tookTime = GetNowCount() - mStartTime;	//������������
		int waitTime = mCount*1000/FPS - tookTime;	//�҂ׂ�����
		if( waitTime > 0 ){
			Sleep(waitTime);	//�ҋ@
		}
	}
};

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	
	Configuration::create();
	/** �Œ�R���t�B�O�ǂݍ��� */
	ASSERT( Configuration::getInstance()->readFixedConfig( FileNames::Text::FixedConfig.c_str() ) );
	
	/** �σR���t�B�O�ǂݍ��� */
	/** ������ */
	decrypt( FileNames::Text::VariableConfig.c_str(), PassWord::EncryptPass );
	if ( Configuration::getInstance()->readVariableConfig( FileNames::Text::VariableConfig.c_str() ) ) {
		/** �����Ȃ�ēx�Í��� */
		encrypt( FileNames::Text::VariableConfig.c_str(), PassWord::EncryptPass );
	} else {
		/** ���s�Ȃ珉����ԃR���t�B�O�ǂݍ��� */
		ASSERT( Configuration::getInstance()->readVariableConfig( FileNames::Text::ConfigDataTemplateName.c_str() ) );
	}

	/** fps�ݒ� */
	Fps fps( Configuration::getInstance()->getFrameRate() );

	/** �X�e�[�W�ݒ�ǂݍ��� */
	StageConfig::create();
	ASSERT( StageConfig::getInstance()->readStageConfig( FileNames::Text::StageConfigFileName.c_str() ) );

	/** �E�B���h�E�^�C�g���ݒ� */
	ASSERT( SetMainWindowText( CommonStr::Common::GameTitleStr.c_str() ) + 1 );

	ChangeWindowMode(Configuration::getInstance()->getWindowMode()),DxLib_Init(),SetDrawScreen( DX_SCREEN_BACK );

	while( ProcessMessage()==0 && ClearDrawScreen()==0 && CheckHitKey(KEY_INPUT_ESCAPE)==0
		&& !Sequence::Parent::isGameEsc ){
		if( !Sequence::Parent::instance() ) {
			Sequence::Parent::create();
		}
		
		Sequence::Parent::instance()->update();
		fps.Update();	//�X�V
#ifdef _DEBUG
		fps.Draw();		//�`��
#endif
		ScreenFlip();
		fps.Wait();		//�ҋ@
	}

	Sequence::Parent::instance()->destroy();
	DxLib_End();
	return 0;
}