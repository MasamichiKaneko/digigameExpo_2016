#pragma once

/**
*  �Q�[���̏�ԃN���X
*/

#include <vector>
#include <map>
#include <string>
#include "Game\Point.h"

class FileLoader;


namespace Charactor{
	class CharactorBase;
	namespace Friend{
		class FriendBase;
	}
	namespace Enemy{
		class EnemyBase;
	}
}

namespace Effect{
	class Explosion;
	class EffectBase;
}

namespace Game{
	
	class CharaStatusReader;
	class EnemyStatusReader;
	class StageMap;

	class State{
	public:
		State( FileLoader* loader , int stageNum );
		~State();

		void update();  //�Q�[����ԍX�V
		
		/** �I�����L�����^�C�v */
		enum FriendType{
			Plane,
			Miko,
			Majokko,
			Yojo,

			Sale,
			Awake,
			Non
		};

		/** ��~�E�Đ��{�^���^�C�v��` */
		enum PlayStopButtonType{
			Play,
			Stop
		};

		/** �G�t�F�N�g�^�C�v */
		enum EffectType{
			Shot,
			Explosion
		};

		bool getGameOver() const{ return mGameOver; }
		int getAssetsValue() const{ return mAssets; }
		int getInitAssetsValue() const{ return mInitAssets; }
		int getScoreValue() const{ return mScore; }
		int getStageNum() const{ return mStageNum; }
		Game::CharaStatusReader* getReadFriendData() const { return mFriendData; }
		StageMap* getMapState() const{ return mStageMap; }
		FriendType getSelectionType() const { return mSelection; }
		std::vector<int> getSelectIconHandles() const { return mCharaSelectIconHandles; }

		/** �`��֐� */
		void draw( bool selectionDraw ) const;

		/** ��~�E�Đ��{�^���`�� */
		void drawPlayStopButton( PlayStopButtonType type ) const;

		/**
		* �L�������p�֐�
		* return true :���p����
		*        false:���p�s��
		*/
		bool mSaleFriend( int posX, int posY );

		/**
		* �L�����o���֐�
		* return true :���p����
		*        false:���p�s��
		*/
		bool mAwakeFriend( int posX, int posY );

		/** ���͎�t */
		/** posX,posY: �}�E�X���W */
		void mInputPrompt( int posX, int posY );

		/** �}�E�X���W����~�E�Đ��{�^����ɑ��݂��邩��Ԃ� */
		/** true:���݂��� false:���݂��Ȃ� */
		bool mIsMousuPointOnPlayStopButton() const;

		/** �I���L�����^�C�v�Z�b�g */
		void mSetCharaType( int aCharaTypeId );

		/** �I�����炵�֐� */
		void mShiftSelection();

	private:

		/** �`�b�v��� */
		enum ChipType{
			PlaneChip = 0,
			NotBePlaced,   //�ݒu�s�`�b�v
			GoalChip,      //�ڕW�`�b�v
			/** �ȉ����`�b�v */
			RoadChip_0,
			RoadChip_1,
			RoadChip_2,
			RoadChip_3,
			RoadChip_4,
			RoadChip_5,
			RoadChip_6,
			RoadChip_7,
			RoadChip_8,
			RoadChip_9,
			RoadChip_10
		};

		/** �I�𒆃L�����^�C�v */
		FriendType mSelection;

		/** �}�b�v�`�� */
		void drawMap( bool selectionDraw )const;
		/** ���L�����U���͈͕`�� */
		void drawFriendsRange()const;
		/** �A�C�R���`�� */
		void drawCharaIcon( bool selectionDraw )const;
		/** �󋵕`�� */
		void drawState()const;
		/** �G�t�F�N�g�`�� */
		void drawEffect( std::vector<Effect::EffectBase*> effect ) const;
		

		/** ���W���}�b�v��ł��邩�ǂ����̔��� */
		bool isPointOnMap( int posX, int posY ) const;

		/** 
		* �L�����ǉ��֐� 
		* return true :�z�u����
		*        false:�z�u�s��
		*/
		Charactor::Friend::FriendBase* getAddFriend( int posX, int posY ) const;

		/** �`��Ώۂ̃L�����擾�֐� */
		std::vector<Charactor::CharactorBase*> getDrawTargets() const;

		/** �L�����`�� */
		void drawCharactors( bool selectionDraw )const;

		/** �G�L�����폜 */
		/** ���ꂽ�G */
		void deleteDeadEnemys();
		/** ���B�����G */
		void deleteArrivalEnemys();
		
