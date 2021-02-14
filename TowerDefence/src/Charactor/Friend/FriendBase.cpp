#include "Charactor\Friend\FriendBase.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "Game\StageMap.h"
#include "CommonConst.h"
#include "Effect\Explosion.h"
#include "Effect\Shot.h"

using namespace std;

namespace Charactor{
	namespace Friend{

		/** �G�t�F�N�ggetter */
		std::vector< Effect::EffectBase* > FriendBase::getEffect()const{
			vector< Effect::EffectBase* > effects;
			effects.reserve( mShots.size() + mExplosions.size() );
			effects.insert( effects.begin() ,mShots.begin(), mShots.end() );
			effects.insert( effects.begin() ,mExplosions.begin(), mExplosions.end()  );
			return effects;
		}

		/** �L�����^�C�vgetter */
		string FriendBase::getCharactorType() { return CharactorType; }

		Enemy::EnemyBase* FriendBase::calcNearEnemy( vector< Enemy::EnemyBase* >* aEnemys ){
			/** �G�L�����z�񂩂�U���Ώۂ�T�� */
				vector< Enemy::EnemyBase* >::iterator countIt;
				vector< Enemy::EnemyBase* >::iterator targetIt;
				countIt = aEnemys->begin();
				targetIt = aEnemys->end();
				Enemy::EnemyBase* enemy = 0;
				for ( ; countIt != aEnemys->end(); countIt++ ){

					/** �������Ă��Ȃ��܂��͏o�����Ă��Ȃ��G�Ȃ玟���`�F�b�N */
					if ( !(*countIt)->getSurvive() || !(*countIt)->getAppearance() ) {
						continue;
					}

					/** �����v�Z */
					int distance =  static_cast< int >( mPoint.getDistance( (*countIt)->getPoint() ) + 0.5);

					/** �U���͈͓��ł��邩 */
					bool isInRange = ( distance < this->getRange() + ( ( *countIt )->getRange() ) );
					/** �͈͊O�̓G�Ȃ玟�̓G���`�F�b�N */
					if ( !isInRange ){
						continue;
					}

					/** �ŏ��̃^�[�Q�b�g�ł��邩 */
					bool isFirstTarget = ( targetIt == aEnemys->end() );

					/** �ŏ��̃^�[�Q�b�g�Ȃ�b��^�[�Q�b�g�ɃZ�b�g */
					if ( isFirstTarget ) {	
						targetIt = countIt;
						/** �ŏ��łȂ���Ύb��^�[�Q�b�g�ƗD��x���r */
					} else {
						/** �D��^�[�Q�b�g�w��G�L�����Ȃ�^�[�Q�b�g�Ƃ��ăZ�b�g���ă��[�v�I�� */
						if ( (*countIt)->getPriority() ) {
							targetIt = countIt;
							break;
						}
						/** ������r���Ėh�q�ڕW�ɋ߂������^�[�Q�b�g�ɐݒ� */
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



		/** �����v�Z */
		FriendBase::DirectionId FriendBase::calcDirectionId( Enemy::EnemyBase* aEnemy ) const{
			DirectionId direction = LowerLeft;
			/** �G���W���擾 */
			int enemyPosX = aEnemy->getPoint().getX();
			int enemyPosY = aEnemy->getPoint().getY();

			bool upper = ( enemyPosY < mPoint.getY() );
			bool left  = ( enemyPosX < mPoint.getX() );

			/** ���� */
			if ( !upper && left ) {
				direction = LowerLeft;
			/** �E�� */
			} else if( !upper && !left ) {
				direction = LowerRight;
			/** ���� */
			} else if ( upper && left ) {
				direction = UpperLeft;
			/** �E�� */
			} else if ( upper && !left ) {
				direction = UpperRight;
			}
			return direction;
		}


		void FriendBase::planeUpdate( std::vector< Enemy::EnemyBase* >* aEnemys, std::vector< FriendBase* >* aFriends, Game::StageMap* aStageMap ) {
			/** �G�z�񂪑��݂��Ȃ��Ȃ�I�� */
			if ( aEnemys == NULL ) {
				return;
			}

			/** �^�[�Q�b�g���ݒ肳��Ă��Ȃ���΍U���Ώۂ�T�� */
			if ( !mHaveTarget ) {
				mTargetEnemy = FriendBase::calcNearEnemy( aEnemys );
				mHaveTarget = true;
				/** �^�[�Q�b�g���ݒ肳��Ă���ΐ�����Ԃ��m�F���A�K�v�ł���΍ĒT�� */
			} else {
				if ( !mTargetEnemy->getSurvive() ) {
				//	vector< Enemy::EnemyBase* >::iterator targetIt;
					mTargetEnemy = FriendBase::calcNearEnemy( aEnemys );
				}
			}

			/** �U���Ώۂ�����Βe���΂��Q�[�W�����Z�b�g */
			if ( mTargetEnemy != 0 ) {
				/** �^�[�Q�b�g�ʒu����L�����̌�����ݒ� */
				mMotionId = calcDirectionId( mTargetEnemy ) * CommonConst::CharaDotImgDivNumX;
				/** �U�����[�V���������Z */
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