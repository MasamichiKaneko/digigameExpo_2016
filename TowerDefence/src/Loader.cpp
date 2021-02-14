#include "Loader.h"
#include "DxLib.h"
#include "define.h"

using namespace std;



DivStatus::DivStatus(
	string aFileName,
	int aAllNum,
	int aXNum,
	int aYNum,
	int aXSize,
	int aYSize )
	: FileName( aFileName ),
	AllNum( aAllNum ),
	XNum( aXNum ),
	YNum( aYNum ),
	XSize( aXSize ),
	YSize( aYSize ){};

DivStatus::~DivStatus(){
}

int DivStatus::LoadDivGraph( DivStatus div,int* GraphHandle ){
	int status = DxLib::LoadDivGraph(
		div.FileName.c_str(),
		div.AllNum,
		div.XNum,
		div.YNum,
		div.XSize,
		div.YSize,
		GraphHandle);
	return status;
}

int DivStatus::LoadDivGraph( DivStatus* div,int* GraphHandle ){
	int status = DxLib::LoadDivGraph(
		div->FileName.c_str(),
		div->AllNum,
		div->XNum,
		div->YNum,
		div->XSize,
		div->YSize,
		GraphHandle);
	return status;
}


/**
* ここからローダークラス
*/
FileLoader::FileLoader(
	std::vector< string >* ImageFileNameList,
	std::vector< DivStatus >* DivImageFileNameList,
	std::vector< string >* TextFileNameList,
	std::vector< string >* SoundFileNameList )
	: mImageFileHandlers( 0 ),
	mDivImageFileHandlers( 0 ),
	mTextFileHandlers( 0 ),
	mSoundFileHandlers( 0 ),
	mLoadFunction( 0 ){
		/* 非同期設定 */
		SetUseASyncLoadFlag(TRUE);

		/* 画像読込み */
		if ( ImageFileNameList != 0 ) {
			mImageFileHandlers = new map<string,int>();
			for ( unsigned int i = 0; i < ImageFileNameList->size(); i++ ) {
				int graphicHandle = 0;
				/* 読み込んで */
				graphicHandle = LoadGraph( ImageFileNameList->at(i).c_str() );
				mLoadFunction++;
				/* 結果入れとく */
				if ( graphicHandle != -1 ) {
					mImageFileHandlers->insert( pair<string,int>(ImageFileNameList->at(i) , graphicHandle ));
				}
			}
		}

		/* 画像分割読込み */
		if ( DivImageFileNameList != 0 ) {
			mDivImageFileHandlers = new map<string ,vector<int> >();
			for ( unsigned int i = 0; i < DivImageFileNameList->size(); i++ ) {
				int graphicHandles[256] = {0};
				int status = DivStatus::LoadDivGraph(
					DivImageFileNameList->at(i),
					graphicHandles);
				mLoadFunction++;
				/* 結果を詰める */
				if ( status == 0 ){
					vector<int> inputHandlers;
					for ( int j = 0; j < DivImageFileNameList->at(i).AllNum; j++ ) {
						inputHandlers.push_back( graphicHandles[j] );
					}
					mDivImageFileHandlers->insert( pair<string, vector<int> >( 
						DivImageFileNameList->at(i).FileName, inputHandlers ) );
				}
			}
		}
		/* テキスト読込み */
		if ( TextFileNameList != 0 ) {
			mTextFileHandlers = new map<string,int>();
			for ( unsigned int i = 0; i < TextFileNameList->size() ; i++ ) {
				int textHandle = 0;
				textHandle = FileRead_open( TextFileNameList->at(i).c_str() );
				mLoadFunction++;
				if ( textHandle != -1 ) {
					mTextFileHandlers->insert( pair<string,int>( TextFileNameList->at(i) , textHandle ));
				}
			}
		}

		/* サウンド読込み */
		if ( SoundFileNameList != 0 ) {
			mSoundFileHandlers = new map<string, int>();
			for ( unsigned int i = 0; i < SoundFileNameList->size(); i++ ) {
				int soundHandle = 0;
				soundHandle = LoadSoundMem( SoundFileNameList->at(i).c_str() );
				mLoadFunction++;
				if ( soundHandle != -1 ) {
					mSoundFileHandlers->insert( pair<string,int>( SoundFileNameList->at(i) , soundHandle ));
				}
			}
		}
		SetUseASyncLoadFlag( FALSE ) ;
}

