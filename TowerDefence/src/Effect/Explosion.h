#pragma once

#include "Effect\EffectBase.h"

/** ���j�G�t�F�N�g���� */

namespace Effect{

	class Explosion : public EffectBase{
	public:
		Explosion(
			Game::Point aPoint,
			int aId,
			int aSize );
		~Explosion(){}
		void update();
		int getPanel()const{ return ( mUpdateCallNum / switchingFPS ); }
	private:
		/** �؂�ւ��t���[���� */
		static const int switchingFPS;
		/** �X�V�� */
		int mUpdateCallNum;
	};
}