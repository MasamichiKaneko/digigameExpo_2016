#include "Game\State.h"
#include "Charactor\Friend\FriendBase.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "Charactor\Enemy\Ocj.h"
#include "Game\StageMap.h"
#include "Game\CharaStatusReader.h"
#include "Game\EnemyStatusReader.h"
#include "Charactor\Friend\Plane.h"
#include "Charactor\Friend\Miko.h"
#include "Charactor\Friend\Majokko.h"
#include "Charactor\Friend\Yojo.h"
#include "Effect\Explosion.h"
#include "Effect\Shot.h"
#include "StageConfig.h"
#include "Loader.h"
#include "FileNames.h"
#include "DxLib.h"
#include "Configuration.h"
#include "utils.h"
#include "define.h"
#include "XMLNodeNameString.h"
#include "aes_encryption.h"
#include <algorithm>
#include "Sequence\CommonFunc.h"
#include "CommonConst.h"
#include "CommonString.h"
#include <sstream>
#include <boost\lexical_cast.hpp>

using namespace std;
using namespace FileNames;
using namespace Charactor;

namespace Game{

	/** ��~�E�Đ��{�^���`��ʒu */
	const int State::PlayStopButtonPosX = 0;
	const int State::PlayStopButtonPosY = 0;

	/** �G�L����HP�\���c�� */
	const int State::GaugeDrawHeight = 5;

	/** �A�C�R���`��ʒu */
	const int State::CharaIconDrawX = 480;
	const int State::CharaIconDrawY = 0;

	/** �A�C�R���`��Ԋu */
	const int State::CharaIconDrawIntervalX = CommonConst::CharaDotImgSizeX;
	const int State::CharaIconDrawIntervalY = CommonConst::CharaDotImgSizeY;

	/** �󋵕`��ʒu */
	const int State::AssetDrawPosX = 500;
	const int State::AssetDrawPosY = 290;
	const int State::ScoreDrawPosX = 500;
	const int State::ScoreDrawPosY = 330;
	const int State::EconomicDrawPosX = 530;
	const int State::EconomicDrawPosY = 235;

	/** �X�e�[�^�X�`��ʒu */
	const int State::StatusDrawPosX = 575;
	const int State::PowerValueDrawPosY = 120;
	const int State::SpeedValueDrawPosY = 140;
	const int State::RangeValueDrawPosY = 160;
	const int State::CostValueDrawPosY = 180;

	/** �󋵕\���J�n�ʒu */
	const int State::StateDrawPosX = 480;
	const int State::StateDrawPosY = 0;

	/** �Q�[�W�J���[ */
	const int State::EnemyHPGaugeColor     = GetColor( 0, UCHAR_MAX, 0);
	const int State::FriendSkillGaugeColor = GetColor( UCHAR_MAX, 0 ,0 );
	const int State::GaugeBackColor        = GetColor( 0, 0, 0 );
	const int State::GaugeFrameColor       = GetColor( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );

	/** �s���Ƃ̃L�����A�C�R���`�搔 */
	const int State::CharaIconDrawNumRow = 4;

	/** �L�����`��ʒu�␳ */
	const int State::FriendDrawCorr = -4;

	/** �O���b�h�J���[ */
	const int State::GridColor = GetColor( 0, 0, 0 );
	
	/** �t�H���g�֘A */
	const int State::FontSize = 16;
	const char* State::FontStyle = "HGS�s����";
	const int State::StringColor = GetColor( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );

	/** �\���������� */
	const int State::AssetNumOfDisits = 8;
	const int State::ScoreNumOfDisits = 8;

	/** �L�������W��r�֐�(���בւ��p) */
	bool CharactorPosCompare( const CharactorBase* right, const CharactorBase* left ){
		
		bool result = true;
		if ( right->getPoint().getY() != left->getPoint().getY() ) {
			result = right->getPoint().getY() < left->getPoint().getY();
		} else {
			result = right->getPoint().getX() < left->getPoint().getX();
		}
		
		return result;
	}

	/** �����G�L�����T���p */
	bool FindSurviveEnemy( const Enemy::EnemyBase* enemy ) {
		return enemy->getSurvive();
	}

	/** �o�����G�L�����T���p */
	bool FindAppearanceEnemy( const Enemy::EnemyBase* enemy ){
		return enemy->getAppearance();
	}

	/** �`��ΏۓG�L�����T���p */
	bool FindDrawTargetEnemy( const Enemy::EnemyBase* enemy ){
		return enemy->getSurvive() && enemy->getAppearance();
	}

