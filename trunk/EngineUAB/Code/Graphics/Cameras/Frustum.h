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
#include "BoundingObjects.h"

class CFrustum 
{
public:

	CFrustum();

	void	Update						( const D3DXMATRIX& viewProjection );
	bool	SphereVisible				( const D3DXVECTOR3 &p, float radius ) const;
	bool	BoxVisible					( const TBoundingBox& bb, const D3DXMATRIX& WorldMatrix) const;
	bool	BoxVisible					( const D3DXVECTOR3 &max, const D3DXVECTOR3 &min ) const;
	bool	BoxVisible					(CalBoundingBox& bb);
	bool	BoxVisibleByVertexs			( const Vect3f* points) const;
	Vect3f	ComputeIntersection			(D3DXPLANE plane1, D3DXPLANE plane2, D3DXPLANE plane3);
	void	DrawFrustum					();

	Vect3f* GetCorners () { return m_Corners; }
	
	TBoundingBox GetAABB() { return m_frustumBB; }

private:

	TBoundingBox m_frustumBB;
	D3DXPLANE m_frustum[6];
	Vect3f m_Corners[8];
};

#endif //INC_FRUSTUM_H_
