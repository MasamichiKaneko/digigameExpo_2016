#pragma once

/**
* ���L�������N���X
*/

#include <vector>
#include "Game\Point.h"
#include "Charactor\CharactorBase.h"

namespace Game{	
	class StageMap;
}

namespace Effect{
	class Shot;
	class Explosion;
}

namespace Charactor{

	namespace Enemy{
		class EnemyBase;
	}

	namespace Friend{

		static const std::string CharactorType = "Friend";
		static const int AwakeValueUnit = 6;
		class FriendBase : public CharactorBase{
		public:
			FriendBase( 
				int aSkillPoint,
				int aPower, 
				int aSpeed,
				int aRange,
				int aId,
				int aLevel,
				Game::Point p
				) :
			mSkillPoint( aSkillPoint ),
				mPower( aPower ),
				mSpeed( aSpeed ),
				mAttackGauge(aSpeed),
				mLevel( aLevel ),
				mSkillGauge( 0 ),
				mStun( 0 ),
				mSlow( 0 ),
				mSlip( 0 ),
				mAwakeValueUnit( AwakeValueUnit ),
				mMaxSkillGauge( 30 * AwakeValueUnit ),
				mPowerRatio( 1.0 ),
				mSpeedRatio( 1.0 ),
				mRangeRatio( 1.0 ),
				mHaveTarget( false ),
				mIsAwake( false ){
					mRange = aRange;
					mMotionId = 0;
					mPoint = p;
					mId = aId;
			}

			virtual ~FriendBase(){};

			/* �X�V�֐� */
			/**                     ���G�L�����z��                                �����L�����z��                          ���}�b�v�z��      */
			virtual void update( std::vector< Enemy::EnemyBase* >* aEnemys, std::vector< FriendBase* >* aFriends, Game::StageMap* aStageMap ) = 0;

			/* �X�e�[�^�Xget */
			int getSkillPoint() const{ return mSkillPoint; }
			int getPower() const{ return static_cast< int >( mPower * mPowerRatio ); }
			int getSpeed() const{ return static_cast< int >( mSpeed * ( 1.0 / mSpeedRatio ) ); }
			int getRange() const{ return static_cast< int >( mRange * mRangeRatio ); }
			int getId() const{ return mId; }
			int getLevel() const{ return mLevel; }
			bool getIsAwake() const{ return mIsAwake; }
			Game::Point getPoint() const{ return mPoint; }
			
			/** �\���Q�[�W�ɃX�L���Q�[�W��Ԃ� */
			int getGauge() const{ return mSkillGauge; }
			int getGaugeMax() const{ return mMaxSkillGauge; }

			/* �{��set */
			void setPowerRatio( float ratio ){ mPowerRatio = ratio; return; }
			void setSpeedRatio( float ratio ){ mSpeedRatio = ratio; return; }
			void setRangeRatio( float ratio ){ mRangeRatio = ratio; return; }

			/** �o��set */
			void setIsAwake( bool value ){ mIsAwake = value; }

			/** �G�t�F�N�ggetter */
			std::vector< Effect::EffectBase* > getEffect()const; 
			/** �L�����^�C�vgetter */
			std::string getCharactorType();

		protected:

			/** ����id */
			enum DirectionId{
				LowerLeft = 0,
				LowerRight,
				UpperLeft,
				UpperRight
			};

			/** �X�V�����̑S�L�������ʕ����֐� */
			/** �͈͓��̈�ԋ߂��G��T������ */
			/** �G�L�������͈͓��ɂ��Ȃ����end��n�� */
			Enemy::EnemyBase* calcNearEnemy( std::vector< Enemy::EnemyBase* >* aEnemys );

			/** �����v�Z */
			DirectionId calcDirectionId( Enemy::EnemyBase* aEnemy ) const;

			/** �ʏ�X�V���� */
			void planeUpdate( std::vector< Enemy::EnemyBase* >* aEnemys, std::vector< FriendBase* >* aFriends, Game::StageMap* aStageMap );

			/** �U���Ώۂ̓G */
			bool mHaveTarget;
			Enemy::EnemyBase* mTargetEnemy;

			/* ���ʃX�e�[�^�X */
			int mSkillGauge;
			int mMaxSkillGauge;

			/** �o�����t���O */
			bool mIsAwake;

			/* �U�����X�L���|�C���g���� */
			int mSkillPoint;
			/* �Η� */
			int mPower;
			/* �o�X�e�l */
			int mStun;
			int mSlow;
			int mSlip;
			/* �U���Ԋu(fps�P��) */
			int mSpeed;
			int mAttackGauge;
			/* ���x�� */
			int mLevel;

			/** �o���l�P�� */
			int mAwakeValueUnit;

			/** �e�G�t�F�N�g */
			std::vector< Effect::Explosion* > mExplosions;
			std::vector< Effect::Shot* > mShots;


			/* �X�e�[�^�X�̔{�� */
			/* �Ⴆ�Ύ��͂̃L������1.5�{�ɋ�������A�V�X�g�n�̃L�����������Ƃ��� */
			/* ��������L�����̂��̒l��1.5�Ƃ��ɘM���1.5�{�ɂȂ�悤�ɂ��� */
			/* ���̒l��M��̂͐F�X�ƕs�s���o����������M��Ȃ� */
			float mPowerRatio;
			float mSpeedRatio;
			float mRangeRatio;
		};
	}
}