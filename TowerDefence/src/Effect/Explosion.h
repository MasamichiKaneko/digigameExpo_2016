#pragma once

#include "Effect\EffectBase.h"

/** 爆破エフェクト制御 */

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
		/** 切り替えフレーム数 */
		static const int switchingFPS;
		/** 更新回数 */
		int mUpdateCallNum;
	};
}