#pragma once
/** 共通処理クラス */
#include <string>
#include <vector>

using namespace std;

namespace Sequence{

class CommonFunc{
public:
	static void create();
	static void destroy();
	static CommonFunc* getInstance();
	/** マウスポインタ描画 */
	void drawMousePointer() const;
	/** フレーム描画  */
	void drawSelectFrame( int posX,int posY,std::string drawString ) const;
	/** 画像の中心に文字を表示して描画 */
	static void drawStringOnImg( int posX, int posY, int ImgHandle, int fontColor, int fontHandle, std::string drawString );
	/** フレーム上にマウスポインタが存在するかを判定 */
	bool isMousePointOnFrame( int framePosX,int framePosY );
	/** 画像上にマウスポインタが存在するかを判定 */
	static bool isMousePointOnImage( int imgPosX, int imgPosY, int imgHandle );
	/** 複数のフレーム描画 */
	void drawMultiSelectFrame( 
		int posX,int posY,            //最初のフレーム位置
		int intervalX,                //描画間隔(x方向)
		int intervalY,                //描画間隔(y方向)
		vector<string> modeNameArray, //描画文字
		bool isMouseDicision          //マウス判定有無
		) ;
	/** 選択中フレーム取得 */
	/** 描画してること前提だが描画してないものを選択する場合が特殊なので考慮しない */
	int getSelectFrame()const { return mSelectFrameNum; }


private:
	CommonFunc();
	~CommonFunc();
	static CommonFunc* instance;
	/** 画像ハンドル */
	int mMousePointerImgHandle;
	int mSelectFrameHandle;
	int mFontHandle;
	std::vector< int > mStageIcons;

	/** 選択フレームナンバー */
	int mSelectFrameNum;
};

}