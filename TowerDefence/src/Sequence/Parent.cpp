#include "define.h"
#include "Sequence\Parent.h"
#include "Sequence\Logo.h"
#include "DxLib.h"
#include "FileNames.h"
#include "XMLNodeNameString.h"
#include "Sequence\CommonFunc.h"
#include "Score.h"
#include "aes_encryption.h"
#include "utils.h"
#include "Configuration.h"
#include <map>
#include <string>

using namespace std;

namespace Sequence{

	Parent* Parent::mInstance = 0;
	bool Parent::isGameEsc = false;

	void Parent::create(){
		ASSERT( !mInstance );
		mInstance = new Parent();
	}

	void Parent::destroy(){
		ASSERT( mInstance );
		SAFE_DELETE( mInstance );
	}

	Parent* Parent::instance(){
		return mInstance;
	}

	Parent::Parent() : 
	mMode( MODE_NONE ),
		mChild( 0 ),
	    mScore( 0 ){
			//最初はロゴ
			mChild = new Logo();
			//実体がなければスコア実体作成
			if ( Score::getInstance() == 0 ) {
				Score::create();
				//復号上書き
				decrypt( FileNames::ScoreDataName.c_str(), PassWord::EncryptPass );
				//読込み
				if ( !Score::getInstance()->readScore(FileNames::ScoreDataName.c_str()) ) {
					/** 読込み失敗時は初期スコアでscore.datを上書きして読み込む */
					int scoreHandle = FileRead_open( FileNames::Text::ScoreDataTemplateName.c_str() );
					ASSERT( Score::getInstance()->readScore( scoreHandle ) );
					FileRead_close( scoreHandle );
				} else {
					//暗号化上書き
					encrypt( FileNames::ScoreDataName.c_str(), PassWord::EncryptPass );
				}
			}
			mScore = Score::getInstance();

			// BGM読み込み
			mBGMHandle = LoadSoundMem( FileNames::Sound::TitleBGMName.c_str() );
			ASSERT( mBGMHandle + 1 );
			
			//決定音読み込み
			mDecisionSEHandle = LoadSoundMem( FileNames::Sound::DecisionSEName.c_str() );
			ASSERT( mDecisionSEHandle );

			{
				using namespace ConfigData;
				//BGMボリューム設定
				int BGMMaxLevel = 
					Configuration::getInstance()->getValue().at( MaxValueNode ).at( BGMLevelStr );
				int BGMSetLevel = 
					Configuration::getInstance()->getValue().at( SetValueNode ).at( BGMLevelStr );
				float BGMRatio = static_cast<float>( BGMSetLevel ) / static_cast<float>( BGMMaxLevel );
				int BGMVolume = static_cast<int>( UCHAR_MAX * BGMRatio );
				ASSERT( ChangeVolumeSoundMem( BGMVolume , mBGMHandle ) + 1 );
				//SEボリューム設定
				int SEMaxLevel = 
					Configuration::getInstance()->getValue().at( MaxValueNode ).at( SELevelStr );
				int SESetLevel = 
					Configuration::getInstance()->getValue().at( SetValueNode ).at( SELevelStr );
				float SERatio = static_cast<float>( SESetLevel ) / static_cast<float>( SEMaxLevel );
				int SEVolume = static_cast<int>( UCHAR_MAX * SERatio );
				ASSERT( ChangeVolumeSoundMem( SEVolume , mDecisionSEHandle ) + 1 );
			}

			/** 共通処理インスタンス作成 */
			CommonFunc::create();
	}

	Parent::~Parent(){
		//残っていれば抹殺
		SAFE_DELETE( mChild );
		if ( CommonFunc::getInstance != 0 ) {
			CommonFunc::destroy();
		}
		ASSERT( DeleteSoundMem( mBGMHandle ) + 1 );
		ASSERT( DeleteSoundMem( mDecisionSEHandle ) + 1 );
	}


	void Parent::update(){
		Base* nextChild = mChild->update( this );
		if ( nextChild != mChild ){
			//遷移判定
			Child* casted = dynamic_cast< Child* >( nextChild );
			ASSERT( casted ); //失敗はありえない。
			SAFE_DELETE( mChild );
			mChild = casted;
		}
		nextChild = 0; //念のため
	}

	void Parent::setMode( Mode mode ){
		mMode = mode;
	}

	Parent::Mode Parent::mode() const {
		return mMode;
	}

} //namespace Sequence
