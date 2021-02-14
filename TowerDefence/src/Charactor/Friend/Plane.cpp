#include "Charactor\Friend\Plane.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "CommonConst.h"
#include "Effect\Shot.h"
#include "define.h"
using namespace std;

namespace Charactor{
	namespace Friend{
		/** BaseÇ∆ì¡Ç…ïœçXÇÕñ≥Çµ */
		Plane::Plane(
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
		
		Plane::~Plane(){}


		void Plane::update(
			vector< Enemy::EnemyBase* >* aEnemys, 
			vector< FriendBase* >* aFriends, 
			Game::StageMap* aStageMap ){

				if ( mIsAwake ) {
					this->setPowerRatio( 1.5 );
					this->setSpeedRatio( 1.5 );
					if ( mSkillGauge != 0 ) {
						mSkillGauge--;
					}
					if ( mSkillGauge == 0 ) {
						this->setIsAwake( false );
					}
				} else {
					this->setPowerRatio( 1.0 );
					this->setSpeedRatio( 1.0 );
				}

				planeUpdate( aEnemys, aFriends, aStageMap );

				for ( unsigned int i = 0; i < mShots.size(); i++ ) {
					mShots.at(i)->update();
				}
				vector<Effect::Shot*>::iterator It;
				It = mShots.begin();
				for ( ;It != mShots.end(); ) {
					if ( !(*It)->getIsDraw() ) {
						It = mShots.erase( It );
					} else {
						It++;
					}
				}
		}

		void Plane::draw()const{
		}
	}
}