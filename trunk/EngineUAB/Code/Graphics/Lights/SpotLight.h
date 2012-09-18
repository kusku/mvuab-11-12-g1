#pragma once

#ifndef _SPOTLIGHT_H
#define _SPOTLIGHT_H

class CXMLTreeNode;

#include "Lights\Light.h"
#include "Textures\Texture.h"

class CSpotLight : public CLight
{
public:
	CSpotLight();
	CSpotLight( CXMLTreeNode &XMLNode );
	virtual ~CSpotLight();

	virtual void		Render				( CRenderManager* RM );
	virtual void		RenderShadows		( CRenderManager* RM );
	void				SetShadowMap		();

	void			SetAngle		( const float Angle )		{ m_Angle = Angle; }
	void			SetFallOff		( const float FallOff )		{ m_FallOff = FallOff; }

	float			GetAngle		() const					{ return m_Angle; }
	float			GetFallOff		() const					{ return m_FallOff; }

	void				SetDirection		( const Vect3f &Direction ) { m_Direction = Direction; }
	const Vect3f&		GetDirection		() const					{ return m_Direction; }

	void			CalculateOrientationDebugRender		();

protected:
	void		CreateShadowTextures(bool staticMap, CTexture::TFormatType formatStatic, uint32 staticWidth, uint32 staticHeight, bool dynamicMap, CTexture::TFormatType formatDynamic, uint32 dynamicWidth, uint32 dynamicHeight);

	float			m_Angle;
	float			m_FallOff;
	Vect3f			m_Direction;
};

#endif
