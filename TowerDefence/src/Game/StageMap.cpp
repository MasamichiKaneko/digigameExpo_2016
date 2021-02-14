#include "Game\StageMap.h"
#include "define.h"
#include "utils.h"
#include <vector>
#include <string>
#include <boost\tokenizer.hpp>
#include <boost\lexical_cast.hpp>
#include <algorithm>

using namespace std;
using namespace boost;

namespace Game{
	StageMap::StageMap( int fileHandle ){
		ASSERT(readMapData( fileHandle ));
	}

	StageMap::~StageMap(){
	}

	/** ���͉��t�H�[�}�b�g�Ȃ̂Ńe�L�g�[�� */
	/** ToDo: �G���[�`�F�b�N�Ƃ��܂� */
	bool StageMap::readMapData( int fileHandle ){

		bool result = false;
		/** �n���h�����當����󂯎�� */
		vector<string> mapDataStr;
		ASSERT( FileRead_getAll( &mapDataStr , fileHandle ) );

		char_separator<char> sep(",");
		typedef tokenizer< char_separator<char> >::iterator tokenIt;

		/** �T�C�Y�擾 */
//		int sizeX,sizeY;
		vector<string>::iterator mapIt = find(mapDataStr.begin(),mapDataStr.end(),"[size]");
		if (mapIt != mapDataStr.end() ){
	//		goto FUNC_END;
		}
		mapIt++;
		tokenizer<char_separator< char >>SizeTokens( *mapIt, sep );
		{
			tokenIt it = SizeTokens.begin();
			mSizeX = lexical_cast< int >( *it );
			it++;
			mSizeY = lexical_cast< int >( *it );
		}

		/** �}�b�v�f�[�^�擾 */
		vector< MapChip > mapChipId;
		mapIt = find( mapDataStr.begin(),mapDataStr.end(),"[map]" );
		if (mapIt != mapDataStr.end() ){
	//		goto FUNC_END;
		}
		mapIt++;

		for ( int y = 0; y < mSizeY; y++ ) {
			MapChip mapChipBuf;
			tokenizer<char_separator< char >>MapTokens( *mapIt, sep );
			tokenIt it = MapTokens.begin();
			for ( tokenIt it = MapTokens.begin(); it != MapTokens.end(); it++ ) {
				/** id�擾 */
				mapChipBuf.setId( lexical_cast< int >( *it ) );
				/** ������Ԃ͐ݒu���� */
				mapChipBuf.setIsOnCharactor( false );
				mapChipId.push_back( mapChipBuf );
			}
			mapIt++;
		}


		/** �}�b�v���[�g�擾 */
		vector<Route>route;
		mapIt = find( mapDataStr.begin(), mapDataStr.end(),"[route]" );
		if (mapIt != mapDataStr.end() ){
	//		goto FUNC_END;
		}
		mapIt++;
		for ( ; mapIt != mapDataStr.end() ; mapIt++ ) {
			Route routeBuf;
			tokenizer<char_separator< char >>RouteTokens( *mapIt, sep );
			tokenIt it = RouteTokens.begin();
			routeBuf.x = lexical_cast< int >( *it );
			it++;
			routeBuf.y = lexical_cast< int >( *it );
			route.push_back( routeBuf );
		}

		mRoot = route;
		mMapArray = mapChipId;
		mChipSize = 32;
		mStageNum = 1;

		result = true;
//FUNC_END:
		return result;

	}
}