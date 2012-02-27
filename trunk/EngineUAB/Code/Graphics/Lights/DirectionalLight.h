//----------------------------------------------------------------------------------
// CDirectionalLight class
// Author: Marc Cuscullola
//
// Description:
// Define una luz de tipo Directional
//----------------------------------------------------------------------------------
#pragma once

#ifndef _DIRECTIONALLIGHT_H
#define _DIRECTIONALLIGHT_H

class CXMLTreeNode;

#include "Light.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"

class CDirectionalLight : public CLight
{
public:
	CDirectionalLight();
	CDirectionalLight( CXMLTreeNode &XMLNode );
	virtual ~CDirectionalLight();

	virtual void		Render				( CRenderManager* RM );
	virtual void		SetShadowMap		(  );

	void				SetDirection		( const Vect3f &Direction ) { m_Direction = Direction; }
	const Vect3f&		GetDirection		() const					{ return m_Direction; }

	void			CalculateOrientationDebugRender		();

protected:
	Vect3f			m_Direction;
	Vect2i			m_OrthoShadowMapSize;
};

#endif
