#pragma once

#include "Charactor\Enemy\EnemyBase.h"

namespace Charactor{
	namespace Enemy{
		class Ocj : public EnemyBase{
		public:
			Ocj(
				int aId,                 //id
				int aLevel,              //レベル
				int aHitPoint,           //HP
				int aSpeed,              //速度
				int aScore,              //討伐時加算スコア
				int aAssets,             //討伐時加算資金
				int aDamage,             //到達時損害値
				int aRangeRadius,        //当たり判定半径
				int aAdventStartFrame,   //出現開始フレーム
				std::vector< int > aRoot,//経路情報
				Game::StageMap* aStage,        //マップ情報
				double aResistStun,       //スタン耐性
				double aResistSlow        //スロウ耐性
				);
			
			~Ocj();
			
			void update( 
				std::vector< EnemyBase* >* aEnemys,
				std::vector< Friend::FriendBase* >* aFriends,
				Game::StageMap* aStageMap );

			void draw() const;

		private:
		};
	}
}