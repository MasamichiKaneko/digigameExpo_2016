#pragma once

void halt( const char* filename, int line, const char* message );

/**
* マクロ置き場
*/
//アサーションチェック
//0やfalse、ぬるぽが渡されると強制終了
#define ASSERT( exp ) ( ( !! ( exp ) ) || ( halt( __FILE__, __LINE__, #exp ), 0 ) )

//delete関係
#define SAFE_DELETE( x ) { if(x != 0) {delete ( x ); ( x ) = 0;} }
#define SAFE_DELETE_ARRAY( x ) { if(x != 0){ delete[]( x ); ( x ) = 0;} }
