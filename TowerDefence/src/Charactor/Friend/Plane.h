#pragma once

#include "Charactor\Friend\FriendBase.h"

namespace Effect{
	class Shot;
}

namespace Charactor{

	namespace Friend{
		class Plane : public FriendBase{
		public:
			Plane(
				int aSkillPoint,
				int aPower, 
				int aSpeed,
				int aRange,
				int aId,
				int aLevel,
				Game::Point p);

			~Plane();

			void update( 
				std::vector< Enemy::EnemyBase* >* aEnemys, 
				std::vector< FriendBase* >* aFriends, 
				Game::StageMap* aStageMap );
			void draw() const;
		};
	}
}