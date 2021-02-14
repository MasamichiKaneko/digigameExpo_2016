#pragma once
/** ���ʏ����N���X */
#include <string>
#include <vector>

using namespace std;

namespace Sequence{

class CommonFunc{
public:
	static void create();
	static void destroy();
	static CommonFunc* getInstance();
	/** �}�E�X�|�C���^�`�� */
	void drawMousePointer() const;
	/** �t���[���`��  */
	void drawSelectFrame( int posX,int posY,std::string drawString ) const;
	/** �摜�̒��S�ɕ�����\�����ĕ`�� */
	static void drawStringOnImg( int posX, int posY, int ImgHandle, int fontColor, int fontHandle, std::string drawString );
	/** �t���[����Ƀ}�E�X�|�C���^�����݂��邩�𔻒� */
	bool isMousePointOnFrame( int framePosX,int framePosY );
	/** �摜��Ƀ}�E�X�|�C���^�����݂��邩�𔻒� */
	static bool isMousePointOnImage( int imgPosX, int imgPosY, int imgHandle );
	/** �����̃t���[���`�� */
	void drawMultiSelectFrame( 
		int posX,int posY,            //�ŏ��̃t���[���ʒu
		int intervalX,                //�`��Ԋu(x����)
		int intervalY,                //�`��Ԋu(y����)
		vector<string> modeNameArray, //�`�敶��
		bool isMouseDicision          //�}�E�X����L��
		) ;
	/** �I�𒆃t���[���擾 */
	/** �`�悵�Ă邱�ƑO�񂾂��`�悵�ĂȂ����̂�I������ꍇ������Ȃ̂ōl�����Ȃ� */
	int getSelectFrame()const { return mSelectFrameNum; }


private:
	CommonFunc();
	~CommonFunc();
	static CommonFunc* instance;
	/** �摜�n���h�� */
	int mMousePointerImgHandle;
	int mSelectFrameHandle;
	int mFontHandle;
	std::vector< int > mStageIcons;

	/** �I���t���[���i���o�[ */
	int mSelectFrameNum;
};

}