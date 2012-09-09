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

#include "cal3d\cal3d.h"
#include <d3dx9math.h>
#include <d3d9.h>
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math\Matrix44.h"
#include "Math\Plane.h"

class CFrustum 
{
public:

	void	Update						( const D3DXMATRIX &viewproj );
	bool	SphereVisible				( const D3DXVECTOR3 &p, float radius ) const;
	bool	BoxVisible					( const D3DXVECTOR3 &max, const D3DXVECTOR3 &min ) const;
	bool	BoxVisible					(CalBoundingBox& bb);
	bool	BoxVisibleByVertexs			( const Vect3f* points) const;
	Vect3f	ComputeIntersection			(D3DXPLANE plane1, D3DXPLANE plane2, D3DXPLANE plane3);

private:

	float m_proj[16];
	float m_modl[16];
	float m_clip[16];
	float m_frustum[6][4];
};

#endif //INC_FRUSTUM_H_
