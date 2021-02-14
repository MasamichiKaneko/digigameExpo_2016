#pragma once

/**
* 自キャラ基底クラス
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

			/* 更新関数 */
			/**                     ↓敵キャラ配列                                ↓自キャラ配列                          ↓マップ配列      */
			virtual void update( std::vector< Enemy::EnemyBase* >* aEnemys, std::vector< FriendBase* >* aFriends, Game::StageMap* aStageMap ) = 0;

			/* ステータスget */
			int getSkillPoint() const{ return mSkillPoint; }
			int getPower() const{ return static_cast< int >( mPower * mPowerRatio ); }
			int getSpeed() const{ return static_cast< int >( mSpeed * ( 1.0 / mSpeedRatio ) ); }
			int getRange() const{ return static_cast< int >( mRange * mRangeRatio ); }
			int getId() const{ return mId; }
			int getLevel() const{ return mLevel; }
			bool getIsAwake() const{ return mIsAwake; }
			Game::Point getPoint() const{ return mPoint; }
			
			/** 表示ゲージにスキルゲージを返す */
			int getGauge() const{ return mSkillGauge; }
			int getGaugeMax() const{ return mMaxSkillGauge; }

			/* 倍率set */
			void setPowerRatio( float ratio ){ mPowerRatio = ratio; return; }
			void setSpeedRatio( float ratio ){ mSpeedRatio = ratio; return; }
			void setRangeRatio( float ratio ){ mRangeRatio = ratio; return; }

			/** 覚醒set */
			void setIsAwake( bool value ){ mIsAwake = value; }

			/** エフェクトgetter */
			std::vector< Effect::EffectBase* > getEffect()const; 
			/** キャラタイプgetter */
			std::string getCharactorType();

		protected:

			/** 向きid */
			enum DirectionId{
				LowerLeft = 0,
				LowerRight,
				UpperLeft,
				UpperRight
			};

			/** 更新処理の全キャラ共通部分関数 */
			/** 範囲内の一番近い敵を探す処理 */
			/** 敵キャラが範囲内にいなければendを渡す */
			Enemy::EnemyBase* calcNearEnemy( std::vector< Enemy::EnemyBase* >* aEnemys );

			/** 向き計算 */
			DirectionId calcDirectionId( Enemy::EnemyBase* aEnemy ) const;

			/** 通常更新処理 */
			void planeUpdate( std::vector< Enemy::EnemyBase* >* aEnemys, std::vector< FriendBase* >* aFriends, Game::StageMap* aStageMap );

			/** 攻撃対象の敵 */
			bool mHaveTarget;
			Enemy::EnemyBase* mTargetEnemy;

			/* 共通ステータス */
			int mSkillGauge;
			int mMaxSkillGauge;

			/** 覚醒中フラグ */
			bool mIsAwake;

			/* 振ったスキルポイント総数 */
			int mSkillPoint;
			/* 火力 */
			int mPower;
			/* バステ値 */
			int mStun;
			int mSlow;
			int mSlip;
			/* 攻撃間隔(fps単位) */
			int mSpeed;
			int mAttackGauge;
			/* レベル */
			int mLevel;

			/** 覚醒値単位 */
			int mAwakeValueUnit;

			/** 各エフェクト */
			std::vector< Effect::Explosion* > mExplosions;
			std::vector< Effect::Shot* > mShots;


			/* ステータスの倍率 */
			/* 例えば周囲のキャラを1.5倍に強化するアシスト系のキャラがいたとして */
			/* 強化するキャラのこの値を1.5とかに弄れば1.5倍になるようにする */
			/* 元の値を弄るのは色々と不都合出そうだから弄らない */
			float mPowerRatio;
			float mSpeedRatio;
			float mRangeRatio;
		};
	}
}