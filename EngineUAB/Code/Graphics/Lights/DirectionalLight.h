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

#include "Light.h"
#include "Math\Vector3.h"

class CDirectionalLight : public CLight
{
public:
	CDirectionalLight();
	virtual ~CDirectionalLight();

	virtual void		Render				( CRenderManager* RM );

	void				SetDirection		( const Vect3f &Direction ) { m_Direction = Direction; }
	const Vect3f&		GetDirection		() const					{ return m_Direction; }

protected:
	Vect3f			m_Direction;
};

#endif
