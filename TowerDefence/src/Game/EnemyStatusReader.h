#pragma once

/**
* �G�L�����X�e�[�^�X�ǂݍ��݃N���X
*/

#include <vector>
#include <string>
#include <map>


namespace Charactor{
	namespace Enemy{
		class EnemyBase;
	}
}


namespace Game{
	class StageMap;
	class EnemyStatusReader{
	public:

		EnemyStatusReader( const char* EnemyStatusDataFileName, const char* EnemyAppearanceDataFileName, StageMap* aMap );
		EnemyStatusReader( const int EnemyStatusDataFileName, const int EnemyAppearanceDataFileName, StageMap* aMap );
		~EnemyStatusReader(){}

		std::vector< Charactor::Enemy::EnemyBase* > getEnemysData()const{ return mEnemys; }

	private:

		typedef struct {
			int hitPoint;
			int speed;
			int score;
			int asset;
			int assetDamage;
			int range;
			double stunRegist;
			double slowRegist;
		}Status;

		/** �X�e�[�^�X�ǂݍ��݊֐� */
		bool readEnemyStatusData( const char* EnemyStatusDataFileName );
		bool readEnemyStatusData( const int EnemyStatusDataFileHandle );
		/** �o�����ǂݍ��݊֐� */
		bool readEnemyAppearanceData( const char* EnemyAppearanceDataFileName, StageMap* aMap );
		bool readEnemyAppearanceData( const int EnemyAppearanceDataFileHandle, StageMap* aMap );

		/** key:id value:(key:�X�e�[�^�X�^�C�v value:�X�e�[�^�X�l) */
		std::map< int , Status > mReadStatusData;
		
		std::vector< Charactor::Enemy::EnemyBase* > mEnemys;
	};
}