	/** �ǂݍ��݃f�[�^���擾 */
	State::State( FileLoader* loader , int stageNum) : 
	mGameOver( false ), mStageMap( 0 ), mFriendData( 0 ), mScore( 0 ), mAssets( 0 ), mInitAssets( 0 ), mStageNum( 0 ){

		/** �X�e�[�W���擾 */
		mStageNum = stageNum;

		/** ���L�����X�e�[�^�X�f�[�^�ǂݍ��� */
		mFriendData = new CharaStatusReader();
		decrypt( FileNames::CharaStatus.c_str(), PassWord::EncryptPass );
		//ASSERT( mFriendData->readExpTable( FileNames::Text::ExpTableDataName.c_str() ) );
		if( !mFriendData->readCharaData( FileNames::CharaStatus.c_str() ) ){
			/** ���s�Ȃ�e���v���ǂݍ��� */
			int statusHandle = FileRead_open( FileNames::Text::CharaDataTemplateName.c_str() );
			vector<string> readData;
			FileRead_getAll( &readData, statusHandle );
			FileWrite_vectorAll(readData,FileNames::CharaStatus.c_str());
			ASSERT( mFriendData->readCharaData( FileNames::Text::CharaDataTemplateName.c_str() ) );
		}
		encrypt( FileNames::CharaStatus.c_str(), PassWord::EncryptPass );

		/** �L�����X�e�[�^�X�擾 */
		vector<Game::CharaStatusReader::Status>::iterator statusIt = mFriendData->getStatusValue()->begin();
		for ( ; statusIt != mFriendData->getStatusValue()->end() ; statusIt++ ) {
			mStatusValues[ statusIt->id ] = statusIt->TotalValue;
		}

		/** �}�b�v�f�[�^�ǂݍ��� */
		mStageMap = new StageMap( loader->getTextFileHandle()->at(Text::MapInfoFileVector.at( stageNum )) );
		/** �G�L�����X�e�[�^�X�ǂݍ��� */
		mEnemyData = new EnemyStatusReader( loader->getTextFileHandle()->at( Text::EnemyStatusData ),
			loader->getTextFileHandle()->at( Text::EnemyAppearanceVector.at( stageNum ) ), mStageMap );
		/** �p�ς݂̃n���h���J�� */
		loader->deleteTextHandleAll();

		/** �G�L�����o�����ǂݍ��� */
		mEnemys = mEnemyData->getEnemysData();

		/** ���ΑK�z�󂯎�� */
		mInitAssets = StageConfig::getInstance()->getConfigValue()->at( stageNum ).initAsset;
		mAssets = mInitAssets;

		/** �n���h���󂯎�� */
		mMapChip = loader->getDivImageFileHanlde()->at( Image::MapChipImg );
		mTempleHandle = loader->getImageFileHandle()->at( Image::TempleImg );
		mRangeHandle = loader->getImageFileHandle()->at( Image::RangeImg );
		mSelectionFrameHandle = loader->getImageFileHandle()->at( Image::CharaBuildCursor );
		mStateBackHandle = loader->getImageFileHandle()->at( Image::StateBackImage );
		mEconomicIconHandle = loader->getDivImageFileHanlde()->at( Image::EconomicIconImage );

		/** �t�H���g�n���h���ݒ� */
		mFontHandle = CreateFontToHandle( FontStyle, FontSize, -1, DX_FONTTYPE_ANTIALIASING);

		/** ���L�����h�b�g�G */
		for ( unsigned int i = 0; i < FileNames::Image::DotImagesVector.size(); i++ ) {
			string imageName = FileNames::Image::DotImagesVector.at( i );
			/** �h�b�g�G�n���h���擾 */
			mCharaIconHandles[imageName] = loader->getDivImageFileHanlde()->at(imageName);
			/** �I��p�L�����A�C�R���ɒǉ� */
			mCharaSelectIconHandles.push_back( mCharaIconHandles.at(imageName).at( 0 ) );
		}
		/** ���p�A�C�R���ǉ� */
		mCharaSelectIconHandles.push_back( loader->getImageFileHandle()->at( FileNames::Image::SaleIconImage ) ); 
		/** �o���A�C�R���ǉ� */
		mCharaSelectIconHandles.push_back( loader->getImageFileHandle()->at( FileNames::Image::AwakeIconImage ) );
		/** �`��ʒu�v�Z */
		for ( unsigned int i = 0; i < mCharaSelectIconHandles.size(); i++ ) {
			/** �A�C�R���擪�n���h�����󂯎�� */
			int posX = CharaIconDrawX + ( ( i % CharaIconDrawNumRow ) * CharaIconDrawIntervalX );
			int posY = CharaIconDrawY + ( ( i / CharaIconDrawNumRow ) * CharaIconDrawIntervalY );
			mIconDrawPos.push_back( Point( posX, posY ) );
		}

		/** �G�L�����h�b�g�G */
		for ( unsigned int i = 0; i < FileNames::Image::EnemyDotImageVector.size(); i++ ) {
			string imageName = FileNames::Image::EnemyDotImageVector.at( i );
			/** �h�b�g�G�n���h���󂯎�� */
			mEnemyIconHandles[imageName] = loader->getDivImageFileHanlde()->at(imageName);
		}

		/** �G�t�F�N�g�摜�擾 */
		for ( unsigned int i = 0; i < FileNames::Image::EffectImagesVector.size(); i++ ) {
			string imageName = FileNames::Image::EffectImagesVector.at( i );
			/** �n���h���󂯎�� */
			mEffectImageHandles[imageName] = loader->getDivImageFileHanlde()->at(imageName);
		}

		/** �����n���h���󂯎�� */
		mExplosionSEHandle = loader->getSoundFileHandle()->at( FileNames::Sound::ExplosionSEName );

		/** �ŏ��͑I�𖳂��̏�� */
		mSelection = Plane;

		/** �O���b�h�Z�x�v�Z */
		int maxValue = Configuration::getInstance()->getValue().at(ConfigData::MaxValueNode).at(ConfigData::GridLevelStr);
		int setValue = Configuration::getInstance()->getValue().at(ConfigData::SetValueNode).at(ConfigData::GridLevelStr);
		float ratio  =  static_cast<float>( setValue ) / static_cast<float>(maxValue);
		mGridDipth   = static_cast<int>( UCHAR_MAX * ratio );
	}

