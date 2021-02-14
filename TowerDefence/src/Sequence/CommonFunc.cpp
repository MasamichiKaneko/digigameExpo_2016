#include <string>
#include "Sequence\CommonFunc.h"
#include "FileNames.h"
#include "DxLib.h"
#include "define.h"


using namespace std;

namespace Sequence{

/** 共通処理クラス */
CommonFunc* CommonFunc::instance = 0;

/* 表示文字サイズ */
const int FontSize = 18;
const int FontColor = GetColor(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);
const char* FontStyle = "HGS行書体";

CommonFunc::CommonFunc() {
	/** ハンドル読込み */
	mMousePointerImgHandle = LoadGraph(FileNames::Image::MousePointer.c_str());
	mSelectFrameHandle = LoadGraph(FileNames::Image::SelectFrame.c_str());
	/* フォントハンドル作成 */
	mFontHandle = CreateFontToHandle( FontStyle, FontSize, -1, DX_FONTTYPE_ANTIALIASING);
	ASSERT( mFontHandle + 1 );
	ASSERT(	mMousePointerImgHandle + 1 );
	ASSERT(	mSelectFrameHandle + 1 );
}

CommonFunc::~CommonFunc(){
	ASSERT(DeleteFontToHandle(mFontHandle     ) + 1);
	ASSERT(DeleteGraph(mSelectFrameHandle     ) + 1);
	ASSERT(DeleteGraph(mMousePointerImgHandle ) + 1);
}

void CommonFunc::create(){
	instance = new CommonFunc();
}

void CommonFunc::destroy(){
	if ( instance != 0 ) {
		SAFE_DELETE(instance);
	}
}

CommonFunc* CommonFunc::getInstance(){
	return instance;
}

void CommonFunc::drawMousePointer() const{
	int posX, posY;
	/** マウスポインタ位置取得 */
	ASSERT( GetMousePoint( &posX, &posY ) + 1 );

	DrawGraph( posX, posY , mMousePointerImgHandle, TRUE);
}

void CommonFunc::drawSelectFrame( int posX,int posY,string drawString ) const{
	drawStringOnImg(posX, posY, mSelectFrameHandle,FontColor, mFontHandle, drawString);
}

void CommonFunc::drawStringOnImg( int posX, int posY, int ImgHandle, int fontColor, int fontHandle , std::string drawString ){
	
	int sizeX, sizeY;
	/** フレーム画像サイズ取得 */
	ASSERT( GetGraphSize( ImgHandle,&sizeX,&sizeY) + 1 );
	/** フォントサイズ幅取得 */
	int strSize = GetDrawStringWidth( drawString.c_str(), drawString.length());
	/** フォント表示位置補正取得 */
	int correctionX = ( sizeX - strSize ) / 2;
	int correctionY = ( sizeY - FontSize ) / 2;
	/** フォント表示位置取得 */
	int strPosX = posX + correctionX;
	int strPosY = posY + correctionY;

	DrawGraph( posX, posY , ImgHandle, TRUE);
	DrawStringToHandle(strPosX , strPosY ,drawString.c_str(), fontColor, fontHandle );
}

bool CommonFunc::isMousePointOnFrame( int framePosX,int framePosY ) {
	return isMousePointOnImage( framePosX, framePosY, mSelectFrameHandle );
}

bool CommonFunc::isMousePointOnImage( int imgPosX, int imgPosY, int imgHandle ) {
	int posX, posY;
	int sizeX, sizeY;
	/** マウスポインタ位置取得 */
	ASSERT( GetMousePoint( &posX, &posY ) + 1 );
	
	/** 画像サイズ取得 */
	ASSERT( GetGraphSize(imgHandle,&sizeX,&sizeY) + 1 );
	
	/** 画像座標とマウスポインタ位置の相対位置を求める */
	int relativePosX = posX - imgPosX;
	int relativePosY = posY - imgPosY;

	bool isOnImg;
	isOnImg  = ( ( relativePosX > 0 ) && ( relativePosX < sizeX ) );
	isOnImg &= ( ( relativePosY > 0 ) && ( relativePosY < sizeY ) );

	return isOnImg;
}


void CommonFunc::drawMultiSelectFrame( 
	int posX,int posY,            //最初のフレーム位置
	int intervalX,                //描画間隔(x方向)
	int intervalY,                //描画間隔(y方向)
	vector<string> modeNameArray, //描画文字
	bool isMouseDicision          //マウス判定有無
	)  {

		mSelectFrameNum = -1;

		for ( unsigned int i = 0; i < modeNameArray.size(); i++) {
			/** フレーム表示位置取得 */
			int framePosX = i * intervalX + posX;
			int framePosY = i * intervalY + posY;
			/** マウス判定ありなら表示輝度と選択フレームセット */
			if ( isMouseDicision ){
				if ( isMousePointOnImage( framePosX,framePosY,mSelectFrameHandle ) ) {
					mSelectFrameNum = static_cast<int>( i );
					SetDrawBright(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);
				} else {
					SetDrawBright(UCHAR_MAX / 2,UCHAR_MAX / 2,UCHAR_MAX / 2);
				}
			}
			drawSelectFrame( framePosX, framePosY ,modeNameArray.at(i) );
		}
		if ( isMouseDicision ) {
			/** デフォルトに戻す */
			SetDrawBright(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);
		}
}


}