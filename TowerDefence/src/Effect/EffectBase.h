#pragma once
#include "Game\Point.h"

namespace Effect{
	class EffectBase {
	public:
		EffectBase( 
			Game::Point aPoint,
			int aId,
			int aSize )
			: mPoint( aPoint ),
			mId( aId ),
			mSize( aSize ),
			mPanel( 0 ) , 
			mIsDraw(true){}

		~EffectBase(){};
		virtual void update() = 0;
		Game::Point getPoint()const{ return mPoint; }
		void setPoint( int x, int y ){ mPoint.setX( x ); mPoint.setY( y ); }
		void setPoint( Game::Point p ){ setPoint( p.getX(), p.getY() ); }
		int getSize()const{ return mSize; }
		int getId()const{ return mId; }
		int getPanel()const{ return mPanel; }
		bool getIsDraw() const{ return mIsDraw; }
	protected:
		bool mIsDraw;
		Game::Point mPoint;
		int mSize;
		int mId;
		int mPanel;
	};
}