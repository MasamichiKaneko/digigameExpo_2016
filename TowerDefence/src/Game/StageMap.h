#pragma once

/**
 * �}�b�v�N���X
 */
#include "Array2D.h"
#include <vector>

namespace Game{

	class StageMap{
	public:
		/* Loader�œǂݍ��񂾃}�b�v�t�@�C���̃n���h����n�� */
		StageMap( int fileHandle );
		~StageMap();
	

		/* �}�b�v��� */
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

			/* �`�b�vID */
			int chipId;
			/* �L�����ݒu�t���O */
			bool isOnCharactor;
		};

		/** ���[�g��� */
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
		* �ǂݍ��݊֐�
		* �t�H�[�}�b�g���莟���蒼���A���͉��t�H�[�}�b�g��
		*/
		bool readMapData( int fileHandle );

		int mChipSize;
		int mSizeX, mSizeY;
		int mStageNum;
		std::vector< Route > mRoot;
		std::vector< MapChip > mMapArray;
	};

}