	State::~State(){
		SAFE_DELETE( mFriendData );
		SAFE_DELETE( mEnemyData  );
		SAFE_DELETE( mStageMap   );
	}

	void State::update(){

		/** ���L�������� */
		for ( unsigned int i = 0; i < mFriends.size(); i++ ) {
			mFriends.at(i)->update( &mEnemys, &mFriends, mStageMap );
		}

		/** �|�����G�L�����폜 */
		deleteDeadEnemys();

		/** �G�L�������� */
		for ( unsigned int i = 0; i < mEnemys.size(); i++ ) {
			mEnemys.at(i)->update( &mEnemys, &mFriends, mStageMap );
		}

		/** ���B�G�L�����폜 */
		deleteArrivalEnemys();

		/** �G�t�F�N�g�X�V */
		for ( unsigned int i = 0; i < mEnemyExplosions.size() ; i++ ) {
			mEnemyExplosions.at(i)->update();
		}

		deleteEnemyExplosions();

		/** �Q�[���I�[�o�[���� */
		/** �������s���邩�G�S�łŏI�� */
		if ( mAssets <= 0 || mEnemys.size() == 0 ) {
			mGameOver = true;
		}
	}

	/** ���ꂽ�G�폜 */
	void State::deleteDeadEnemys(){
		vector<Enemy::EnemyBase*>::iterator EnemyIt;
		EnemyIt = mEnemys.begin();
		for ( ; EnemyIt != mEnemys.end() ; ) {
			/** ����������HP0�ȉ��̓G�������� */
			if ( (*EnemyIt)->getHitPoint() <= 0 && (*EnemyIt)->getSurvive() ) {
				/** �X�R�A�ǉ� */
				mScore += (*EnemyIt)->getScore();
				/** �����ǉ� */
				mAssets += (*EnemyIt)->getAssets();
				/** �����t���O��� */
				(*EnemyIt)->setSurvive( false );
				/** ���j�G�t�F�N�g�Z�b�g */
				int size = CommonConst::EnemyDotImgSizeXVector.at( (*EnemyIt)->getId() );
				Effect::Explosion* effect = new Effect::Explosion( (*EnemyIt)->getPoint() ,1 , size);
				mEnemyExplosions.push_back( effect );
				/** �������Đ� */
				ASSERT( PlaySoundMem( mExplosionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
				EnemyIt = mEnemys.erase( EnemyIt );
			} else {
				EnemyIt++;
			}
		}
	}

	/** ���B�����G�폜 */
	void State::deleteArrivalEnemys(){
		vector<Enemy::EnemyBase*>::iterator EnemyIt;
		EnemyIt = mEnemys.begin();
		for ( ; EnemyIt != mEnemys.end(); ) {
			/** �������̓G�����B���Ă����� */
			if ( (*EnemyIt)->getDistanceToGoal() == 0 && (*EnemyIt)->getSurvive() ) {
				/** ���������� */
				mAssets -= (*EnemyIt)->getAssetDamage();
				if ( mAssets < 0 ) {
					mAssets = 0;
				}
				/** �����t���O��� */
				(*EnemyIt)->setSurvive( false );
				EnemyIt = mEnemys.erase( EnemyIt );
			} else {
				EnemyIt++;
			}
		}
	}

	/** �����G�t�F�N�g�폜 */
	void State::deleteEnemyExplosions(){
		vector<Effect::Explosion*>::iterator It;
		It = mEnemyExplosions.begin();
		for ( ;It != mEnemyExplosions.end(); ) {
			if ( !(*It)->getIsDraw() ) {
				It = mEnemyExplosions.erase( It );
			} else {
				It++;
			}
		}
	}

	/** ���L�����z�u�֐� */
	Friend::FriendBase* State::getAddFriend( int posX, int posY ) const{

		int sizeX = mStageMap->getMapSizeX();
		int sizeY = mStageMap->getMapSizeY();

		Friend::FriendBase* retFriend = NULL;

		Point p(posX, posY);

		/** id�`�F�b�N */
		vector< CharaStatusReader::Status >::iterator it;
		it = find(mFriendData->getStatusValue()->begin(),mFriendData->getStatusValue()->end(), mSelection);
		
		/** �L�����f�[�^����󂯎��e�l */
		int necessaryCost, skill, power, speed, range, level;

		/** ���݂���id�Ȃ�L�����̊e�ݒ�l���擾 */
		if ( it != mFriendData->getStatusValue()->end() ) {
			necessaryCost = it->TotalValue.at( CharaDataXml::CostStr );
			skill = it->TotalValue.at( CharaDataXml::SkillStr );
			power = it->TotalValue.at( CharaDataXml::PowerStr );
			speed = it->TotalValue.at( CharaDataXml::SpeedStr );
			range = it->TotalValue.at( CharaDataXml::RangeStr );
			level = it->lv;
		} else {
			/** ���݂��Ȃ�id�Ȃ�u���Ȃ� */
			goto FUNC_END;
		}

		/** ���Y������Ȃ���Βu���Ȃ� */
		if ( mAssets <= necessaryCost ) {
			goto FUNC_END;
		}
		
		/** �}�b�v�`�F�b�N */
		/** �w����W���}�b�v�̑傫���ȓ����`�F�b�N */
		bool coordinateCheck = true;
		coordinateCheck &= ( ( 0 <= posX ) && ( posX < sizeX ));
		coordinateCheck &= ( ( 0 <= posY ) && ( posY < sizeY ));
		if ( !coordinateCheck ) {
			goto FUNC_END;
		}

		/** �z�u�\�ȃ}�b�v�`�b�vid���`�F�b�N */
		if ( mStageMap->getMapInfo( posX, posY ).getId() != PlaneChip ) {
			goto FUNC_END;
		}

		/** ���L�������u����Ă��Ȃ����`�F�b�N */
		if ( mStageMap->getMapInfo( posX, posY ).getIsOnCharactor() ) {
			goto FUNC_END;
		}

		/** ���W�Œu���ʒu���擾 */
		p.setX( p.getX() * mStageMap->getChipSize() );
		p.setY( p.getY() * mStageMap->getChipSize() );

		/** �U���͈͕␳ */
		/** �����Ă���}�X����ǉ� */
		range += static_cast< int >( CommonConst::RangeBasicUnit / 2 );
		/** ��{�P�ʂ��Ƃ�1�}�X���ɒ��߂��� */
		range = static_cast< int >( range * ( static_cast< double >( CommonConst::MapChipSize ) / CommonConst::RangeBasicUnit ) );

		/** ���L������ǉ� */
		switch( mSelection ){
		case Plane:
			retFriend = new Friend::Plane(skill, power, speed, range, mSelection, level, p);
			break;
		case Miko:
			retFriend = new Friend::Miko(skill, power, speed, range, mSelection, level, p);
			break;
		case Majokko:
			retFriend = new Friend::Majokko(skill, power, speed, range, mSelection, level, p);
			break;
		case Yojo:
			retFriend = new Friend::Yojo(skill, power, speed, range, mSelection, level, p);
			break;

		case Non:
		default:
			goto FUNC_END;
			break;
		}

FUNC_END:
		return retFriend;
	}

	/** ���L�������p�֐� */
	bool State::mSaleFriend( int posX, int posY ){
		bool result = false;
		vector < Friend::FriendBase* >::iterator friendIt;
		/** �}�b�v��w��}�X�ɃL�������u����Ă��邩�̔��� */
		if ( !mStageMap->getMapInfo(posX, posY).getIsOnCharactor() ) {
			/** �u����Ă��Ȃ���Ή��������֐������I�� */
			goto FUNC_END;
		}

		/** ���W�ƈ�v����L������T�� */
		for ( friendIt = mFriends.begin(); friendIt != mFriends.end(); friendIt++ ) {
			/** ���W����v�Ȃ烋�[�v�𔲂��� */
			if ( ( (*friendIt)->getPoint().getX() == ( posX * CommonConst::MapChipSize ) )
				&& ( (*friendIt)->getPoint().getY() == ( posY * CommonConst::MapChipSize ) ) ) {
				break;
			}
		}

		/** ���p���� */
		if ( friendIt != mFriends.end() ) {
			/** �R�X�g�̔����������ɊҌ� */
			int cost = mFriendData->getStatusValue()->at( (*friendIt)->getId() ).TotalValue.at( CharaDataXml::CostStr );
			mAssets += (cost / 2);
			/** �w��}�X���L�������u����Ă��Ȃ���Ԃɂ��� */
			mStageMap->setOnFriendFlag(posX, posY, false);
			mFriends.erase( friendIt );
			result = false;
		}

FUNC_END:
		return result;
	}

	/** ���L�����o���֐� */
	bool State::mAwakeFriend( int posX, int posY ){
		bool result = false;
		vector < Friend::FriendBase* >::iterator friendIt;
		/** �}�b�v��w��}�X�ɃL�������u����Ă��邩�̔��� */
		if ( !mStageMap->getMapInfo(posX, posY).getIsOnCharactor() ) {
			/** �u����Ă��Ȃ���Ή��������֐������I�� */
			goto FUNC_END;
		}

		/** ���W�ƈ�v����L������T�� */
		for ( friendIt = mFriends.begin(); friendIt != mFriends.end(); friendIt++ ) {
			/** ���W����v�Ȃ烋�[�v�𔲂��� */
			if ( ( (*friendIt)->getPoint().getX() == ( posX * CommonConst::MapChipSize ) )
				&& ( (*friendIt)->getPoint().getY() == ( posY * CommonConst::MapChipSize ) ) ) {
				break;
			}
		}

		/** �o���t���O���Z�b�g */
		if ( friendIt != mFriends.end() ) {
			if ( ( (*friendIt)->getGauge() == (*friendIt)->getGaugeMax() ) && !(*friendIt)->getIsAwake() ) {
				(*friendIt)->setIsAwake( true );
			}
		}

FUNC_END:
		return result;
	}

	/** �S�̂̕`�� */
	void State::draw( bool selectionDraw ) const{
		drawMap( selectionDraw );
		drawFriendsRange();
		
		
		/** �L�����`�� */
		drawCharactors( selectionDraw );

		/** �G�t�F�N�g�`�� */
		if ( mEnemyExplosions.size() > 0 ) {
			vector< Effect::EffectBase* > effect;
			effect.insert( effect.begin(), mEnemyExplosions.begin(), mEnemyExplosions.end() );
			drawEffect( effect );
		}
		
		for ( unsigned int i = 0 ; i < mFriends.size(); i++ ) {
			if ( mFriends.at(i)->getEffect().size() > 0 ) {
				drawEffect( mFriends.at(i)->getEffect() );
			}
		}
		/** �Q�[���󋵕`�� */
		drawState();
		drawCharaIcon( selectionDraw );
	}

	/** �}�b�v�`�� */
	void State::drawMap( bool selectionDraw ) const{
		int mapSizeY  = mStageMap->getMapSizeY();
		int mapSizeX  = mStageMap->getMapSizeX();
		int chipSizeX = mStageMap->getChipSize();
		int chipSizeY = mStageMap->getChipSize();

		/** �`�b�v�`�� */
		for ( int y = 0 ; y < mapSizeY; y++ ){
			int posY = y *( mStageMap->getChipSize() );
			for ( int x = 0; x < mapSizeX; x++ ) {
				int posX = x * mStageMap->getChipSize();
				int mapChipId = mStageMap->getMapInfo( x, y ).getId();
				DrawGraph( posX, posY, mMapChip.at(mapChipId), TRUE );
			}
		}
		
		/** �}�E�X�ʒu�擾 */
		int mousePosX, mousePosY;
		ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );
		

		/** �}�E�X���}�b�v��ɂ���ꍇ�͋P�x���グ�� */
		bool mousePointOnMap = isPointOnMap( mousePosX, mousePosY );

		if( mousePointOnMap && selectionDraw ) {
			/** �}�b�v�`�b�v�̔ԍ��v�Z */
			int mapChipX = mousePosX / mStageMap->getChipSize();
			int mapChipY = mousePosY / mStageMap->getChipSize();
			int posX = mapChipX * mStageMap->getChipSize();
			int posY = mapChipY * mStageMap->getChipSize();
			SetDrawBlendMode( DX_BLENDMODE_ADD,  CHAR_MAX / 2 );
			DrawBox( posX ,posY
				, posX + mStageMap->getChipSize()
				, posY + mStageMap->getChipSize()
				, GetColor( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX )
				, TRUE);
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );
		}

		/** �_�Е`�� */
		DrawGraph( 0, 0, mTempleHandle , TRUE );

		/** �O���b�h�`�� */
		ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,mGridDipth ) + 1 );
		drawGrid( 0, 0, mapSizeY + 1, mapSizeX + 1, chipSizeY , chipSizeX, GridColor );
		ASSERT( SetDrawBlendMode( DX_BLENDMODE_NOBLEND ,UCHAR_MAX ) + 1 );
	}

	/** ���L�����U���͈͕`�� */
	void State::drawFriendsRange() const{

		/** �U���͈͕`�� */
		for ( unsigned int i = 0; i < mFriends.size(); i++ ) {
			/** �͈͔{���v�Z */
			int distance = mFriends.at(i)->getRange();
			/** �`�撸�_�ʒu�v�Z */

			int upperLeftX  = mFriends.at(i)->getPoint().getX() + (mStageMap->getChipSize() / 2);
			int upperLeftY  = mFriends.at(i)->getPoint().getY() + (mStageMap->getChipSize() / 2);
			int lowerRightX = mFriends.at(i)->getPoint().getX() + (mStageMap->getChipSize() / 2);
			int lowerRightY = mFriends.at(i)->getPoint().getY() + (mStageMap->getChipSize() / 2);
			upperLeftX  -= distance;
			upperLeftY  -= distance;
			lowerRightX += distance;
			lowerRightY += distance;
			/** �`�� */
			ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX / 10 ) + 1 );
			ASSERT( DrawExtendGraph( upperLeftX, upperLeftY, lowerRightX, lowerRightY, mRangeHandle, TRUE ) + 1 );
			ASSERT( SetDrawBlendMode( DX_BLENDMODE_NOBLEND ,UCHAR_MAX ) + 1 );

		}

	}

	/** �L�����`�� */
	void State::drawCharactors( bool selectionDraw ) const{

		/** �L�����N�^�z���p�� */
		vector< CharactorBase* > charactors = getDrawTargets();
		
		/** �I�𒆂̃L���������ă}�E�X�|�C���^���}�b�v��ɂ���΃}�b�v��ɕ\�������邽�߂ɒǉ� */
		int mousePosX, mousePosY;
		ASSERT( GetMousePoint( &mousePosX, &mousePosY ) + 1 );
		int throughFriendPosX = -1;
		int throughFriendPosY = -1;
		if ( isPointOnMap( mousePosX, mousePosY ) && selectionDraw ) {
			int mapPosX = mousePosX / mStageMap->getChipSize();
			int mapPosY = mousePosY / mStageMap->getChipSize();
			Friend::FriendBase* addFriend = getAddFriend( mapPosX, mapPosY );
			if ( addFriend != NULL ) {
				charactors.push_back( addFriend );
				throughFriendPosX = addFriend->getPoint().getX();
				throughFriendPosY = addFriend->getPoint().getY();
			}
		}

		/** ���W���Ƀ\�[�g */
		sort( charactors.begin(), charactors.end(), CharactorPosCompare );

		/** �`�� */
		for ( vector< CharactorBase* >::iterator it = charactors.begin(); it != charactors.end() ;it++ ) {
			
			int handle = 0;
			int id = (*it)->getId();
			int motion = (*it)->getMotionId();
			int gaugeColor = 0;
			/** id�ƃL�����^�C�v����`�悷��n���h���擾 */
			try{
				if ( (*it)->getCharactorType() == Friend::CharactorType ) {
					handle = mCharaIconHandles.at( FileNames::Image::DotImagesVector.at(id) ).at(motion);
					gaugeColor = FriendSkillGaugeColor;
				} else if( (*it)->getCharactorType() == Enemy::CharactorType ) {
					handle = mEnemyIconHandles.at( FileNames::Image::EnemyDotImageVector.at(id) ).at(motion);
					gaugeColor = EnemyHPGaugeColor;
				} else {
					ASSERT( false );
				}
				/** �`��ʒu�v�Z */
				int drawPosX = (*it)->getPoint().getX();
				int drawPosY = (*it)->getPoint().getY();
				int sizeX, sizeY;
				ASSERT( GetGraphSize( handle, &sizeX, &sizeY ) + 1 );
				drawPosX -= ( ( sizeX - mStageMap->getChipSize() ) / 2 );
				drawPosY -= ( ( sizeY - mStageMap->getChipSize() ) );
				drawPosY += FriendDrawCorr;
				/** �L�����`�� */
				if ( throughFriendPosX == (*it)->getPoint().getX() && throughFriendPosY == (*it)->getPoint().getY()
					&& (*it)->getCharactorType() == Friend::CharactorType ) {
						SetDrawBlendMode( DX_BLENDMODE_ALPHA, UCHAR_MAX/3 );
						ASSERT( DrawGraph( drawPosX, drawPosY, handle, TRUE ) + 1 );
						SetDrawBlendMode( DX_BLENDMODE_ALPHA, UCHAR_MAX );
				} else {
					ASSERT( DrawGraph( drawPosX, drawPosY, handle, TRUE ) + 1 );
					/** �Q�[�W�`�� */
					drawPosX = (*it)->getPoint().getX();
					int drawPosLowerX = drawPosX + mStageMap->getChipSize();
					int drawPosLowerY = drawPosY;
					int drawPosUpperY = drawPosY - GaugeDrawHeight;
					double gaugePerce = static_cast< double >( (*it)->getGauge() );
					gaugePerce /= static_cast< double >( (*it)->getGaugeMax() );
					int geugeValuePos = drawPosX + static_cast< int >( mStageMap->getChipSize() * gaugePerce );
					ASSERT( DrawBox( drawPosX, drawPosUpperY, drawPosLowerX, drawPosLowerY, GaugeBackColor, TRUE ) + 1 );
					if ( (*it)->getCharactorType() == Friend::CharactorType && ( (*it)->getGauge() != (*it)->getGaugeMax() ) ) {
						int value = static_cast< int >( UCHAR_MAX * 0.8 );
						ASSERT( SetDrawBright( value, value ,value ) + 1 );
					}
					ASSERT( DrawBox( drawPosX, drawPosUpperY, geugeValuePos, drawPosLowerY, gaugeColor, TRUE ) + 1 );
					if ( (*it)->getCharactorType() == Friend::CharactorType && ( (*it)->getGauge() != (*it)->getGaugeMax() ) ) {
						ASSERT( SetDrawBright( UCHAR_MAX, UCHAR_MAX ,UCHAR_MAX ) + 1 );
					}
					ASSERT( DrawBox( drawPosX, drawPosUpperY, drawPosLowerX, drawPosLowerY, GaugeFrameColor, FALSE ) + 1 );
				}
			} catch( out_of_range& ) {
				/** TODO:�����s���̂��߁A�b��Ή� */
			}
#ifdef _DEBUG
			ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX / 5 ) + 1 );
			DrawCircle( (*it)->getPoint().getX(), (*it)->getPoint().getY(), (*it)->getRange() , GetColor(UCHAR_MAX, UCHAR_MAX, 0));
			ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX ) + 1 );
