#pragma once

#include "Charactor\Enemy\EnemyBase.h"

namespace Charactor{
	namespace Enemy{
		class Ocj : public EnemyBase{
		public:
			Ocj(
				int aId,                 //id
				int aLevel,              //���x��
				int aHitPoint,           //HP
				int aSpeed,              //���x
				int aScore,              //���������Z�X�R�A
				int aAssets,             //���������Z����
				int aDamage,             //���B�����Q�l
				int aRangeRadius,        //�����蔻�蔼�a
				int aAdventStartFrame,   //�o���J�n�t���[��
				std::vector< int > aRoot,//�o�H���
				Game::StageMap* aStage,        //�}�b�v���
				double aResistStun,       //�X�^���ϐ�
				double aResistSlow        //�X���E�ϐ�
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