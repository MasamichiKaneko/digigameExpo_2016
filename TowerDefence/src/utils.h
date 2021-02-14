#include <vector>
#include <map>
#include <string>
#include <XMLNodeNameString.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/exception/all.hpp>

/**
* 小物置き場
* ちょっとした関数はここに追加する
*/
#pragma once

/**
* キーを1回だけ押す処理
* [引数]
* int InputKey:キーコード
* [戻り値]
* true :押している 
* false:押していないor前フレームから押しっぱなし
*/
bool inputKeyTriggered( int CheckHitKey );

/**
 * 読み込んだテキストファイルの中身を全部取得する処理
 * [引数]
 * vector<string>* output :読み込んだテキストの中身を格納する領域(1要素ごとに1行読み込む)
 * int FileHandle         :ファイルハンドル
 * [戻り値]
 * true : 正常終了
 * false: エラー発生
 */
bool FileRead_getAll( std::vector<std::string>* output,int FileHandle );

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
bool FileRead_getAllConnectedStr( std::string* output, int FileHandle );
bool FileRead_getAllConnectedStr( std::stringstream* output, int FileHandle );

/**
 * 文字列vectorの中身を全部ファイルに出力する処理
 * [引数]
 * vector<string>* output :出力する文字列vector
 * const char* FileName         :出力ファイル名
 * [戻り値]
 * なし
 */
void FileWrite_vectorAll( std::vector<std::string> output, const char* FileName );

/**
* マウス入力を1回だけする処理
* [引数]
* int InputMouse:マウス入力
* [戻り値]
* true :入力を検知
* false:入力なしor前フレームから入力しっぱなし
*/
bool inputMouseTriggered(int InputMouse);

/**
* xml読み込み関数
* [引数]
* const char* fileName:ファイル名
* ptree &o_pt: 読み込みプロパティツリー
* [戻り値]
* true:読み込み成功
* false:失敗
*/
bool readXml( const char* fileName, boost::property_tree::ptree &o_pt );

/**
* [引数]
* const int fileHandle:ファイルハンドル
* ptree &o_pt: 読み込みプロパティツリー
*/
bool readXml( const int fileHandle, boost::property_tree::ptree &o_pt );

/**
 * グリッド描画関数
 */
void drawGrid( 
	int posX,      int posY,      //描き始め座標
	int numH,      int numV,      //本数(H:水平 , V:垂直)
	int intervalH, int intervalV, //間隔
	int color   );                //線の色


/**
* ノードの属性を読み込む関数
* 
*/
template <typename T> bool readAttr( 
	std::string targetNodeName, 
	std::map<std::string, T>& o_attrValues,
	boost::property_tree::ptree &i_pt,
	std::vector<std::string> attrNames ){
		bool result = true;
		boost::optional<T> readBuf;

		for ( unsigned int i = 0; i < attrNames.size(); i++ ) {
			/** 読み込む */
			std::string readAttr = XmlCommon::attrStr + attrNames.at(i);
			readBuf = i_pt.get_optional<T>( targetNodeName + "."+ readAttr );

			if ( !readBuf ){
				o_attrValues.clear();
				result = false;
				goto FUNC_END;
			}

			o_attrValues[attrNames.at(i)] = *readBuf;
		}

FUNC_END:
		return result;
}


/**
* int型変数を0詰めで指定桁数にそろえる処理
* [引数]
* int aValue: 対象の変数
* int digit : 指定桁数
* [戻り値]
* string型 0を詰めた後の文字列
*/
std::string getZeroPadding( int aValue, int digit );

/**
* フェードイン、フェードアウトの光度処理クラス
* 別ファイルに切り出すかも
*/
class FadeInFadeOut{
public:
	FadeInFadeOut(
		float aFadefloatime,
		float aWaitfloatime,
		float aFadeOutTime,
		float aWaitOutTime );
	~FadeInFadeOut();

	enum Mode{
		FADE_IN,  //フェードイン
		WAIT_IN,  //フェードイン完了後待ち時間
		FADE_OUT, //フェードアウト
		WAIT_OUT, //フェードアウト完了後待ち時間
		END       //終了
	};

	Mode getMode()const{	return mMode; }
	int getParameter() const { return mParameter; }

	/** 中断用 */
	void setMode( Mode aMode );

	/** パラメータを更新 */
	void UpdateParameter();
private:
	FadeInFadeOut();
	Mode mMode;
	int mTime;
	int mParameter;
	float mFadeInTime;
	float mWaitInTime;
	float mFadeOutTime;
	float mWaitOutTime;
};




