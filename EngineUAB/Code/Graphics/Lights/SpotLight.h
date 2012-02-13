#pragma once

#ifndef _SPOTLIGHT_H
#define _SPOTLIGHT_H

class CXMLTreeNode;

#include "DirectionalLight.h"

class CSpotLight : public CDirectionalLight
{
public:
	CSpotLight();
	CSpotLight( CXMLTreeNode &XMLNode );
	virtual ~CSpotLight();

	virtual void		Render				( CRenderManager* RM );
	void				SetShadowMap		();

	void			SetAngle		( const float Angle )		{ m_Angle = Angle; }
	void			SetFallOff		( const float FallOff )		{ m_FallOff = FallOff; }

	float			GetAngle		() const					{ return m_Angle; }
	float			GetFallOff		() const					{ return m_FallOff; }

protected:
	float			m_Angle;
	float			m_FallOff;
};

#endif
