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
			//�ŏ��̓��S
			mChild = new Logo();
			//���̂��Ȃ���΃X�R�A���̍쐬
			if ( Score::getInstance() == 0 ) {
				Score::create();
				//�����㏑��
				decrypt( FileNames::ScoreDataName.c_str(), PassWord::EncryptPass );
				//�Ǎ���
				if ( !Score::getInstance()->readScore(FileNames::ScoreDataName.c_str()) ) {
					/** �Ǎ��ݎ��s���͏����X�R�A��score.dat���㏑�����ēǂݍ��� */
					int scoreHandle = FileRead_open( FileNames::Text::ScoreDataTemplateName.c_str() );
					ASSERT( Score::getInstance()->readScore( scoreHandle ) );
					FileRead_close( scoreHandle );
				} else {
					//�Í����㏑��
					encrypt( FileNames::ScoreDataName.c_str(), PassWord::EncryptPass );
				}
			}
			mScore = Score::getInstance();

			// BGM�ǂݍ���
			mBGMHandle = LoadSoundMem( FileNames::Sound::TitleBGMName.c_str() );
			ASSERT( mBGMHandle + 1 );
			
			//���艹�ǂݍ���
			mDecisionSEHandle = LoadSoundMem( FileNames::Sound::DecisionSEName.c_str() );
			ASSERT( mDecisionSEHandle );

			{
				using namespace ConfigData;
				//BGM�{�����[���ݒ�
				int BGMMaxLevel = 
					Configuration::getInstance()->getValue().at( MaxValueNode ).at( BGMLevelStr );
				int BGMSetLevel = 
					Configuration::getInstance()->getValue().at( SetValueNode ).at( BGMLevelStr );
				float BGMRatio = static_cast<float>( BGMSetLevel ) / static_cast<float>( BGMMaxLevel );
				int BGMVolume = static_cast<int>( UCHAR_MAX * BGMRatio );
				ASSERT( ChangeVolumeSoundMem( BGMVolume , mBGMHandle ) + 1 );
				//SE�{�����[���ݒ�
				int SEMaxLevel = 
					Configuration::getInstance()->getValue().at( MaxValueNode ).at( SELevelStr );
				int SESetLevel = 
					Configuration::getInstance()->getValue().at( SetValueNode ).at( SELevelStr );
				float SERatio = static_cast<float>( SESetLevel ) / static_cast<float>( SEMaxLevel );
				int SEVolume = static_cast<int>( UCHAR_MAX * SERatio );
				ASSERT( ChangeVolumeSoundMem( SEVolume , mDecisionSEHandle ) + 1 );
			}

			/** ���ʏ����C���X�^���X�쐬 */
			CommonFunc::create();
	}

	Parent::~Parent(){
		//�c���Ă���Ζ��E
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
			//�J�ڔ���
			Child* casted = dynamic_cast< Child* >( nextChild );
			ASSERT( casted ); //���s�͂��肦�Ȃ��B
			SAFE_DELETE( mChild );
			mChild = casted;
		}
		nextChild = 0; //�O�̂���
	}

	void Parent::setMode( Mode mode ){
		mMode = mode;
	}

	Parent::Mode Parent::mode() const {
		return mMode;
	}

} //namespace Sequence
