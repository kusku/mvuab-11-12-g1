//----------------------------------------------------------------------------------
// CLight class
// Author: Marc Cuscullola
//
// Description:
// Define una interfície para dar propiedades de las luces
//----------------------------------------------------------------------------------
#pragma once

#ifndef _LIGHT_H
#define _LIGHT_H

class CRenderManager;

#include "Object3D.h"
#include "Utils\Named.h"
#include "Math\Color.h"
#include <string>

class CLight : public CObject3D, CNamed
{
public:
	enum TLightType
	{
		OMNI=0,
		DIRECTIONAL,
		SPOT
	};

public:
	CLight();
	virtual ~CLight();

	virtual void	Render				( CRenderManager *RM );
	bool			RenderShadows		() const;

	void				SetColor						( const CColor &color )					{ m_Color = color; }
	void				SetStartRangeAttenuation		( const float StartRangeAttenuation )	{ m_StartRangeAttenuation = StartRangeAttenuation; }
	void				SetEndRangeAttenuation			( const float EndRangeAttenuation )		{ m_EndRangeAttenuation = EndRangeAttenuation; }
	void				SetType							( const TLightType Type )				{ m_Type = Type; }

	const CColor&		GetColor					() const		{ return m_Color; }
	float				GetStartRangeAttenuation	() const		{ return m_StartRangeAttenuation; }
	float				GetEndRangeAttenuation		() const		{ return m_EndRangeAttenuation; }
	TLightType			GetType						() const		{ return m_Type; }

protected:
	CColor				m_Color;
	TLightType			m_Type;	
	bool				m_RenderShadows;
	float				m_StartRangeAttenuation;
	float				m_EndRangeAttenuation;

	static TLightType		GetLightTypeByName		( const std::string &StrLightType );
};

#endif