#endif
			}
		
	}

	/** �`��Ώۂ̓G�擾�֐� */
	vector< CharactorBase* > State::getDrawTargets() const{
	//	try{
		vector< CharactorBase* > charactors;
		unsigned int drawEnemyNum = count_if( mEnemys.begin(), mEnemys.end(), FindDrawTargetEnemy );
		charactors.reserve( drawEnemyNum );
		/** �`��Ώۂ̓G�L�����擾 */
		vector< Enemy::EnemyBase* >::const_iterator targetEnemy = mEnemys.begin();
		for (;;) {
			targetEnemy = find_if( targetEnemy, mEnemys.end(), FindDrawTargetEnemy );

			if ( targetEnemy == mEnemys.end() ) {
				break;
			} else {
				charactors.push_back( *targetEnemy );
				targetEnemy++;
			}
		}

		/** �Ώۂ̎��L�����擾 */
		charactors.insert(charactors.begin(), mFriends.begin(), mFriends.end());
		return charactors;
		/*} catch( out_of_range& e ) {
			e;
		}*/
	}

	/** TODO:�󋵕`�� */
	void State::drawState() const{

		/** �o�b�N�\�� */
		DrawGraph( StateDrawPosX, StateDrawPosY, mStateBackHandle, TRUE );

		/** �I�𒆂̃L�����X�e�[�^�X�`�� */
		stringstream powerStr;
		stringstream speedStr;
		stringstream rangeStr;
		stringstream costStr;
		switch( mSelection ) {
		case Plane:
		case Miko:
		case Majokko:
		case Yojo:
			{
				/** �X�e�[�^�X�l�𕶎���Ŏ擾 */
				vector< CharaStatusReader::Status >::iterator it;
				it = find(mFriendData->getStatusValue()->begin(),mFriendData->getStatusValue()->end(), mSelection);
				powerStr << boost::lexical_cast<string>( it->TotalValue.at( CharaDataXml::PowerStr ) );
				double speedValue = static_cast<double>( it->TotalValue.at( CharaDataXml::SpeedStr ) );
				speedValue /= static_cast<double>( Configuration::getInstance()->getFrameRate() );
				speedStr << fixed << setprecision(2) << speedValue << CommonStr::Common::SecondStr;
				double rangeValue = static_cast<double>( it->TotalValue.at( CharaDataXml::RangeStr ) );
				rangeValue /= static_cast<double>( CommonConst::RangeBasicUnit );
				rangeStr << fixed << setprecision(2) << rangeValue << CommonStr::Common::TroutStr;
				costStr << it->TotalValue.at( CharaDataXml::CostStr );
			}
			break;
		case Sale:
		case Awake:
		case Non:
			powerStr << CommonStr::Status::NoStatusStr;
			speedStr << CommonStr::Status::NoStatusStr;
			rangeStr << CommonStr::Status::NoStatusStr;
			costStr << CommonStr::Status::NoStatusStr;
			break;
		default:
			ASSERT( false );
			break;
		}
		DrawStringToHandle( StatusDrawPosX, PowerValueDrawPosY, powerStr.str().c_str(), StringColor, mFontHandle );
		DrawStringToHandle( StatusDrawPosX, SpeedValueDrawPosY, speedStr.str().c_str(), StringColor, mFontHandle );
		DrawStringToHandle( StatusDrawPosX, RangeValueDrawPosY, rangeStr.str().c_str(), StringColor, mFontHandle );
		DrawStringToHandle( StatusDrawPosX, CostValueDrawPosY , costStr.str().c_str() , StringColor, mFontHandle );

		/** �Ԏ��������茋�ʕ`�� */
		int imgNum = 0;
		if ( mAssets < mInitAssets ) {
			/** �Ԏ��Ȃ�؂�ւ� */
			imgNum = 1;
		}
		DrawGraph( EconomicDrawPosX, EconomicDrawPosY, mEconomicIconHandle.at( imgNum ) , TRUE );

		/** ���ΑK�z�`�� */
		DrawStringToHandle( AssetDrawPosX, AssetDrawPosY, getZeroPadding( mAssets, AssetNumOfDisits ).c_str() , StringColor, mFontHandle );

		/** �X�R�A�`��   */
		DrawStringToHandle( ScoreDrawPosX, ScoreDrawPosY, getZeroPadding( mScore, ScoreNumOfDisits ).c_str() , StringColor, mFontHandle );

	}

	/** �A�C�R���`�� */
	void State::drawCharaIcon( bool selectionDraw ) const{

		for ( unsigned int i = 0; i < mCharaSelectIconHandles.size(); i++ ) {
			/** �A�C�R���擪�n���h�����󂯎�� */
			int handle = mCharaSelectIconHandles.at( i );
			int posX = mIconDrawPos.at(i).getX();
			int posY = mIconDrawPos.at(i).getY();
			if ( ( i == mSelection ) || Sequence::CommonFunc::isMousePointOnImage( posX, posY, handle ) && selectionDraw ) {
				ASSERT( SetDrawBright( UCHAR_MAX , UCHAR_MAX , UCHAR_MAX ) + 1 );
			} else {
				ASSERT( SetDrawBright( UCHAR_MAX / 2, UCHAR_MAX / 2, UCHAR_MAX / 2 ) + 1 );
			}
			ASSERT( DrawGraph( mIconDrawPos.at(i).getX(), mIconDrawPos.at(i).getY(), handle,TRUE ) + 1 );
		}
		ASSERT( SetDrawBright( UCHAR_MAX , UCHAR_MAX , UCHAR_MAX ) + 1 );
		/** �I�𒆃L������g�ň͂� */
		if ( mSelection != Non ) {
			int posX = mIconDrawPos.at(mSelection).getX();
			int posY = mIconDrawPos.at(mSelection).getY();
			ASSERT( DrawGraph( posX, posY, mSelectionFrameHandle, TRUE ) + 1 );
		}
	}

	/** �G�t�F�N�g�`�� */
	void State::drawEffect( vector<Effect::EffectBase*> effect ) const{

		vector<Effect::EffectBase*>::const_iterator It = effect.begin();
		for ( ; It != effect.end(); It++ ) {
			if ( (*It)->getIsDraw() ) {
				string imageName = FileNames::Image::EffectImagesVector.at( (*It)->getId() );
				int panelId = (*It)->getPanel();
				int handle = mEffectImageHandles.at(imageName).at(panelId);
				/** �`��ʒu�v�Z */
				int sizeX = (*It)->getSize();
				int sizeY = (*It)->getSize();
				int upperPosX = (*It)->getPoint().getX() - ( ( ( sizeX - CommonConst::MapChipSize ) / 2 ) );
				int upperPosY = (*It)->getPoint().getY() - ( ( ( sizeY - CommonConst::MapChipSize ) / 2 ) );
				int lowerPosX = upperPosX + (*It)->getSize();
				int lowerPosY = upperPosY + (*It)->getSize();
				ASSERT(  DrawExtendGraph( upperPosX, upperPosY, lowerPosX, lowerPosY, handle, TRUE ) + 1 );
#ifdef _DEBUG
				ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX / 5 ) + 1 );
				DrawCircle( (*It)->getPoint().getX(), (*It)->getPoint().getY(), (*It)->getSize() / 2 , GetColor(UCHAR_MAX, UCHAR_MAX, 0));
				ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,UCHAR_MAX ) + 1 );