		/** �����G�t�F�N�g�폜 */
		void deleteEnemyExplosions();

		/** �O���b�h�J���[ */
		static const int GridColor;
		/** �O���b�h�Z�x   */
		int mGridDipth;

		/** �摜�n���h�� */
		/** �}�b�v�`�b�v�n���h�� */
		std::vector<int> mMapChip;
		/** �_�Ѓn���h�� */
		int mTempleHandle;
		/** �U���͈̓C���[�W�n���h�� */
		int mRangeHandle;
		/** �󋵕\���o�b�N�n���h�� */
		int mStateBackHandle;
		/** �I�𒆃t���[���n���h�� */
		int mSelectionFrameHandle;
		/** �Ԏ������摜�n���h�� */
		std::vector<int> mEconomicIconHandle;
		/** �L�����A�C�R���n���h�� */
		std::vector< int > mCharaSelectIconHandles;
		std::map< std::string, std::vector< int > > mCharaIconHandles;
		/** �G�L�����A�C�R���n���h�� */
		std::map< std::string, std::vector< int > > mEnemyIconHandles;
		/** �G�t�F�N�g�摜�n���h�� */
		std::map< std::string, std::vector< int > > mEffectImageHandles;

		/** �t�H���g�n���h�� */
		int mFontHandle;

		/** �����n���h�� */
		int mExplosionSEHandle;

		/** ���L�����I��p�A�C�R���`��ʒu */
		std::vector< Point > mIconDrawPos;

		/** �ǂݍ��񂾃L�����f�[�^�i�[�p */
		std::map<int, std::map<std::string, int> > mStatusValues;

		/** �Q�[���I���t���O */
		bool mGameOver;
		/** �X�R�A */
		int mScore;
		/** �����Y */
		int mAssets;
		/** �������Y */
		int mInitAssets;
		/** �X�e�[�W�� */
		int mStageNum;
		/** ���L���� */
		std::vector<Charactor::Friend::FriendBase*> mFriends;
		/** �G�L���� */
		std::vector<Charactor::Enemy::EnemyBase*> mEnemys;
		/** �G���j�G�t�F�N�g */
		std::vector<Effect::Explosion*> mEnemyExplosions;
		
		/** �}�b�v  */
		StageMap* mStageMap;
		/** ���L�����f�[�^�ǂݍ��� */
		CharaStatusReader* mFriendData;
		/** �G�L�����f�[�^�ǂݍ��� */
		EnemyStatusReader* mEnemyData;

		/** �萔 */
		/** ��~�E�Đ��{�^���`��ʒu */
		static const int PlayStopButtonPosX;
		static const int PlayStopButtonPosY;
		/** �L�����A�C�R���`��ʒu */
		static const int CharaIconDrawX;
		static const int CharaIconDrawY;
		/** �L�����A�C�R���`��Ԋu */
		static const int CharaIconDrawIntervalX;
		static const int CharaIconDrawIntervalY;
		/** �G�L����HP�\���c�� */
		static const int GaugeDrawHeight;
		/** �󋵕`��ʒu */
		static const int AssetDrawPosX;
		static const int AssetDrawPosY;
		static const int ScoreDrawPosX;
		static const int ScoreDrawPosY;
		static const int EconomicDrawPosX;
		static const int EconomicDrawPosY;
		/** �\���Q�[�W�J���[ */
		static const int EnemyHPGaugeColor;
		static const int FriendSkillGaugeColor;
		static const int GaugeBackColor;
		static const int GaugeFrameColor;
		/** �����̐F */
		static const int StringColor;
		/** �s���Ƃ̃L�����A�C�R���`�搔 */
		static const int CharaIconDrawNumRow;
		
		/** �󋵕\���J�n�ʒu */
		static const int StateDrawPosX;
		static const int StateDrawPosY;

		/** �X�e�[�^�X�\���ʒu */
		static const int StatusDrawPosX;
		static const int PowerValueDrawPosY;
		static const int SpeedValueDrawPosY;
		static const int RangeValueDrawPosY;
		static const int CostValueDrawPosY;

		/** �L�����`��ʒu�␳ */
		static const int FriendDrawCorr;
		
		/** �t�H���g�T�C�Y */
		static const int FontSize;
		/** �t�H���g�X�^�C�� */
		static const char* FontStyle;

		/** ���ΑK�z�\������ */
		static const int AssetNumOfDisits;

		/** �X�R�A�\������ */
		static const int ScoreNumOfDisits;
	};
}