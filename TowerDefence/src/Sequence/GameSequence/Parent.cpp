#include "define.h"
#include "Sequence\Parent.h"
#include "Sequence\GameSequence\Parent.h"
#include "Sequence\GameSequence\Ready.h"
#include "Game\State.h"
#include "FileNames.h"
#include "Loader.h"
#include "CommonConst.h"

using namespace std;
using namespace FileNames;

namespace Sequence{
	namespace GameSequence{

		FileLoader* Parent::mLoader = 0;

		Parent::Parent( GrandParent::Mode mode,int StageNum ) :
		mChild( 0 ),
		mState( 0 ),
		mInitImageFileList( 0 ),
		mInitDivImageFileList( 0 ),
		mInitTextFileList( 0 ),
		mInitSoundFileList( 0 ),
		mStageNumber( StageNum ){
			/** �ŏ��͏������� */
			setInitReadFile();

			mChild = new Ready( this, 
				mInitImageFileList,
				mInitDivImageFileList,
				mInitTextFileList,
				mInitSoundFileList );
		}

		Parent::~Parent(){
			SAFE_DELETE( mInitImageFileList );
			SAFE_DELETE( mInitDivImageFileList );
			SAFE_DELETE( mInitTextFileList );
			SAFE_DELETE( mInitSoundFileList );
			SAFE_DELETE( mChild );
			SAFE_DELETE( mState );
			SAFE_DELETE( mLoader );
		}

		/* �㗬���[�h�Ƃ̕ϊ� */
		Parent::Mode Parent::mode() const{
			Mode r = MODE_NONE;
			switch( GrandParent::instance()->mode() ){
			case GrandParent::MODE_PLAY:
				r = MODE_PLAY;
				break;
			default:
				ASSERT(false);
				break;
			}
			return r;
		}

		void Parent::startLoading(
			std::vector< std::string >* ImageFileNameList,
			std::vector< DivStatus >* DivImageFileNameList,
			std::vector< std::string >* TextFileNameList,
			std::vector< std::string >* SoundFileNameList) {
				/** �ǂݍ���ł���Έ�x�j�� */
				if ( mLoader != 0 ) {
					SAFE_DELETE( mLoader );
				}
				mLoader = new FileLoader(
					ImageFileNameList,
					DivImageFileNameList,
					TextFileNameList,
					SoundFileNameList );
		}

		/** ��ԏ����� */
		void Parent::setInitGameState() {
			mState = new Game::State( mLoader, mStageNumber );
		}

		/** �����ǂݍ��݊֘A */
		void Parent::setInitReadFile() {
			setInitImage();
			setInitDivImage();
			setInitText();
			setInitSound();
		}

		void Parent::setInitImage(){
			mInitImageFileList = new vector<string>();
			/** �����G�擾 */
			mInitImageFileList->insert( mInitImageFileList->end(),
				Image::StandImagesVector.begin(), Image::StandImagesVector.end() );
			/** �_�Љ摜�擾 */
			mInitImageFileList->push_back( Image::TempleImg );
			/** �U���͈͉摜�擾 */
			mInitImageFileList->push_back( Image::RangeImg );
			/** ���p�A�C�R���擾 */
			mInitImageFileList->push_back( Image::SaleIconImage );
			/** �o���A�C�R�� */
			mInitImageFileList->push_back( Image::AwakeIconImage );
			/** �A�C�R���I��p�J�[�\�� */
			mInitImageFileList->push_back( Image::CharaBuildCursor );
			/** �󋵕\���o�b�N */
			mInitImageFileList->push_back( Image::StateBackImage );
			/** �m�F�o�b�N */
			mInitImageFileList->push_back( Image::ConfirmBackImage );
			/** �X�R�A�{�[�h */
			mInitImageFileList->push_back( Image::ScoreBoardImage );
			/** �I�����b�Z�[�W */
			mInitImageFileList->push_back( Image::EndMessageImage );
		}