FileLoader::~FileLoader(){
	/** 残った読み込みメモリを開放 */
	deleteImageHandleAll();
	deleteTextHandleAll();
	deleteSoundHandleAll();
	deleteDivImageHandleAll();

	SAFE_DELETE( mImageFileHandlers );
	SAFE_DELETE( mTextFileHandlers );
	SAFE_DELETE( mSoundFileHandlers );
	SAFE_DELETE( mDivImageFileHandlers );
}

bool FileLoader::deleteTextHandle( string aFileName ) {
	bool result = false;
	map< string, int >::iterator it = mTextFileHandlers->find( aFileName );
	if ( it != mTextFileHandlers->end() ) {
		ASSERT( FileRead_close( it->second ) + 1 );
		mTextFileHandlers->erase( it );
		result = true;
	}
	return result;
}

bool FileLoader::deleteImageHandle( string aFileName ) {
	bool result = false;
	map< string, int >::iterator it = mImageFileHandlers->find( aFileName );
	if ( it != mImageFileHandlers->end() ) {
		ASSERT( DeleteGraph( it->second ) + 1 );
		mImageFileHandlers->erase( it );
		result = true;
	}
	return result;
}

bool FileLoader::deleteSoundHandle( string aFileName ) {
	bool result = false;
	map< string, int >::iterator it = mSoundFileHandlers->find( aFileName );
	if ( it != mSoundFileHandlers->end() ) {
		ASSERT( DeleteSoundMem( it->second ) + 1 );
		mSoundFileHandlers->erase( it );
		result = true;
	}
	return result;
}

bool FileLoader::deleteDivImageHandle( string aFileName ) {
	bool result = false;
	map< string, vector<int> >::iterator it = mDivImageFileHandlers->find( aFileName );
	if ( it != mDivImageFileHandlers->end() ) {
		for ( unsigned int i = 0; i < it->second.size(); i++ ) {
			ASSERT( DeleteGraph( it->second.at( i ) ) + 1 );
		}
		mDivImageFileHandlers->erase( it );
		result = true;
	}
	return result;
}

void FileLoader::deleteTextHandleAll(){
	map< string, int >::iterator it = mTextFileHandlers->begin();
	for ( ; it != mTextFileHandlers->end() ; ) {
		ASSERT( FileRead_close( it->second ) + 1 );
		it = mTextFileHandlers->erase( it );
	}
}

void FileLoader::deleteImageHandleAll(){
	map< string, int >::iterator it = mImageFileHandlers->begin();
	for ( ; it != mImageFileHandlers->end() ; ) {
		ASSERT( DeleteGraph( it->second ) + 1 );
		it = mImageFileHandlers->erase( it );
	}
}

void FileLoader::deleteSoundHandleAll(){
	map< string, int >::iterator it = mSoundFileHandlers->begin();
	for ( ; it != mSoundFileHandlers->end() ; ) {
		ASSERT( DeleteSoundMem( it->second ) + 1 );
		it = mSoundFileHandlers->erase( it );
	}
}

void FileLoader::deleteDivImageHandleAll(){
	map< string, vector< int > >::iterator it = mDivImageFileHandlers->begin();
	for ( ; it != mDivImageFileHandlers->end() ; ) {
		for ( unsigned int i = 0 ;i < it->second.size() ; i++ ) {
			ASSERT( DeleteGraph( it->second.at( i ) ) + 1 );
		}
		it = mDivImageFileHandlers->erase( it );
	}
}

bool FileLoader::CheckFileRead(){
	int status = GetASyncLoadNum();
	return ( status == 0 );
}