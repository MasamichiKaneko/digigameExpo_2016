#include "Game\Point.h"
#include <cmath>

namespace Game{
	Point::Point() : mX( 0 ),mY( 0 ){}
	Point::Point( int aX, int aY ) : mX( aX ), mY( aY ){}
	Point::~Point(){}

	double Point::getDistance( Point p1, Point p2 ){
		return p1.getDistance( p2 );
	}

	double Point::getDistance( Point p ) const {
		double dX = static_cast< double >( mX - p.getX() );
		double dY = static_cast< double >( mY - p.getY() );

		dX *= dX;
		dY *= dY;

		return sqrt( ( dX + dY ) );
	}

	int Point::getX() const{
		return mX;
	}

	int Point::getY() const{
		return mY;
	}

	void Point::setX( int aX ) {
		mX = aX;
	}

	void Point::setY( int aY ) {
		mY = aY;
	}

	void Point::setPoint( Point p ) {
		mX = p.getX();
		mY = p.getY();
	}

	void Point::moveX( int aX ) {
		mX += aX;
	}

	void Point::moveY( int aY ) {
		mY += aY;
	}

	void Point::movePoint( Point p ) {
		mX += p.getX();
		mY += p.getY();
	}

}