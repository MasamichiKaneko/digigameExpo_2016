#pragma once

#include "Sequence\Child.h"
#include <vector>
#include <string>

class FadeInFadeOut;


/**
 * �^�C�g��
 */
namespace Sequence{
	class Parent;
	class CommonFunc;
	class Title : public Child{
	public:
		/**
		* [����]
		* true :���oon
		* false:���ooff
		*/
		Title(bool Production);
		~Title();
		Base* update( Parent* );
	private:

		enum NextMode{
			SELECT,
			OPTION,
			END,
			NONE,
		};
		
		/* ���[�h�\���� */
		std::vector<std::string> mModeName;

		/* �^�C�g����ʕ`�� */
		void draw() const;

		/* �I���t���[���`�� */
		void drawSelectFrame() const;

		/* ���͎�t���� */
		void mInputPrompt(Base** nextMode);

		/* ���ǂ̃��[�h��I�����Ă��邩���}�E�X�|�C���^�̈ʒu����擾 */
		/** �v�������狤�ʉ����邩�� */
		NextMode mGetSelectWhichMode() const;

		/** �E�B���h�E�T�C�Y */
		int windowSizeX;
		int windowSizeY;

		/* �摜�n���h�� */
		int mBackGroundHandle;
		int mTitleLogoHandle;
		int mStandImageHandle;
		
		int mTime;
		static int mSelection;
		FadeInFadeOut* bright;
		CommonFunc* commonFunc;

		/** �萔��` */
		/** �`��ʒu */
		static const int StandImgPosX;
		static const int StandImgPosY;

		static const int LogoImgPosX;
		static const int LogoImgPosY;

		static const int SelectPosX;
		static const int SelectPosY;

		/* ���j���[�`��Ԋu��` */
		static const int drawInterval;


		
	};

} //namespace Sequence
