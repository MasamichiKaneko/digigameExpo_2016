#include "utils.h"
#include "DxLib.h"
#include "Configuration.h"
#include "define.h"
#include <sstream>
#include <fstream>
#include <iomanip>

/**
* キーを1回だけ押す処理
* [引数]
* int InputKey:キーコード
* [戻り値]
* true :押している 
* false:押していないor前フレームから押しっぱなし
*/
bool inputKeyTriggered( int InputKey ) {
	static bool ret[ 256 ] = {false};
	char Buf[256];
	GetHitKeyStateAll(Buf);
	if ( Buf[InputKey] == 1 ) {
		if ( !ret[InputKey] ) {
			ret[InputKey] = true;
			return true;
		} else {
			return false;
		}
	} else {
		ret[InputKey] = false;
	}
	return false;
}

/**
* マウス入力を1回だけする処理
* [引数]
* int InputMouse:マウス入力
* [戻り値]
* true :入力を検知
* false:入力なしor前フレームから入力しっぱなし
*/
bool inputMouseTriggered(int InputMouse) {
	static bool ret[ 0x0080 ] = {false};
	int mouseInput = GetMouseInput();
	if ( mouseInput == InputMouse ) {
		if ( !ret[ InputMouse ] ) {
			ret[InputMouse] = true;
			return true;
		} else {
			return false;
		}
	} else {
		ret[ InputMouse ] = false;
	}
	return false;
}

/**
 * 読み込んだテキストファイルの中身を全部取得する処理
 * [引数]
 * vector<string>* output :読み込んだテキストの中身を格納する領域(1要素ごとに1行読み込む)
 * int FileHandle         :ファイルハンドル
 * [戻り値]
 * true : 正常終了
 * false: エラー発生
 */
bool FileRead_getAll( std::vector<std::string>* output,int FileHandle ){
	output->clear();
	while ( FileRead_eof( FileHandle )  == 0 ) {
		int bufSize = 4096;
		char buf[4096];
		int status = FileRead_gets( buf, bufSize, FileHandle );
		/* エラーチェック */
		if ( status == -1 ) {
			/* 中途半端に読み込むのはよくない */
			output->clear();
			return false;
		}
		output->push_back( buf );
	}
	return true;
}

/**
 * 読み込んだテキストファイルの中身を1つの文字列データで全部取得する処理
 * DxLibのアーカイブ機能に対応
 * [引数]
 * string* output :読み込んだテキストの中身を格納する領域
 * int FileHandle         :ファイルハンドル
 * [戻り値]
 * true : 正常終了
 * false: エラー発生
 */
bool FileRead_getAllConnectedStr( std::string* output, int FileHandle ){
	output->clear();
	std::stringstream readStr;
	bool result = FileRead_getAllConnectedStr(&readStr, FileHandle);
	if ( result ) {
		output = &readStr.str();
	}
	return result;
}

bool FileRead_getAllConnectedStr( std::stringstream* output, int FileHandle ){

	output->clear();
	std::vector<std::string> readData;
	bool result = FileRead_getAll( &readData, FileHandle );

	if ( result ) {
		for ( unsigned int i = 0; i < readData.size(); i++ ) {
			*output << readData.at(i) << std::endl;
		}
	}
	return result;
}

/**
 * 文字列vectorの中身を全部ファイルに出力する処理
 * [引数]
 * vector<string>* output :出力する文字列vector
 * const char* FileName         :出力ファイル名
 * [戻り値]
 * なし
 */
void FileWrite_vectorAll( std::vector<std::string> output, const char* FileName ){
	std::ofstream ofs( FileName );
	std::stringstream readStr;

	for ( unsigned int i = 0; i < output.size(); i++ ) {
		readStr << output.at( i ) << std::endl;
	}
	ofs << readStr.str();
	ofs.close();
	return;
}

/**
* int型変数を0詰めで指定桁数にそろえる処理
* [引数]
* int aValue: 対象の変数
* int digit : 指定桁数
* [戻り値]
* string型 0を詰めた後の文字列
*/
std::string getZeroPadding( int aValue, int digit ){
	std::ostringstream oss;
	
	oss << std::setw( digit ) << std::setfill( '0' ) << aValue;

	return oss.str();
}

/**
* xml読み込み関数
* [引数]
* const char* fileName:ファイル名
* ptree &o_pt: 読み込みプロパティツリー
* [戻り値]
* true:読み込み成功
* false:失敗
*/
bool readXml( const char* fileName, boost::property_tree::ptree &o_pt ){
	bool result = false;
	std::stringstream readStr;
	/** ファイル読み込み */
	int txtHandle;
	txtHandle = FileRead_open( fileName );
	if ( txtHandle != 0 ) {
		result = readXml( txtHandle, o_pt );
		ASSERT( FileRead_close( txtHandle ) + 1 );
	}
	return result;
}

