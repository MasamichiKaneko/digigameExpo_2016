#pragma once
/**
* ���W�N���X
*/

namespace Game{
	class Point{
	public:
		Point();
		Point(int aX, int aY);
		~Point();

		/* ���W�Ԃ̋�������o�� */
		static double getDistance( Point p1, Point p2 );
		double getDistance( Point p )const;

		int getX() const;
		int getY() const;

		void setX( int aX );
		void setY( int aY );

		void setPoint( Point p );

		/* ���W�ړ� */
		void moveX( int aX );
		void moveY( int aY );
		void movePoint( Point p );
	private:
		int mX,mY;
	};
}