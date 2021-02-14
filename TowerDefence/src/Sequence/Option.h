#pragma once

/**
 * �I�v�V�������
 */

#include <map>
#include <vector>
#include <string>
#include "Sequence\Child.h"

namespace Sequence{

	class CommonFunc;
	class Parent;

	class Option : public Child{
	public:
		Option();
		~Option();
		Base* update( Parent* );

	private:

		CommonFunc* commonFunc;

		typedef struct{
			int x, y;
		}pos;

		/** ���̓X�e�[�^�X */
		enum InputStatus{
			DECIDE,  //����
			CANCEL,  //�L�����Z��
			NONE,    //����
		};
		
		/** �`��֌W */
		void draw();
		void drawMeter( std::string aValueType );
		void drawPlusMinusIcon( std::string aValueType );
		void drawGridSample();

		/** �`��ʒu�v�Z */
		void culcDrawPos();
		
		/** ���͎�t */
		void mInputPrompt( Base** nextMode, Parent** parent ); 
		
		/** �ݒ�����݂̐ݒ�l�Z�b�g */
		void mSetConfigValue( std::string aValueType, Parent** parent );


		/** �摜�n���h�� */
		int mBackGroundHandle;
		int mMeterHandle;
		int mDisplayBarHandle;
		int mPlusIconHandle;
		int mMinusIconHandle;
		int mMapChipSampleHandle;

		/** �t�H���g�n���h�� */
		int mFontHandle;

		/** �ݒ�l */
		std::map< std::string, int > mSetValues;
		std::map< std::string, int > mMaxValues;

		/** �ݒ�l�e�[�u�� */
		std::map< std::string, int > mValueTable;
		enum ValueTable{
			BGM,
			SE,
			Grid,
		};

		/** �ݒ�l���Ƃ̕`��ʒu��` */
		std::map< std::string, pos > mValueTypePos;
		std::map< std::string, pos > mValueRatioPos;
		std::map< std::string, pos > mMeterStartPos;
		std::map< std::string, pos > mMinusIconPos;
		std::map< std::string, pos > mPlusIconPos;

		/** �O���b�h�Z�x�ێ��p */
		int mGridDipth;

		/** �I�����[�h */
		std::vector< std::string > mModeName;

		/** �e��萔 */
		static const int DisplayBarDrawPosX;
		static const int DisplayBarDrawPosY;
		static const int DisplayBarIntervalX;
		static const int DisplayBarIntervalY;

		static const int SelectModePosX;
		static const int SelectModePosY;
		static const int SelectModeIntervalX;
		static const int SelectModeIntervalY;

		static const int PlusMinusIconSizeX;
		static const int PlusMinusIconSizeY;
		static const int PlusMinusIconNum;

		static const int GridDrawPosX;
		static const int GridDrawPosY;
		static const int DrawMapChipNum;
		static const int GridLineColor;

		static const int FontSize;
		static const int FontColor;

	};
}