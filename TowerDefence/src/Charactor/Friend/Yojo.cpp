#include "Charactor\Friend\Yojo.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "Effect\Explosion.h"
#include "CommonConst.h"
#include "DxLib.h"
#include "Sequence\GameSequence\Parent.h"
#include "FileNames.h"
#include "define.h"

using namespace std;

namespace Charactor{
	namespace Friend{
		/** Baseと特に変更は無し */
		Yojo::Yojo(
			int aSkillPoint,
			int aPower, 
			int aSpeed,
			int aRange,
			int aId,
			int aLevel,
			Game::Point p) : 
		FriendBase(
			aSkillPoint,
			aPower, 
			aSpeed,
			aRange,
			aId,
			aLevel,
			p){}

		Yojo::~Yojo(){}

		void Yojo::update(
			std::vector< Enemy::EnemyBase* >* aEnemys, 
			std::vector< FriendBase* >* aFriends, 
			Game::StageMap* aStageMap ){
				
				/** 敵配列が存在しないなら終了 */
				if ( aEnemys == NULL ) {
					return;
				}

				/** 覚醒時はスリップ値設定 */
				if ( mIsAwake ) {
					mSlip = this->getPower() * 2;
					if ( mSkillGauge != 0 ) {
						mSkillGauge--;
					}
					if ( mSkillGauge == 0 ) {
						this->setIsAwake( false );
					}
				} else {
					mSlip = 0;
				}

				/** ターゲットが設定されていなければ攻撃対象を探索 */
				if ( !mHaveTarget ) {
					mTargetEnemy = FriendBase::calcNearEnemy( aEnemys );
					mHaveTarget = true;
					/** ターゲットが設定されていれば生存状態を確認し、必要であれば再探索 */
				} else {
					if ( !mTargetEnemy->getSurvive() ) {
						mTargetEnemy = FriendBase::calcNearEnemy( aEnemys );
					}
				}
				/** 攻撃対象がいれば範囲攻撃をしゲージをリセット */
				if ( mTargetEnemy != 0 ) {
					/** ターゲット位置からキャラの向きを設定 */
					mMotionId = calcDirectionId( mTargetEnemy ) * CommonConst::CharaDotImgDivNumX;
					/** 攻撃モーション分加算 */
					mMotionId += ( ( mAttackGauge * CommonConst::CharaDotImgDivNumX ) - 1 ) / this->getSpeed();
					if ( this->getSpeed() <= mAttackGauge ) {
						Effect::Explosion* explosion;
						explosion = new Effect::Explosion( this->getPoint() , 1 , this->getRange() * 2 );
						mExplosions.push_back( explosion );
						mAttackGauge = 0;
						mHaveTarget = false;
						/** 爆発音追加 */
						int SEHandle = 
							Sequence::GameSequence::Parent::loader()->getSoundFileHandle()->at( FileNames::Sound::ExplosionSEName );
						ASSERT( PlaySoundMem( SEHandle, DX_PLAYTYPE_BACK ) + 1 );
						/** 範囲内全体の敵にダメージ加算 */
						vector< Enemy::EnemyBase* >::iterator enemyIt = aEnemys->begin();
						for ( ; enemyIt != aEnemys->end() ; enemyIt++ ) {
							if ( !(*enemyIt)->getAppearance() ) {
								continue;
							}
							int distance = static_cast< int >( (*enemyIt)->getPoint().getDistance( this->getPoint() ) );
							if ( distance < ( this->getRange() + (*enemyIt)->getRange() ) ) {
								(*enemyIt)->addDamage( this->getPower() );
								(*enemyIt)->setSlip( this->mSlip );
							}
						}
						if ( ( mSkillGauge < mMaxSkillGauge ) && !mIsAwake ) {
							mSkillGauge += mAwakeValueUnit;
						}
					} else {
						mAttackGauge++;
					}
				} else {
					mMotionId = FriendBase::LowerLeft;
					mAttackGauge = 0;
					mHaveTarget = false;
				}

				/** 爆破エフェクト更新 */
				for ( unsigned int i = 0; i < mExplosions.size(); i++ ) {
					mExplosions.at( i )->update();
				}
				for ( vector<Effect::Explosion*>::iterator It = mExplosions.begin() ;It != mExplosions.end(); ) {
					if ( !(*It)->getIsDraw() ) {
						It = mExplosions.erase( It );
					} else {
						It++;
					}
				}
		}

		void Yojo::draw() const{
		}
	}
}