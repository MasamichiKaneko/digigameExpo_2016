#pragma once

/**
* 敵キャラステータス読み込みクラス
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

		/** ステータス読み込み関数 */
		bool readEnemyStatusData( const char* EnemyStatusDataFileName );
		bool readEnemyStatusData( const int EnemyStatusDataFileHandle );
		/** 出現情報読み込み関数 */
		bool readEnemyAppearanceData( const char* EnemyAppearanceDataFileName, StageMap* aMap );
		bool readEnemyAppearanceData( const int EnemyAppearanceDataFileHandle, StageMap* aMap );

		/** key:id value:(key:ステータスタイプ value:ステータス値) */
		std::map< int , Status > mReadStatusData;
		
		std::vector< Charactor::Enemy::EnemyBase* > mEnemys;
	};
}