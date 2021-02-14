#pragma once
/**
* 座標クラス
*/

namespace Game{
	class Point{
	public:
		Point();
		Point(int aX, int aY);
		~Point();

		/* 座標間の距離割り出し */
		static double getDistance( Point p1, Point p2 );
		double getDistance( Point p )const;

		int getX() const;
		int getY() const;

		void setX( int aX );
		void setY( int aY );

		void setPoint( Point p );

		/* 座標移動 */
		void moveX( int aX );
		void moveY( int aY );
		void movePoint( Point p );
	private:
		int mX,mY;
	};
}