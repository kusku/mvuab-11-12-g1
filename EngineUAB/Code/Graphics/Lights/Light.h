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
class CTexture;
class CRenderableObjectsManager;
class CEffect;

#include "Object3D.h"
#include "Utils\Named.h"
#include "Math\Color.h"
#include "Math\Matrix44.h"
#include <string>
#include <vector>

class CLight : public CObject3D, public CNamed
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

	virtual void	Render				( CRenderManager *RM ) = 0;
	bool			RenderShadows		() const		{ return m_RenderShadows; }

	void			BeginRenderEffectManagerShadowMap	( CEffect *Effect );
	void			GenerateShadowMap					( CRenderManager *RM );

	virtual void		SetShadowMap					( CRenderManager *RM ) = 0;
	void				SetColor						( const CColor &color )					{ m_Color = color; }
	void				SetStartRangeAttenuation		( const float StartRangeAttenuation )	{ m_StartRangeAttenuation = StartRangeAttenuation; }
	void				SetEndRangeAttenuation			( const float EndRangeAttenuation )		{ m_EndRangeAttenuation = EndRangeAttenuation; }
	void				SetRenderShadows				( const bool shadows )					{ m_RenderShadows = shadows; }
	void				SetType							( const TLightType Type )				{ m_Type = Type; }
	void				SetGenerateDynamicShadowMap		( bool GenerateDynamicShadowMap )		{ m_GenerateDynamicShadowMap = GenerateDynamicShadowMap; }
	void				SetGenerateStaticShadowMap		( bool GenerateStaticShadowMap )		{ m_GenerateStaticShadowMap = GenerateStaticShadowMap; }
	void				SetMustUpdateStaticShadowMap	( bool MustUpdateStaticShadowMap )		{ m_MustUpdateStaticShadowMap = MustUpdateStaticShadowMap; }

	const CColor &		GetColor						() const		{ return m_Color; }
	float				GetStartRangeAttenuation		() const		{ return m_StartRangeAttenuation; }
	float				GetEndRangeAttenuation			() const		{ return m_EndRangeAttenuation; }
	bool				GetRenderShadows				() const		{ return m_RenderShadows; }
	TLightType			GetType							() const		{ return m_Type; }
	bool				GetGenerateDynamicShadowMap		() const		{ return m_GenerateDynamicShadowMap; }
	bool				GetGenerateStaticShadowMap		() const		{ return m_GenerateStaticShadowMap; }
	bool				GetMustUpdateStaticShadowMap	() const		{ return m_MustUpdateStaticShadowMap; }
	CTexture*			GetStaticShadowMap				() const		{ return m_pStaticShadowMap; }
	CTexture*			GetDynamicShadowMap				() const		{ return m_pDynamicShadowMap; }
	CTexture*			GetShadowMaskTexture			() const		{ return m_pShadowMaskTexture; }
	const Mat44f &		GetViewShadowMap				() const		{ return m_ViewShadowMap; }
	const Mat44f &		GetProjectionShadowMap			() const		{ return m_ProjectionShadowMap; }

	std::vector<CRenderableObjectsManager*> &	GetStaticShadowMapRenderableObjectsManagers()	{ return m_StaticShadowMapRenderableObjectsManagers; }
	std::vector<CRenderableObjectsManager*> &	GetDynamicShadowMapRenderableObjectsManagers()	{ return m_DynamicShadowMapRenderableObjectsManagers; }

protected:
	static TLightType		GetLightTypeByName		( const std::string &StrLightType );

	CColor				m_Color;
	TLightType			m_Type;	
	bool				m_RenderShadows;
	float				m_StartRangeAttenuation;
	float				m_EndRangeAttenuation;

	bool				m_GenerateDynamicShadowMap;
	bool				m_GenerateStaticShadowMap;
	bool				m_MustUpdateStaticShadowMap;

	CTexture			*m_pStaticShadowMap;
	CTexture			*m_pDynamicShadowMap;
	CTexture			*m_pShadowMaskTexture;

	std::vector<CRenderableObjectsManager*>		m_StaticShadowMapRenderableObjectsManagers;
	std::vector<CRenderableObjectsManager*>		m_DynamicShadowMapRenderableObjectsManagers;

	Mat44f				m_ViewShadowMap;
	Mat44f				m_ProjectionShadowMap;
};

#endif
