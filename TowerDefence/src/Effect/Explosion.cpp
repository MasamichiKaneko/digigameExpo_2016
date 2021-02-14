#include "Effect\Explosion.h"
#include "CommonConst.h"


namespace Effect{

	const int Explosion::switchingFPS = 6;

	Explosion::Explosion(
			Game::Point aPoint,
			int aId,
			int aSize )
			: EffectBase(
			aPoint,
			aId,
			aSize),
			mUpdateCallNum( 0 ){}

	void Explosion::update(){
		mUpdateCallNum++;

		if ( CommonConst::ExplosionDivNumAll <= ( mUpdateCallNum / switchingFPS ) ) {
			mIsDraw = false;
		} else {
			mPanel = ( mUpdateCallNum / switchingFPS );
		}
	}

}