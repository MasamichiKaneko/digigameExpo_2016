#include "Charactor\Friend\Majokko.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "Effect\Shot.h"
using namespace std;


namespace Charactor{
	namespace Friend{
		/** Base�Ƀo�X�e�l�ǉ� */
		Majokko::Majokko(
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
			p),
		mBadStatusValue( aPower * aPower ) {
		}

		Majokko::~Majokko(){}

		void Majokko::update(
			std::vector< Enemy::EnemyBase* >* aEnemys, 
			std::vector< FriendBase* >* aFriends, 
			Game::StageMap* aStageMap ){

				/** �X���E�ƃX�g�b�v�̐؂�ւ� */
				if ( mIsAwake ) {
					this->mStun = mBadStatusValue;
					mSlow = 0;
					if ( mSkillGauge != 0 ) {
						mSkillGauge--;
					}
					if ( mSkillGauge == 0 ) {
						this->setIsAwake( false );
					}
				} else {
					mSlow = mBadStatusValue;
					mStun = 0;
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

		void Majokko::draw() const{
		}

	}
}