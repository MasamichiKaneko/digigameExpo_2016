#pragma once

#include "Charactor\Friend\FriendBase.h"

namespace Charactor{

	namespace Friend{
		class Majokko : public FriendBase{
		public:
			Majokko(
				int aSkillPoint,
				int aPower, 
				int aSpeed,
				int aRange,
				int aId,
				int aLevel,
				Game::Point p);

			~Majokko();

			void update(
				std::vector< Enemy::EnemyBase* >* aEnemys, 
				std::vector< FriendBase* >* aFriends, 
				Game::StageMap* aStageMap );

			void draw() const;
		private:
			/** バステ値(火力依存) */
			int mBadStatusValue;
		};

	}
}