		void Parent::setInitDivImage(){
			
			mInitDivImageFileList = new vector<DivStatus>();
			
			/** �L�����h�b�g�G�擾 */
			for ( unsigned int i = 0; i < Image::DotImagesVector.size(); i++ ) {
				mInitDivImageFileList->push_back(
					DivStatus(
					Image::DotImagesVector.at(i).c_str() ,
					CommonConst::CharaDotImgDivNumAll,
					CommonConst::CharaDotImgDivNumX,
					CommonConst::CharaDotImgDivNumY,
					CommonConst::CharaDotImgSizeX,
					CommonConst::CharaDotImgSizeY  ) );
			}

			/** �G�L�����h�b�g�G�擾 */
			for ( unsigned int i = 0; i < Image::EnemyDotImageVector.size(); i++ ) {
				mInitDivImageFileList->push_back(
					DivStatus(
					Image::EnemyDotImageVector.at(i).c_str(),
					CommonConst::EnemyDotImgDivNumAll,
					CommonConst::EnemyDotImgDivNumX,
					CommonConst::EnemyDotImgDivNumY,
					CommonConst::EnemyDotImgSizeXVector.at(i),
					CommonConst::EnemyDotImgSizeYVector.at(i) ) );
			}

			/** �}�b�v�`�b�v�擾 */
			mInitDivImageFileList->push_back(
				DivStatus(
				Image::MapChipImg.c_str(),
				CommonConst::MapChipDivNumAll,
				CommonConst::MapChipDivNumX,
				CommonConst::MapChipDivNumY,
				CommonConst::MapChipSize,
				CommonConst::MapChipSize ) );
			
			/** ���j�G�t�F�N�g */
			mInitDivImageFileList->push_back(
				DivStatus(
				Image::ExplosionImage.c_str(),
				CommonConst::ExplosionDivNumAll,
				CommonConst::ExplosionDivNumX,
				CommonConst::ExplosionDivNumY,
				CommonConst::ExplosionSizeX,
				CommonConst::ExplosionSizeY ) );

			/** �e�摜 */
			mInitDivImageFileList->push_back(
				DivStatus(
				Image::ShotImage.c_str(),
				CommonConst::ShotDivNumAll,
				CommonConst::ShotDivNumX,
				CommonConst::ShotDivNumY,
				CommonConst::ShotSizeX,
				CommonConst::ShotSizeY ) );
		
			/** �Ԏ������A�C�R���摜 */
			mInitDivImageFileList->push_back(
				DivStatus(
				Image::EconomicIconImage.c_str(),
				CommonConst::EconomicIconDivNumAll,
				CommonConst::EconomicIconDivNumX,
				CommonConst::EconomicIconDivNumY,
				CommonConst::EconomicIconDivSizeX,
				CommonConst::EconomicIconDivSizeY ) );

			/** ���s�A�C�R�� */
			mInitDivImageFileList->push_back(
				DivStatus(
				Image::ResultLogoImage.c_str(),
				CommonConst::ResultLogoDivNumAll,
				CommonConst::ResultLogoDivNumX,
				CommonConst::ResultLogoDivNumY,
				CommonConst::ResultLogoDivSizeX,
				CommonConst::ResultLogoDivSizeY	) );

		}

		void Parent::setInitText(){
			mInitTextFileList = new vector<string>();
			mInitTextFileList->push_back( Text::MapInfoFileVector.at( mStageNumber ) );
			mInitTextFileList->push_back( Text::EnemyAppearanceVector.at( mStageNumber ) );
			mInitTextFileList->push_back( Text::EnemyStatusData );
		}

		void Parent::setInitSound(){
			mInitSoundFileList = new vector<string>();
			mInitSoundFileList->push_back( Sound::ExplosionSEName );
			mInitSoundFileList->push_back( Sound::ShotSEName );
			mInitSoundFileList->push_back( Sound::StageBGMName );
			mInitSoundFileList->push_back( Sound::DecisionSEName );
		}


		Base* Parent::update( GrandParent* parent){
			Base* next = this;
			Base* nextChild = mChild->update( this );
			/* �J�ڔ��� */
			if ( nextChild != mChild ) {
				GameSequence::Child* casted = dynamic_cast< GameSequence::Child* >( nextChild );
				if ( casted ) {
					SAFE_DELETE( mChild );
					mChild = casted;
				} else {
					next = nextChild;
				}
				casted = 0;
			}
			
			nextChild = 0;
			return next;
		}
	}
}