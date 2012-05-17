#include "Plane.h"

CPlane::CPlane( ) {
}

CPlane::CPlane( const Vect3f &base, const Vect3f &v1, const Vect3f &v2 ) {
	const Vect3f sideA( v1 - base );
	const Vect3f sideB( v2 - base );

	_normal = sideA * sideB;	// Producto vectorial
	_normal.Normalize( );
	_d = -_normal * base;		// Producto escalar
}

CPlane::CPlane( const Vect3f &normal, const Vect3f &point )
	: _normal ( normal )
	, _d( 0 ) 
{
	_d = -_normal * point;
}

CPlane::CPlane( const Vect3f &normal, float d )
	: _normal( normal )
	, _d( d ) 
{
}

CPlane::~CPlane( ) {
}

void CPlane::compute( const Vect3f &normal, const Vect3f &point ) {
	_normal = normal;
	_normal.Normalize( );
	_d = -_normal * point;
}

void CPlane::set( const Vect3f &normal, float distance ) {
	_normal = normal;
	_d = distance;
}

void CPlane::normalize( ) {
	float length( _normal.Length( ) );
	if( length > 0.0f ) {
		length = 1.0f / length;

		_normal.x = _normal.x * length;
		_normal.y = _normal.y * length;
		_normal.z = _normal.z * length;
		_d = _d * length;
	}
}

const Vect3f &CPlane::getNormal( ) const {
	return _normal;
}

float CPlane::getDistance( ) const {
	return _d;
}

float CPlane::distanceToPoint( const Vect3f &point ) const {
	return _normal * point + _d;
}

Vect3f CPlane::projectPoint( const Vect3f &point ) const {
	Vect3f out;

	float distance = distanceToPoint( point );
	out = point - _normal * distance;
	return out;
}

bool CPlane::isPointInside( const Vect3f &point ) const {
	const float v = _normal * point + _d ;

	return v >= 0.0f;
}