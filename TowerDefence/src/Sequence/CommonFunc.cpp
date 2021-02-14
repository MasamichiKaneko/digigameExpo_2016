#include <string>
#include "Sequence\CommonFunc.h"
#include "FileNames.h"
#include "DxLib.h"
#include "define.h"


using namespace std;

namespace Sequence{

/** ���ʏ����N���X */
CommonFunc* CommonFunc::instance = 0;

/* �\�������T�C�Y */
const int FontSize = 18;
const int FontColor = GetColor(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);
const char* FontStyle = "HGS�s����";

CommonFunc::CommonFunc() {
	/** �n���h���Ǎ��� */
	mMousePointerImgHandle = LoadGraph(FileNames::Image::MousePointer.c_str());
	mSelectFrameHandle = LoadGraph(FileNames::Image::SelectFrame.c_str());
	/* �t�H���g�n���h���쐬 */
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
	/** �}�E�X�|�C���^�ʒu�擾 */
	ASSERT( GetMousePoint( &posX, &posY ) + 1 );

	DrawGraph( posX, posY , mMousePointerImgHandle, TRUE);
}

void CommonFunc::drawSelectFrame( int posX,int posY,string drawString ) const{
	drawStringOnImg(posX, posY, mSelectFrameHandle,FontColor, mFontHandle, drawString);
}

void CommonFunc::drawStringOnImg( int posX, int posY, int ImgHandle, int fontColor, int fontHandle , std::string drawString ){
	
	int sizeX, sizeY;
	/** �t���[���摜�T�C�Y�擾 */
	ASSERT( GetGraphSize( ImgHandle,&sizeX,&sizeY) + 1 );
	/** �t�H���g�T�C�Y���擾 */
	int strSize = GetDrawStringWidth( drawString.c_str(), drawString.length());
	/** �t�H���g�\���ʒu�␳�擾 */
	int correctionX = ( sizeX - strSize ) / 2;
	int correctionY = ( sizeY - FontSize ) / 2;
	/** �t�H���g�\���ʒu�擾 */
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
	/** �}�E�X�|�C���^�ʒu�擾 */
	ASSERT( GetMousePoint( &posX, &posY ) + 1 );
	
	/** �摜�T�C�Y�擾 */
	ASSERT( GetGraphSize(imgHandle,&sizeX,&sizeY) + 1 );
	
	/** �摜���W�ƃ}�E�X�|�C���^�ʒu�̑��Έʒu�����߂� */
	int relativePosX = posX - imgPosX;
	int relativePosY = posY - imgPosY;

	bool isOnImg;
	isOnImg  = ( ( relativePosX > 0 ) && ( relativePosX < sizeX ) );
	isOnImg &= ( ( relativePosY > 0 ) && ( relativePosY < sizeY ) );

	return isOnImg;
}


void CommonFunc::drawMultiSelectFrame( 
	int posX,int posY,            //�ŏ��̃t���[���ʒu
	int intervalX,                //�`��Ԋu(x����)
	int intervalY,                //�`��Ԋu(y����)
	vector<string> modeNameArray, //�`�敶��
	bool isMouseDicision          //�}�E�X����L��
	)  {

		mSelectFrameNum = -1;

		for ( unsigned int i = 0; i < modeNameArray.size(); i++) {
			/** �t���[���\���ʒu�擾 */
			int framePosX = i * intervalX + posX;
			int framePosY = i * intervalY + posY;
			/** �}�E�X���肠��Ȃ�\���P�x�ƑI���t���[���Z�b�g */
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
			/** �f�t�H���g�ɖ߂� */
			SetDrawBright(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);
		}
}


}