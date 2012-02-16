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
#include "Math\Matrix44.h"

class CFrustum 
{
public:
	
  void	Update						( const Mat44f &viewproj );
  bool	SphereVisible				( const Vect3f &p, float radius ) const;
  bool	BoxVisible					( const Vect3f &max, const Vect3f &min ) const;
  bool  BoxVisibleByVertexs			( const Vect3f* points) const;
   
private:

  float m_proj[16];
  float m_modl[16];
  float m_clip[16];
  float m_frustum[6][4];
};

#endif //INC_FRUSTUM_H_
