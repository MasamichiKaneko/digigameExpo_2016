#pragma once

/**
 * マップクラス
 */
#include "Array2D.h"
#include <vector>

namespace Game{

	class StageMap{
	public:
		/* Loaderで読み込んだマップファイルのハンドルを渡す */
		StageMap( int fileHandle );
		~StageMap();
	

		/* マップ情報 */
		class MapChip{
		public:
			MapChip(){}
			~MapChip(){}

			int  getId()const{ return chipId; }
			bool getIsOnCharactor()const{ return isOnCharactor; }

			friend class StageMap;
		private:

			void setId( int aId ){ chipId = aId; }
			void setIsOnCharactor( bool aIsOnChara ){ isOnCharactor = aIsOnChara; }

			/* チップID */
			int chipId;
			/* キャラ設置フラグ */
			bool isOnCharactor;
		};

		/** ルート情報 */
		struct Route{
			int x,y;
		};

		int getChipSize() const { return mChipSize; }
		
		int getMapSizeX() const { return mSizeX; }

		int getMapSizeY() const { return mSizeY; }

		const std::vector<Route>* getRoot() const{
			return &mRoot;
		}
		MapChip getMapInfo( int x, int y ){ return mMapArray.at( x + y * mSizeX ); }
		void setOnFriendFlag( int x, int y, bool friendFlag ){ mMapArray.at( x + y * mSizeX ).setIsOnCharactor(friendFlag); }

		std::vector< MapChip >getMapArray()const { return mMapArray; }
	private:
		/**
		* 読み込み関数
		* フォーマット決定次第作り直す、今は仮フォーマットで
		*/
		bool readMapData( int fileHandle );

		int mChipSize;
		int mSizeX, mSizeY;
		int mStageNum;
		std::vector< Route > mRoot;
		std::vector< MapChip > mMapArray;
	};

}