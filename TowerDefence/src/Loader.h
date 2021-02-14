#pragma once

/**
* ファイルローダークラス
* ロード画面動かしつつ一片に読み込む場合はこいつで
*/

#include<map>
#include<vector>
#include<string>

/**
 * 分割読込み用ステータスクラス
 * FileLoaderに分割画像ファイルを食わせるとき用
 */
class DivStatus{
public:
	DivStatus(
		std::string aFileName,
		int aAllNum,
		int aXNum,
		int aYNum,
		int aXSize,
		int aYSize );
	~DivStatus();

	static int LoadDivGraph(DivStatus div,int* GraphHandle);
	static int LoadDivGraph(DivStatus* div,int* GraphHandle);

	/* FileLoaderのみprivateへアクセス可 */
	friend class FileLoader;

private:
	DivStatus();
	std::string FileName;  //ファイル名
	int AllNum;            //総分割数
	int XNum;              //横分割数
	int YNum;              //縦分割数
	int XSize;             //横サイズ
	int YSize;             //縦サイズ
};

class FileLoader{
public:
	FileLoader(
		std::vector< std::string >* ImageFileNameList,   //画像ファイル
		std::vector< DivStatus >* DivImageFileNameList,  //画像ファイル(分割)
		std::vector< std::string >* TextFileNameList,    //テキストファイル
		std::vector< std::string >* SoundFileNameList);  //サウンドファイル

	~FileLoader();

	/**
	 * ファイル読み込み結果を返す
	 * true:全部読み込み完了 false:まだ
	 */
	bool CheckFileRead();

	/**  
	 *  画像読み込み結果を返す、ないときはnull
	 *  first:読み込みファイル名 
	 *  second:画像読み込みハンドル
	 */
	std::map< std::string ,int >* getImageFileHandle(){ return mImageFileHandlers; }
	
	/**
	 * 画像分割読込みの結果を返す、ないときはnull
	 *  first:読み込みファイル名 
	 *  second:画像読み込みハンドル
	 */
	std::map< std::string ,std::vector<int> >* getDivImageFileHanlde(){ return mDivImageFileHandlers; }

	/**  
	 *  テキスト読み込み結果を返す、ないときはnull
	 *  first:読み込みファイル名 
	 *  second:テキスト読み込みハンドル
	 */
	std::map< std::string,int >* getTextFileHandle(){ return mTextFileHandlers; }
	
	/**  
	 *  サウンド読み込み結果を返す、ないときはnull
	 *  first:読み込みファイル名 
	 *  second:サウンド読み込みハンドル
	 */
	std::map< std::string,int >* getSoundFileHandle(){ return mSoundFileHandlers; }

	/**
	 * 総読込み処理数を返す
	 */
	int getLoadFunctionTotal(){ return mLoadFunction; }

	/**
	* 読み込みハンドル削除関数
	*/
	bool deleteTextHandle( std::string aFileName );
	bool deleteImageHandle( std::string aFileName );
	bool deleteSoundHandle( std::string aFileName );
	bool deleteDivImageHandle( std::string aFileName );

	/** 全ハンドル削除 */
	void deleteTextHandleAll();
	void deleteImageHandleAll();
	void deleteSoundHandleAll();
	void deleteDivImageHandleAll();

private:
	int mLoadFunction;

	/* 渡すためのファイルデータ */
	std::map< std::string , int >* mImageFileHandlers;
	std::map< std::string , std::vector<int> >*mDivImageFileHandlers;
	std::map< std::string, int >* mTextFileHandlers;
	std::map< std::string, int >* mSoundFileHandlers;
};