#endif
			}
		}
	}

	/** ���W���}�b�v��ł��邩�ǂ����̔��� */
	bool State::isPointOnMap( int posX, int posY ) const{
		int mapSizeX = mStageMap->getChipSize() * mStageMap->getMapSizeX();
		int mapSizeY = mStageMap->getChipSize() * mStageMap->getMapSizeY();
		bool pointOnMap = ( 0 <= posX ) && ( 0 <= posY );
		pointOnMap &= ( posX < mapSizeX ) && ( posY < mapSizeY );
		
		return pointOnMap;
	}

	/** ���͎�t */
	/** posX,posY: �}�E�X���W */
	void State::mInputPrompt( int posX, int posY ){
		
		/** �}�b�v�������� */
		if ( posX < ( mStageMap->getMapSizeX() * mStageMap->getChipSize() ) ){
			/** �}�b�v�̂ǂ̃}�X�����擾 */
			int mapPosX = posX / mStageMap->getChipSize();
			int mapPosY = posY / mStageMap->getChipSize();
			
			/** �u�������ǂ����̔��� */
			switch( mSelection ) {
			/** �L�����I�𒆂Ȃ�ݒu */
			case Plane:
			case Miko:
			case Majokko:
			case Yojo:
				{
					Friend::FriendBase* addFriend = getAddFriend( mapPosX, mapPosY );
					if ( addFriend != NULL ) {
						mFriends.push_back( addFriend );
						mStageMap->setOnFriendFlag( mapPosX, mapPosY, true );
						mAssets -= mFriendData->getStatusValue()->at( addFriend->getId() ).TotalValue.at( CharaDataXml::CostStr );
					}
				}
				break;
			/** ���p�I���� */
			case Sale:
				{
					bool isSale = false;
					isSale = mSaleFriend( mapPosX, mapPosY );
				}
				break;
			/** �o�� */
			case Awake:
				{
					bool isAwake = false;
					isAwake = mAwakeFriend( mapPosX, mapPosY );
				}
				break;
			case Non:
				break;
			default:
				break;
			}
			/** �����Œu�������ǂ����ŕ`�攻�肷�邩�� */
		/** �󋵕\���������� */
		} else {
			/** �L�����A�C�R������ */
			for ( unsigned int i = 0; i < mIconDrawPos.size(); i++ ) {
				/** �n���h���󂯎�� */
				int handle = mCharaSelectIconHandles.at(i);
				if ( Sequence::CommonFunc::isMousePointOnImage( mIconDrawPos.at(i).getX(), mIconDrawPos.at(i).getY(), handle ) ) {
					/** �L����id�Z�b�g */
					mSetCharaType( i );
					break;
				}
			}
		}
	}

	void State::mShiftSelection() {
		
		if ( mSelection == Non ) {
			return;
		}
		
		int value = mSelection + 1;
		if ( value == Non ) {
			value = Plane;
		}
		mSetCharaType( value );
	}

	void State::mSetCharaType( int aCharaTypeId ) {
		switch( aCharaTypeId ) {
			/** ���݂���id�̏ꍇ */
			case Plane:
			case Miko:
			case Majokko:
			case Yojo:
			case Sale:
			case Awake:
			case Non:
				mSelection = static_cast< FriendType >( aCharaTypeId );
				break;
			/** ����ȊO�͉������Ȃ� */
			default:
				break;
			}
	}


}