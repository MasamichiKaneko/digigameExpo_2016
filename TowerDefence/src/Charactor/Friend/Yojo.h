#pragma once

#include "Charactor\Friend\FriendBase.h"

namespace Charactor{

	namespace Friend{
		class Yojo : public FriendBase{
		public:
			Yojo(
				int aSkillPoint,
				int aPower, 
				int aSpeed,
				int aRange,
				int aId,
				int aLevel,
				Game::Point p);

			~Yojo();

			void update(
				std::vector< Enemy::EnemyBase* >* aEnemys, 
				std::vector< FriendBase* >* aFriends, 
				Game::StageMap* aStageMap );

			void draw() const;

		};

	}
}