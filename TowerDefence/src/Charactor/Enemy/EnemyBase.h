#pragma once

/**
* �G�L�������N���X
*/

#include <vector>
#include "Game\Point.h"
#include "Charactor\CharactorBase.h"

namespace Game{
	class StageMap;
}

namespace Charactor{

	namespace Friend{
		class FriendBase;
	}

	namespace Enemy{

		static const std::string CharactorType = "Enemy";

		class EnemyBase : public CharactorBase{
		public:
			EnemyBase(
				int aId,                 //id
				int aLevel,              //���x��
				int aHitPoint,           //HP
				int aSpeed,              //���x
				int aScore,              //���������Z�X�R�A
				int aAssets,             //���������Z����
				int aDamage,             //���B�����Q�l
				int aRangeRadius,        //�����蔻�蔼�a
				int aAdventStartFrame,   //�o���J�n�t���[��
				std::vector< int > aRoot,//�o�H���
				Game::StageMap* aStage,        //�}�b�v���
				double aResistStun,       //�X�^���ϐ�
				double aResistSlow        //�X���E�ϐ�
				);
			virtual ~EnemyBase(){};

			/* �X�V�֐� */
			/**                     ���G�L�����z��                         �����L�����z��                                ���}�b�v�z��  */
			virtual void update( std::vector< EnemyBase* >* aEnemys, std::vector< Friend::FriendBase* >* aFriends, Game::StageMap* aStageMap ) = 0;

			/* �X�e�[�^�Xgetter��` */
			int getScore() const { return mScore; }
			int getAssets() const { return mAssets; }

			int getHitPoint() const{ return mHitPoint; }
			int getMaxHitPoint() const{ return mMaxHitPoint; }
			int getSpeed() const{ return mSpeed; }
			int getLevel() const{ return mSlow; }
			int getAssetDamage() const { return mAssetDamage; }

			int getStun() const{ return mStun; }
			int getSlow() const{ return mSlow; }
			int getSlip() const{ return mSlip; }
			
			bool getAppearance() const{ return mAppearance; }
			bool getSurvive() const{ return mSurvive; }

			bool getPriority() const{ return mPriority; }
			int getDistanceToGoal() const { return mDistanceToGoal; }

			/** �\���Q�[�W��HP��Ԃ� */
			int getGauge() const{ return mHitPoint; }
			int getGaugeMax() const{ return mMaxHitPoint; }

			/** �L�����^�C�vgetter */
			std::string getCharactorType();

			/* ���[�g�擾 */
			const std::vector< int >* getRoot() const{ return &mRoot; }

			/** �_���[�W�t�^�֐� */
			void addDamage( int aDamageValue ){
				mHitPoint -= aDamageValue;
			}

			/** �����t���O�Z�b�g�֐� */
			void setSurvive( bool aSurvive ) {
				mSurvive = aSurvive;
			}

			/* �X�^���l�t�^�֐� */
			void setStun( int aStunValue ){
	//			if ( mStun == 0 ) {
					mStun = static_cast< int >( aStunValue * mResistStun );
	//			}
			}
			/* �X���E�l�t�^�֐� */
			void setSlow( int aSlowValue ){
	//			if ( mSlow == 0 ) {
					mSlow = static_cast< int >( aSlowValue * mResistSlow );
	//			}
			}
			/* �X���b�v�l�t�^�֐� */
			void setSlip( int aSlip ){
				mSlip = aSlip;
			}

			std::vector< Effect::EffectBase* > getEffect()const{ return mEffect; }

		protected:

			/** ����Id */
			enum DirectionId {
				Front = 0,
				Left,
				Right,
				Back
			};

			/** �ړ�����  ���}�b�v���  ���ړ�����(�h�b�g�P��) */
			void move( Game::StageMap* stage, int movingDistance );

			/** �S�[���܂ł̋����v�Z */
			/** �v�Z�s��-1���Ԃ���� */
			int culcDistanceToGoal( Game::StageMap* stage );

			/* ���ʃX�e�[�^�X */
			/* ���j���̑����X�R�A */
			int mScore;
			/* ���j���̑������Y   */
			int mAssets;

			/** �X���b�v��ԗp�̃J�E���g */
			int mSlipCount;
			/** ��������(���[�V�����v�Z�p) */
			int mMoveCount;
			/** �������[�V�����̊Ԋu */
			int mWalkMotion;
			/* �ϋv */
			int mMaxHitPoint;
			int mHitPoint;
			/* �ړ����x */
			int mSpeed;
			/* ���x�� */
			int mLevel;

			/* ���B���̑��Q */
			int mAssetDamage;

			/** �o���J�n�t���[�� */
			int mAdventStartFrame;

			/** �o�����t���O */
			bool mAppearance;

			/* �X�^���l */
			int mStun;
			/* �X���b�v�l */
			int mSlip;
			/* �X���E�l */
			int mSlow;
			
			/** �����̉摜�؂�ւ��Ԋu */
			static const double WalkSwitchingSecond;

			/* �����t���O */
			bool mSurvive;

			/** �D��t���O */
			/* true�͗D��I�ɍU�������\�� */
			bool mPriority;

			/* �h�q�ڕW�܂ł̋��� */
			/* �^�[�Q�b�g�t���O���Ȃ���h�q�ڕW�Ɉ�ԋ߂�����_���� */
			int mDistanceToGoal;
			
			/** �G�t�F�N�g */
			std::vector< Effect::EffectBase* > mEffect;

			/* �ړ��o�H */
			/* �}�b�v��̋Ȃ���p�╪���ꓹ�ɂ��炩���߃i���o�[��U���Ă����� */
			/* ��������ǂ鏈����z�肵�Ă��� */
			std::vector< int > mRoot;

			/* �ϐ��l */
			/* �󂯂��Ԉُ�l�ɂ�����|���� */
			/* 0.0�Ȃ�����Ȃ��A1.0���傫���Ȃ�����₷�������� */
			double mResistStun;
			double mResistSlow;
		};
	}
}