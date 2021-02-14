#include "define.h"
#include "DxLib.h"
#include "utils.h"
#include "Loader.h"
#include "FileNames.h"
#include "Sequence\GameSequence\Ready.h"
#include "Sequence\GameSequence\Parent.h"
#include "Sequence\GameSequence\InitPlace.h"
#include "XMLNodeNameString.h"
#include "Configuration.h"

using namespace std;
using namespace ConfigData;
namespace Sequence{
	namespace GameSequence{

		Ready::Ready( Parent* parent,
			vector< string >* ImgFileList,
			vector< DivStatus >* DivImgFileList,
			vector< string >* TxtFileList,
			vector< string >* SoundFileList ) {
				/** ロード中画面読み込み */
				mLoadBackImgHandle = LoadGraph( FileNames::Image::LoadBack.c_str() );
				ASSERT( mLoadBackImgHandle + 1 );

				parent->startLoading(ImgFileList,
					DivImgFileList,
					TxtFileList,
					SoundFileList );
		}

		Ready::~Ready(){

		}

		Base* Ready::update( Parent* parent ) {
			Base* next = this;
			DrawGraph(0, 0, mLoadBackImgHandle, TRUE);
			/** 読み込み終了時の処理 */
			if( parent->loader()->CheckFileRead() ){
				/** 音量設定 */
				for ( unsigned int i = 0 ; i < FileNames::Sound::SEFileNameVector.size() ; i++ ) {
					int handle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::SEFileNameVector.at(i) );
					int SEMaxLevel = 
						Configuration::getInstance()->getValue().at( MaxValueNode ).at( SELevelStr );
					int SESetLevel = 
						Configuration::getInstance()->getValue().at( SetValueNode ).at( SELevelStr );
					float SERatio = static_cast<float>( SESetLevel ) / static_cast<float>( SEMaxLevel );
					int SEVolume = static_cast<int>( UCHAR_MAX * SERatio );
					ASSERT( ChangeVolumeSoundMem( SEVolume , handle ) + 1 );
				}

				int handle = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::StageBGMName );
					int BGMMaxLevel = 
						Configuration::getInstance()->getValue().at( MaxValueNode ).at( BGMLevelStr );
					int BGMSetLevel = 
						Configuration::getInstance()->getValue().at( SetValueNode ).at( BGMLevelStr );
					float BGMRatio = static_cast<float>( BGMSetLevel ) / static_cast<float>( BGMMaxLevel );
					int BGMVolume = static_cast<int>( UCHAR_MAX * BGMRatio );
					ASSERT( ChangeVolumeSoundMem( BGMVolume , handle ) + 1 );

				/** 初期状態をセットして本編へ */
				parent->setInitGameState();
				next = new InitPlace( parent );
			}
			return next;
		}

	}
}