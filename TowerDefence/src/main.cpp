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
	unsigned int mStartTime;         //測定開始時刻
	unsigned int mCount;             //カウンタ
	float mFps;             //fps
	static const int N = 60;//平均を取るサンプル数
	int FPS;	//設定したFPS

public:
	Fps( int afps ){
		FPS = afps;
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	bool Update(){
		if( mCount == 0 ){ //1フレーム目なら時刻を記憶
			mStartTime = static_cast< unsigned int >( GetNowCount() );
		}
		if( mCount == N ){ //60フレーム目なら平均を計算する
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
		int tookTime = GetNowCount() - mStartTime;	//かかった時間
		int waitTime = mCount*1000/FPS - tookTime;	//待つべき時間
		if( waitTime > 0 ){
			Sleep(waitTime);	//待機
		}
	}
};

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	
	Configuration::create();
	/** 固定コンフィグ読み込み */
	ASSERT( Configuration::getInstance()->readFixedConfig( FileNames::Text::FixedConfig.c_str() ) );
	
	/** 可変コンフィグ読み込み */
	/** 復号化 */
	decrypt( FileNames::Text::VariableConfig.c_str(), PassWord::EncryptPass );
	if ( Configuration::getInstance()->readVariableConfig( FileNames::Text::VariableConfig.c_str() ) ) {
		/** 成功なら再度暗号化 */
		encrypt( FileNames::Text::VariableConfig.c_str(), PassWord::EncryptPass );
	} else {
		/** 失敗なら初期状態コンフィグ読み込み */
		ASSERT( Configuration::getInstance()->readVariableConfig( FileNames::Text::ConfigDataTemplateName.c_str() ) );
	}

	/** fps設定 */
	Fps fps( Configuration::getInstance()->getFrameRate() );

	/** ステージ設定読み込み */
	StageConfig::create();
	ASSERT( StageConfig::getInstance()->readStageConfig( FileNames::Text::StageConfigFileName.c_str() ) );

	/** ウィンドウタイトル設定 */
	ASSERT( SetMainWindowText( CommonStr::Common::GameTitleStr.c_str() ) + 1 );

	ChangeWindowMode(Configuration::getInstance()->getWindowMode()),DxLib_Init(),SetDrawScreen( DX_SCREEN_BACK );

	while( ProcessMessage()==0 && ClearDrawScreen()==0 && CheckHitKey(KEY_INPUT_ESCAPE)==0
		&& !Sequence::Parent::isGameEsc ){
		if( !Sequence::Parent::instance() ) {
			Sequence::Parent::create();
		}
		
		Sequence::Parent::instance()->update();
		fps.Update();	//更新
#ifdef _DEBUG
		fps.Draw();		//描画
#endif
		ScreenFlip();
		fps.Wait();		//待機
	}

	Sequence::Parent::instance()->destroy();
	DxLib_End();
	return 0;
}