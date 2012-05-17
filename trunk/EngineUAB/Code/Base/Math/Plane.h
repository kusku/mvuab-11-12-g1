#pragma once

#ifndef INC_PLANE_POINT_H_
#define INC_PLANE_POINT_H_

#include "Vector3.h"

class CPlane {
public:
	CPlane( );
	CPlane( const Vect3f &base, const Vect3f &v1, const Vect3f &v2 );
	CPlane( const Vect3f &normal, const Vect3f &point );
	CPlane( const Vect3f &normal, float d );
	~CPlane( );

	void compute( const Vect3f &normal, const Vect3f &point );
	void set( const Vect3f &normal, float distance );

	void normalize( );

	const Vect3f &getNormal( ) const;
	float getDistance( ) const;
	float distanceToPoint( const Vect3f &point ) const;
	Vect3f projectPoint( const Vect3f &point ) const;

	bool isPointInside( const Vect3f &point ) const;

private:
	Vect3f _normal;
	float _d;

};

#endif