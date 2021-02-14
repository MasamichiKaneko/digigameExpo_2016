#include "Charactor\Friend\FriendBase.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "Game\StageMap.h"
#include "CommonConst.h"
#include "Effect\Explosion.h"
#include "Effect\Shot.h"

using namespace std;

namespace Charactor{
	namespace Friend{

		/** エフェクトgetter */
		std::vector< Effect::EffectBase* > FriendBase::getEffect()const{
			vector< Effect::EffectBase* > effects;
			effects.reserve( mShots.size() + mExplosions.size() );
			effects.insert( effects.begin() ,mShots.begin(), mShots.end() );
			effects.insert( effects.begin() ,mExplosions.begin(), mExplosions.end()  );
			return effects;
		}

		/** キャラタイプgetter */
		string FriendBase::getCharactorType() { return CharactorType; }

		Enemy::EnemyBase* FriendBase::calcNearEnemy( vector< Enemy::EnemyBase* >* aEnemys ){
			/** 敵キャラ配列から攻撃対象を探索 */
				vector< Enemy::EnemyBase* >::iterator countIt;
				vector< Enemy::EnemyBase* >::iterator targetIt;
				countIt = aEnemys->begin();
				targetIt = aEnemys->end();
				Enemy::EnemyBase* enemy = 0;
				for ( ; countIt != aEnemys->end(); countIt++ ){

					/** 生存していないまたは出現していない敵なら次をチェック */
					if ( !(*countIt)->getSurvive() || !(*countIt)->getAppearance() ) {
						continue;
					}

					/** 距離計算 */
					int distance =  static_cast< int >( mPoint.getDistance( (*countIt)->getPoint() ) + 0.5);

					/** 攻撃範囲内であるか */
					bool isInRange = ( distance < this->getRange() + ( ( *countIt )->getRange() ) );
					/** 範囲外の敵なら次の敵をチェック */
					if ( !isInRange ){
						continue;
					}

					/** 最初のターゲットであるか */
					bool isFirstTarget = ( targetIt == aEnemys->end() );

					/** 最初のターゲットなら暫定ターゲットにセット */
					if ( isFirstTarget ) {	
						targetIt = countIt;
						/** 最初でなければ暫定ターゲットと優先度を比較 */
					} else {
						/** 優先ターゲット指定敵キャラならターゲットとしてセットしてループ終了 */
						if ( (*countIt)->getPriority() ) {
							targetIt = countIt;
							break;
						}
						/** 距離比較して防衛目標に近い方をターゲットに設定 */
						if ( (*targetIt)->getDistanceToGoal() > (*countIt)->getDistanceToGoal() ) {
							targetIt = countIt;
						}
					}
				}
				if ( targetIt != aEnemys->end() ) {
					enemy = *targetIt;
				}

				return enemy;
		}



		/** 向き計算 */
		FriendBase::DirectionId FriendBase::calcDirectionId( Enemy::EnemyBase* aEnemy ) const{
			DirectionId direction = LowerLeft;
			/** 敵座標を取得 */
			int enemyPosX = aEnemy->getPoint().getX();
			int enemyPosY = aEnemy->getPoint().getY();

			bool upper = ( enemyPosY < mPoint.getY() );
			bool left  = ( enemyPosX < mPoint.getX() );

			/** 左下 */
			if ( !upper && left ) {
				direction = LowerLeft;
			/** 右下 */
			} else if( !upper && !left ) {
				direction = LowerRight;
			/** 左上 */
			} else if ( upper && left ) {
				direction = UpperLeft;
			/** 右上 */
			} else if ( upper && !left ) {
				direction = UpperRight;
			}
			return direction;
		}


		void FriendBase::planeUpdate( std::vector< Enemy::EnemyBase* >* aEnemys, std::vector< FriendBase* >* aFriends, Game::StageMap* aStageMap ) {
			/** 敵配列が存在しないなら終了 */
			if ( aEnemys == NULL ) {
				return;
			}

			/** ターゲットが設定されていなければ攻撃対象を探索 */
			if ( !mHaveTarget ) {
				mTargetEnemy = FriendBase::calcNearEnemy( aEnemys );
				mHaveTarget = true;
				/** ターゲットが設定されていれば生存状態を確認し、必要であれば再探索 */
			} else {
				if ( !mTargetEnemy->getSurvive() ) {
				//	vector< Enemy::EnemyBase* >::iterator targetIt;
					mTargetEnemy = FriendBase::calcNearEnemy( aEnemys );
				}
			}

			/** 攻撃対象がいれば弾を飛ばしゲージをリセット */
			if ( mTargetEnemy != 0 ) {
				/** ターゲット位置からキャラの向きを設定 */
				mMotionId = calcDirectionId( mTargetEnemy ) * CommonConst::CharaDotImgDivNumX;
				/** 攻撃モーション分加算 */
				mMotionId += ( ( mAttackGauge * CommonConst::CharaDotImgDivNumX ) - 1 ) / this->getSpeed();
				if ( this->getSpeed() <= mAttackGauge ) {
					Effect::Shot* shot = new Effect::Shot( this->getPoint() , 1, mStun, mSlow, mSlip , CommonConst::ShotSizeX, this->getPower(), mTargetEnemy );
					mShots.push_back( shot );
					mAttackGauge = 0;
					mHaveTarget = false;
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
		}
	}

}