bool readXml( const int fileHandle, boost::property_tree::ptree &o_pt ){
	bool result = true;
	std::stringstream readStr;
	/** 読み込みチェック */
	if ( fileHandle == -1 ) {
		result = false;
		goto FUNC_END;
	}
	
	/** 文字列読み込み */
	ASSERT( FileRead_getAllConnectedStr( &readStr,fileHandle ) );

	/** xml読み込み */
	try{
		boost::property_tree::read_xml( readStr ,o_pt );
	} catch( boost::exception& ) {
		result = false;
	}

FUNC_END:
	return result;
}

/**
 * グリッド描画関数
 */
void drawGrid( 
	int posX,      int posY,      //描き始め座標
	int numH,      int numV,      //本数(H:水平 , V:垂直)
	int intervalH, int intervalV, //間隔
	int color   ){                //色
		/** 線の長さ計算 */
		int lengthH = intervalV * ( numV - 1 );
		int lengthV = intervalH * ( numH - 1 );

		/** 水平方向の線描画 */
		for ( int i = 0; i < numH; i++ ) {
			ASSERT( DrawLine( 
				posX, 
				posY + ( i * intervalH ), 
				posX + lengthH, 
				posY + ( i * intervalH ), 
				color ) + 1 );
		}
		/** 垂直方向の線描画 */
		for ( int i = 0; i < numV; i++ ) {
			ASSERT( DrawLine( 
				posX + ( i * intervalV ),
				posY, 
				posX + ( i * intervalV ),
				posY + lengthV,
				color ) + 1 );
		}
}

/**
* 画像のフェードイン、フェードアウト処理クラス
*/
FadeInFadeOut::FadeInFadeOut(
	float aFadeInTime,
	float aWaitInTime,
	float aFadeOutTime,
	float aWaitOutTime ) :
mFadeInTime( aFadeInTime ),
	mWaitInTime( aWaitInTime ),
	mFadeOutTime( aFadeOutTime ),
	mWaitOutTime( aWaitOutTime ),
	mMode( FADE_IN ),
	mTime( 0 ),
	mParameter( 0 )
{}

FadeInFadeOut::~FadeInFadeOut(){
	byte brightMax = 255;
	SetDrawBright(brightMax, brightMax, brightMax);
}

void FadeInFadeOut::UpdateParameter(){
	/* フレームレート取得 */
	int rate = Configuration::getInstance()->getFrameRate();
	
	const byte brightMax = 255;
	int drawStatus = 0;

	/* 1fごとの明るさ上昇割合設定値 */
	float crr = 0.0;
	/* 明るさ設定値 */
	
	switch( mMode ){
	case FADE_IN:
		/* 1fごとの明るさ上昇割合設定 */
		crr = ( static_cast<float>( mTime ) / static_cast<float>( rate ) )
			* static_cast<float>( 1.0 / mFadeInTime );
		/* 明るさ設定 */
		mParameter = static_cast<int>( brightMax * ( crr ) );
		if ( mTime == static_cast< int >( rate * mFadeInTime ) ) {
			mTime = 0;
			mMode = WAIT_IN;
		} else {
			mTime++;
		}
		break;
	case WAIT_IN:
		if (mTime == static_cast<int>( rate * mWaitInTime )) {
			mTime = 0;
			mMode = FADE_OUT;
		} else {
			mTime++;
		}
		break;
	case FADE_OUT:
		/* 1fごとの明るさ減少割合設定 */
		crr = ( static_cast<float>( mTime ) / static_cast<float>( rate ) )
			*static_cast<float>( 1.0 / mFadeOutTime );
		/* 明るさ設定 */
		mParameter = brightMax - static_cast<int>( brightMax * ( crr ) );
		if ( mTime == static_cast< int >( rate * mFadeOutTime ) ) {
			mTime = 0;
			mMode = WAIT_OUT;
		} else {
			mTime++;
		}
		break;
	case WAIT_OUT:
		drawStatus = SetDrawBright(0, 0, 0);
		if ( mTime == rate * mWaitOutTime ) {
			mTime = 0;
			mMode = END;
		} else {
			mTime++;
		}
		break;
	case END:
		break;
	default:
		break;
	}

}

void FadeInFadeOut::setMode( Mode aMode ){
	mMode = aMode;
}
