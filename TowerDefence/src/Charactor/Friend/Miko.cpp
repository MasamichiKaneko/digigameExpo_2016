#include "Charactor\Friend\Miko.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "Effect\Shot.h"
#include "Effect\Explosion.h"
#include "DxLib.h"
#include "FileNames.h"
#include "define.h"
#include "Sequence\GameSequence\Parent.h"

using namespace std;

namespace Charactor{
	namespace Friend{
		/** BaseÆÁÉÏXÍ³µ */
		Miko::Miko(
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

		Miko::~Miko(){}

		void Miko::update(
			std::vector< Enemy::EnemyBase* >* aEnemys, 
			std::vector< FriendBase* >* aFriends, 
			Game::StageMap* aStageMap ){

				if ( mIsAwake ) {
					if ( mSkillGauge != 0 ) {
						mSkillGauge--;
					}
					if ( mSkillGauge == 0 ) {
						this->setIsAwake( false );
					}
				}

				planeUpdate( aEnemys, aFriends, aStageMap );

				for ( unsigned int i = 0; i < mShots.size(); i++ ) {
					mShots.at(i)->update();
				}
				for ( unsigned int i = 0; i < mExplosions.size(); i++ ) {
					mExplosions.at(i)->update();
				}

				for ( vector<Effect::Shot*>::iterator It = mShots.begin() ;It != mShots.end(); ) {
					/** oÁÈçqbgµ½GÊuÉj¶¬ */
					if ( mIsAwake && (*It)->getIsHit() ) {
						/** jÊuðæ¾ */
						Enemy::EnemyBase* enemy = (*It)->getTarget();
						Game::Point p = enemy->getPoint();
						Effect::Explosion* explosion;
						explosion = new Effect::Explosion( p, 1 , ( this->getRange() * 2 ) );
						/** jGtFNgðÇÁ */
						mExplosions.push_back( explosion );
						/** ­¹ÇÁ */
						int SEHandle = 
							Sequence::GameSequence::Parent::loader()->getSoundFileHandle()->at( FileNames::Sound::ExplosionSEName );
						ASSERT( PlaySoundMem( SEHandle, DX_PLAYTYPE_BACK ) + 1 );
						/** jÍÍàÌGÉ_[W */
						vector< Enemy::EnemyBase* >::iterator enemyIt = aEnemys->begin();
						for ( ; enemyIt != aEnemys->end() ; enemyIt++ ) {
							/** eªqbgµ½Gð­ */
							if ( *enemyIt == (*It)->getTarget() ) {
								continue;
							}
							int enemyRange = (*enemyIt)->getRange();
							int explosionRange = explosion->getSize() / 2;
							if ( ( *enemyIt )->getPoint().getDistance( p ) < ( enemyRange + explosionRange ) ){
								(*enemyIt)->addDamage( this->getPower() );
							}
						}
					}

					if ( !(*It)->getIsDraw() ) {
						It = mShots.erase( It );
					} else {
						It++;
					}
				}
				for ( vector<Effect::Explosion*>::iterator It = mExplosions.begin() ;It != mExplosions.end(); ) {
					if ( !(*It)->getIsDraw() ) {
						It = mExplosions.erase( It );
					} else {
						It++;
					}
				}
		}

		void Miko::draw() const{

		}

	}
}