#pragma once

void halt( const char* filename, int line, const char* message );

/**
* �}�N���u����
*/
//�A�T�[�V�����`�F�b�N
//0��false�A�ʂ�ۂ��n�����Ƌ����I��
#define ASSERT( exp ) ( ( !! ( exp ) ) || ( halt( __FILE__, __LINE__, #exp ), 0 ) )

//delete�֌W
#define SAFE_DELETE( x ) { if(x != 0) {delete ( x ); ( x ) = 0;} }
#define SAFE_DELETE_ARRAY( x ) { if(x != 0){ delete[]( x ); ( x ) = 0;} }
