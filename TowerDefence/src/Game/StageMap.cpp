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

	/** 今は仮フォーマットなのでテキトーに */
	/** ToDo: エラーチェックとかまだ */
	bool StageMap::readMapData( int fileHandle ){

		bool result = false;
		/** ハンドルから文字列受け取り */
		vector<string> mapDataStr;
		ASSERT( FileRead_getAll( &mapDataStr , fileHandle ) );

		char_separator<char> sep(",");
		typedef tokenizer< char_separator<char> >::iterator tokenIt;

		/** サイズ取得 */
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

		/** マップデータ取得 */
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
				/** id取得 */
				mapChipBuf.setId( lexical_cast< int >( *it ) );
				/** 初期状態は設置無し */
				mapChipBuf.setIsOnCharactor( false );
				mapChipId.push_back( mapChipBuf );
			}
			mapIt++;
		}


		/** マップルート取得 */
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