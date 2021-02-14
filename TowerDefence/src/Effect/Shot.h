#pragma once
#include "Effect\EffectBase.h"
#include <vector>

namespace Charactor{
	namespace Enemy{
		class EnemyBase;
	}
}


namespace Effect{
	class Shot : public EffectBase{
	public:
		Shot( 
			Game::Point aPoint,
			int aId,
			int aStun,
			int aSlow,
			int aSlip,
			int aSize,
			int aDamage,
			Charactor::Enemy::EnemyBase* mTarget
			);
		~Shot(){}

		Charactor::Enemy::EnemyBase* getTarget() const{ return mTargetEnemy; }
		
		bool getIsHit(){ return mIsHit; }
		void update();
	private:
		Charactor::Enemy::EnemyBase* mTargetEnemy;
		bool mIsHit;
		int mDamage;
		int mStun;
		int mSlow;
		int mSlip;
		static const int mSpeed;
	};
}