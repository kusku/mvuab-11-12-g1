//----------------------------------------------------------------------------------
// CCamera class
// Author: Enric Vergara
//
// Description:
// Clase 
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_FRUSTUM_H_
#define INC_FRUSTUM_H_

#include <d3dx9math.h>
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math\Matrix44.h"
#include "Math\Plane.h"

class CFrustum 
{
public:
	CFrustum();


	void	Update						( const Mat44f &viewproj );
	bool	SphereVisible				( const Vect3f &p, float radius ) const;
	bool	BoxVisible					( const Vect3f &max, const Vect3f &min ) const;
	bool  BoxVisibleByVertexs			( const Vect3f* points) const;

	const Vect3f*	GetCorners			() const	{ return m_Corners; }

private:
	
	float m_proj[16];
	float m_modl[16];
	float m_clip[16];
	Vect4f m_frustum[6];
	Vect3f m_Corners[8];

	Vect3f ComputeIntersection(Vect4f plane1, Vect4f plane2, Vect4f plane3);
};

#endif //INC_FRUSTUM_H_
