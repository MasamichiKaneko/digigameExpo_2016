#include "Effect\Shot.h"
#include "Charactor\Enemy\EnemyBase.h"
#include "Sequence\GameSequence\Parent.h"
#include "FileNames.h"
#include "define.h"
#include "DxLib.h"
/** ’e§Œä */

namespace Effect{

	const int Shot::mSpeed = 10;

	Shot::Shot( 
		Game::Point aPoint,
		int aId,
		int aStun,
		int aSlow,
		int aSlip,
		int aSize,
		int aDamage,
		Charactor::Enemy::EnemyBase* mTarget
		) : 
	Effect::EffectBase(
		aPoint,
		aId,
		aSize ),
	mTargetEnemy( mTarget ),
	mDamage( aDamage ),
	mStun( aStun ),
	mSlow( aSlow ),
	mSlip( aSlip ),
	mIsHit( false ){
	}

	void Shot::update(){

		if ( !mIsDraw ) {
			return;
		}

		if ( !mTargetEnemy->getAppearance() || !mTargetEnemy->getSurvive() ) {
			mIsDraw = false;
			return;
		}

		if ( mPoint.getDistance( mTargetEnemy->getPoint() ) <= mTargetEnemy->getRange() ) {
			int SEHandle = 
				Sequence::GameSequence::Parent::loader()->getSoundFileHandle()->at( FileNames::Sound::ShotSEName );
			ASSERT( PlaySoundMem( SEHandle, DX_PLAYTYPE_BACK ) + 1 );
			mTargetEnemy->addDamage( mDamage );
			if ( mSlow != 0 ) {
				mTargetEnemy->setSlow( mSlow );
			}
			if ( mStun != 0 ) {
				mTargetEnemy->setStun( mStun );
			}
			if ( mSlip != 0 ) {
				mTargetEnemy->setSlip( mSlip );
			}
			mIsHit = true;
			mIsDraw = false;
		} else {
			/** Œü‚«ŒvZ */
			int targetPosX = mTargetEnemy->getPoint().getX();
			int targetPosY = mTargetEnemy->getPoint().getY();
			double angle = atan2( static_cast<double>( targetPosY - mPoint.getY() ),
				static_cast<double>( targetPosX - mPoint.getX() ) );
			/** ˆÚ“®‹——£Zo */
			double moveX = ( mSpeed*cos(angle) ) + 0.5;
			double moveY = ( mSpeed*sin(angle) ) + 0.5;
			/** À•WˆÚ“® */
			mPoint.setX( mPoint.getX() + static_cast< int >( moveX ) );
			mPoint.setY( mPoint.getY() + static_cast< int >( moveY ) );
		}
	}
}