#pragma once

/**
* ���S�\��
*/

#include "Sequence\Child.h"
#include <string>

class FadeInFadeOut;

namespace Sequence{

	class Logo : public Child{
	public:
		Logo();
		~Logo();
		Base* update( Parent* );

	private:
		/* ���S�`��֐� */
		/* �\�����I�������true��Ԃ��\�� */
		bool draw();

		/* �ǂݍ��݉摜�̃n���h�� */
		int mImageHandle;

		FadeInFadeOut* bright;

		int mTime;
	};

}