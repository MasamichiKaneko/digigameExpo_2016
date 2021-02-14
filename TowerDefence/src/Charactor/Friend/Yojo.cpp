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
		/** Base�Ɠ��ɕύX�͖��� */
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
				
				/** �G�z�񂪑��݂��Ȃ��Ȃ�I�� */
				if ( aEnemys == NULL ) {
					return;
				}

				/** �o�����̓X���b�v�l�ݒ� */
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

				/** �^�[�Q�b�g���ݒ肳��Ă��Ȃ���΍U���Ώۂ�T�� */
				if ( !mHaveTarget ) {
					mTargetEnemy = FriendBase::calcNearEnemy( aEnemys );
					mHaveTarget = true;
					/** �^�[�Q�b�g���ݒ肳��Ă���ΐ�����Ԃ��m�F���A�K�v�ł���΍ĒT�� */
				} else {
					if ( !mTargetEnemy->getSurvive() ) {
						mTargetEnemy = FriendBase::calcNearEnemy( aEnemys );
					}
				}
				/** �U���Ώۂ�����Δ͈͍U�������Q�[�W�����Z�b�g */
				if ( mTargetEnemy != 0 ) {
					/** �^�[�Q�b�g�ʒu����L�����̌�����ݒ� */
					mMotionId = calcDirectionId( mTargetEnemy ) * CommonConst::CharaDotImgDivNumX;
					/** �U�����[�V���������Z */
					mMotionId += ( ( mAttackGauge * CommonConst::CharaDotImgDivNumX ) - 1 ) / this->getSpeed();
					if ( this->getSpeed() <= mAttackGauge ) {
						Effect::Explosion* explosion;
						explosion = new Effect::Explosion( this->getPoint() , 1 , this->getRange() * 2 );
						mExplosions.push_back( explosion );
						mAttackGauge = 0;
						mHaveTarget = false;
						/** �������ǉ� */
						int SEHandle = 
							Sequence::GameSequence::Parent::loader()->getSoundFileHandle()->at( FileNames::Sound::ExplosionSEName );
						ASSERT( PlaySoundMem( SEHandle, DX_PLAYTYPE_BACK ) + 1 );
						/** �͈͓��S�̂̓G�Ƀ_���[�W���Z */
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

				/** ���j�G�t�F�N�g�X�V */
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