#pragma once
#include <string>
#include <vector>
#include "Game\Point.h"
#include "Effect\EffectBase.h"

namespace Charactor{

	class CharactorBase{
	public:
		CharactorBase(){}
		~CharactorBase(){}

		/** キャラタイプgetter */
		virtual std::string getCharactorType() = 0;
		/** 描画するゲージgetter */
		virtual int getGauge()const = 0;
		virtual int getGaugeMax()const = 0;

		Game::Point getPoint() const{ return mPoint; };
		int getId() const{ return mId; }
		int getMotionId() const{ return mMotionId; }
		int getRange() const{ return mRange; }
		virtual std::vector< Effect::EffectBase* > getEffect()const = 0;

	protected:
		
		/** 座標 */
		Game::Point mPoint;
		/** id */
		int mId;
		/** モーションid */
		int mMotionId;
		/** 判定範囲 */
		int mRange;
	};

}