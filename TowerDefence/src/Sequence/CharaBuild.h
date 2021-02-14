#pragma once

#include "Sequence\Child.h"
#include <vector>
#include <string>
#include <map>

/**
 * �琬���
 */

namespace Game{
	class CharaStatusReader;
}

namespace Sequence{

	class CommonFunc;
	class Parent;
	class CharaBuild : public Child{
	public:
		CharaBuild( /* const char* mCharaDataFileName*/ );
		~CharaBuild();
		Base* update( Parent* );
	private:

		typedef struct{
			int x, y;
		}pos;

		/** �`��ʒu�v�Z */
		void culcDrawPos();

		/** �e��`�� */
		void draw() const;
		void mDrawCharaIcon() const;
		void mDrawCharaStatus() const;

		/** ���̓X�e�[�^�X */
		enum InputStatus{
			DECIDE,  //����
			CANCEL,  //�L�����Z��
			NONE,    //����
		};

		/** ���͎�t */
		void mInputPrompt( Base** nextMode, Parent** parent );

		/** �f�[�^�ǂݏ��� */
		void mReadData( const char* mCharaDataFileName );
		void mWriteData( const char* mCharaDataFileName );

		/** �X�e�[�^�X�\���l�擾 */
		std::string getDisplayStatusValue( std::string typeName, int value ) const;

		/** �X�e�[�^�X */
		Game::CharaStatusReader* mReadStatus;

		/** �ύX�l�ێ��p */
		/**      ���L����id  ���e�X�e�[�^�X�ύX�l */
		std::map<int,       std::map<std::string, int> > mAssignValues;

		/** �摜�n���h�� */
		int mBackGroundHandle;
		int mCursorHandle;
		int mStatusBackHandle;
		int mStatusBarHandle;
		int mMeterHandle;
		int mPlusIconHandle;
		int mMinusIconHandle;
		int mWritingBackHandle;

		std::vector< int > mPlusMinusIconHandles;
		std::vector< int > mStandImgHandles;
		std::vector< int > mCharaIconHandles;

		/** �t�H���g�n���h�� */
		int mFontHandle;

		CommonFunc* commonFunc;

		std::vector< std::string > mModeName;

		/** �`��ʒu��` */
		std::vector< pos > mCharaIconPos;
		std::vector<pos> mStatusBarPos;
		pos mPlusIconPos;
		pos mMinusIconPos;
		pos mMeterDrawPos;
		
		/** �X�e�[�^�X�\���� */
		std::map< std::string, std::string > mDisplayName;

		/** �������݃t���O */
		bool mWriteFlag;

		/** �ύX�O�ύX�ヌ�x�� */
		int beforeLevel;
		int afterLevel;
		int maxLevel;

		/** �萔��` */
		/** �o�[�\���� */
		static const int DrawBarNum;
		/** �t�H���g�֘A */
		static const int FontSize;
		static const int FontColor;
		static const char* FontStyle;

		/** �L�����A�C�R���`��J�n�ʒu */
		static const int CharaIconDrawX;
		static const int CharaIconDrawY;
		/** �L�����A�C�R���`��Ԋu */
		static const int CharaIconIntervalX;
		static const int CharaIconIntervalY;

		/** �L���������G�`��ʒu */
		static const int CharaStandDrawX;
		static const int CharaStandDrawY;

		/** �I���t���[���`��ʒu */
		static const int SelectPosX;
		static const int SelectPosY;

		/** �X�e�[�^�X�\���t���[���`��ʒu */
		static const int CharaStatusBackPosX;
		static const int CharaStatusBackPosY;

		/** �I���t���[���`��Ԋu */
		static const int SelectIntervalX;
		static const int SelectIntervalY;

		/** �X�e�[�^�X�o�[�`��J�n�ʒu */
		static const int StatusBarPosX;
		static const int StatusBarPosY;

		/** �X�e�[�^�X�`��Ԋu */
		static const int StatusBarIntervalX;
		static const int StatusBarIntervalY;

		/** �v���}�C�A�C�R�� */
		static const int PlusMinusIconSizeX;
		static const int PlusMinusIconSizeY;
		static const int PlusMinusIconNum;

		/** ���x���\���ʒu */
		static const int LevelPosX;
		static const int LevelPosY;

		/** sp�\���ʒu */
		static const int SpValuePosX;
		static const int SpValuePosY;

		/** �X�e�[�^�X�\���ʒu */
		static const int StatusDrawPosX;
		static const int StatusDrawIntervalX;

		/** ���̃��x���܂ł�exp�\�� */
		static const int ExpValueForNextLvPosX;
		static const int ExpValueForNextLvPosY;
		
	};
}