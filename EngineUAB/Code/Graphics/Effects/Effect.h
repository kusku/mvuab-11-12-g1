#pragma once

#ifndef _EFFECT_H
#define _EFFECT_H

#include <string>
#include <vector>
#include <d3dx9.h>

#include "EffectDefs.h"
#include "Math\Vector3.h"
#include "Math\Vector2.h"
#include "Math\Matrix44.h"

class CXMLTreeNode;
class CLight;
class CTexture;

class CEffect
{
public:
	CEffect( CXMLTreeNode &XMLNode );
	virtual ~CEffect();

	bool		SetLights		( size_t NumOfLights );
	bool		Load			();
	bool		Load			( const std::string &Filename );
	bool		Reload			();
	bool		SetLight		(CLight* light);

	//DirectX Methods Interface
	LPD3DXEFFECT	GetD3DEffect						() const		{ return m_Effect; }
	D3DXHANDLE		GetTechniqueByName					( const std::string &TechniqueName );
	void			GetParameterBySemantic				( const std::string &SemanticName, D3DXHANDLE &l_Handle, bool Warning = true );

	//Get Methods
	const std::string&	GetEffectName					() const		{ return m_EffectName; }

	D3DXHANDLE		GetWorldMatrix						() const		{ return m_WorldMatrixParameter; }
	D3DXHANDLE		GetViewMatrix						() const		{ return m_ViewMatrixParameter; }
	D3DXHANDLE		GetProjectionMatrix					() const		{ return m_ProjectionMatrixParameter; }
	D3DXHANDLE		GetWorldViewMatrix					() const		{ return m_WorldViewMatrixParameter; }
	D3DXHANDLE		GetViewProjectionMatrix				() const		{ return m_ViewProjectionMatrixParameter; }
	D3DXHANDLE		GetWorldViewProjectionMatrix		() const		{ return m_WorldViewProjectionMatrixParameter; }
	D3DXHANDLE		GetShadowViewProjectionMatrix		() const		{ return m_ShadowViewProjectionMatrixParameter; }
	D3DXHANDLE		GetShadowWorldViewProjectionMatrix	() const		{ return m_ShadowWorldViewProjectionMatrixParameter; }
	D3DXHANDLE		GetShadowWorldViewMatrix			() const		{ return m_ShadowWorldViewMatrixParameter; }
	D3DXHANDLE		GetShadowCameraPositionMatrix		() const		{ return m_ShadowCameraPositionParameter; }
	D3DXHANDLE		GetViewInverseMatrix				() const		{ return m_ViewInverseMatrixParameter; }
	D3DXHANDLE		GetWorldInverseMatrix				() const		{ return m_WorldInverseMatrixParameter; }
	D3DXHANDLE		GetProjInverseMatrix				() const		{ return m_ProjInverseMatrixParameter; }
	D3DXHANDLE		GetViewProjectionInverseMatrix		() const		{ return m_ViewProjectionInverseMatrixParameter; }
	D3DXHANDLE		GetLightsTypeMatrix					() const		{ return m_LightsTypeParameter; }
	D3DXHANDLE		GetLightsPositionMatrix				() const		{ return m_LightsPositionParameter; }
	D3DXHANDLE		GetLightsDirectionMatrix			() const		{ return m_LightsDirectionParameter; }
	D3DXHANDLE		GetLightsAngleMatrix				() const		{ return m_LightsAngleParameter; }
	D3DXHANDLE		GetLightsColorMatrix				() const		{ return m_LightsColorParameter; }
	D3DXHANDLE		GetLightsFallOffMatrix				() const		{ return m_LightsFallOffParameter; }
	D3DXHANDLE		GetLightsStartRangeMatrix			() const		{ return m_LightsStartRangeAttenuationParameter; }
	D3DXHANDLE		GetLightsEndRangeMatrix				() const		{ return m_LightsEndRangeAttenuationParameter; }
	D3DXHANDLE		GetCameraPositionMatrix				() const		{ return m_CameraPositionParameter; }
	D3DXHANDLE		GetBonesMatrix						() const		{ return m_BonesParameter; }
	D3DXHANDLE		GetTimeMatrix						() const		{ return m_TimeParameter; }
	D3DXHANDLE		GetNumLights						() const		{ return m_NumLightsParameter; }
	D3DXHANDLE		GetHalfPixel						() const		{ return m_HalfPixelParameter; }
	D3DXHANDLE		GetRenderTargetSize					() const		{ return m_RenderTargetSizeParameter; }
	D3DXHANDLE		GetLightShadowStaticEnableParameter	() const		{ return m_LightShadowStaticEnableParameter; }
	D3DXHANDLE		GetLightShadowDynamicEnableParameter() const		{ return m_LightShadowDynamicEnableParameter; }
	D3DXHANDLE		GetStaticShadowMapSamplerParameter	(int idx) const	{ return m_StaticShadowMapSamplerParameter[idx]; }
	D3DXHANDLE		GetDynamicShadowMapSamplerParameter	(int idx) const	{ return m_DynamicShadowMapSamplerParameter[idx]; }
	D3DXHANDLE		GetParameterById					(uint16 id);
	D3DXHANDLE		GetLightIntensityParameter			() const		{ return m_LightIntensityParameter; }
	D3DXHANDLE		GetLightLinNearFarParameter			() const		{ return m_LightLinNearFarParameter; }
	D3DXHANDLE		GetShadowLightLinNearFarParameter	() const		{ return m_ShadowLightLinNearFarParameter; }

	uint32			GetActiveLights						() const		{ return m_ActiveLights; }
	const int*		GetLightType						() const		{ return m_LightsType; }
	const float*	GetLightAngle						() const		{ return m_LightsAngle; }
	const float*	GetLightFallOff						() const		{ return m_LightsFallOff; }
	const float*	GetLightStartAtt					() const		{ return m_LightsStartRangeAttenuation; }
	const float*	GetLightEndAtt						() const		{ return m_LightsEndRangeAttenuation; }
	const float*	GetLightIntensity					() const		{ return m_LightsIntensity; }
	const Vect3f*	GetLightPosition					() const		{ return m_LightsPosition; }
	const Vect3f*	GetLightDirection					() const		{ return m_LightsDirection; }
	const Vect3f*	GetLightColor						() const		{ return m_LightsColor; }
	CTexture**		GetLightsDynamicShadowMap			() const		{ return (CTexture**)m_LightsDynamicShadowMap; }
	CTexture**		GetLightsStaticShadowMap			() const		{ return (CTexture**)m_LightsStaticShadowMap; }
	const BOOL*		GetLightsDynamicShadowMapEnable		() const		{ return m_LightsDynamicShadowMapEnable; }
	const BOOL*		GetLightsStaticShadowMapEnable		() const		{ return m_LightsStaticShadowMapEnable; }
	const Mat44f*	GetLightShadowViewProjection		() const		{ return m_LightShadowViewProjection; }
	const Vect2f*	GetLightLinNearFar					() const		{ return m_LightLinNearFar; }

private:
	void		SetNullParameters			();
	bool		LoadEffect					();
	void		Unload						();

	std::string		m_EffectName;
	std::string		m_FileName;
	LPD3DXEFFECT	m_Effect;

	Vect2f			m_LightLinNearFar[MAX_LIGHTS_BY_SHADER];
	uint32			m_ActiveLights;
	float			m_LightsIntensity[MAX_LIGHTS_BY_SHADER];
	Mat44f			m_LightShadowViewProjection[MAX_LIGHTS_BY_SHADER];
	int				m_LightsType[MAX_LIGHTS_BY_SHADER];
	float			m_LightsAngle[MAX_LIGHTS_BY_SHADER];
	float			m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
	float			m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	float			m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	Vect3f			m_LightsPosition[MAX_LIGHTS_BY_SHADER];
	Vect3f			m_LightsDirection[MAX_LIGHTS_BY_SHADER];
	Vect3f			m_LightsColor[MAX_LIGHTS_BY_SHADER];
	CTexture*		m_LightsDynamicShadowMap[MAX_LIGHTS_BY_SHADER];
	CTexture*		m_LightsStaticShadowMap[MAX_LIGHTS_BY_SHADER];
	BOOL			m_LightsDynamicShadowMapEnable[MAX_LIGHTS_BY_SHADER];
	BOOL			m_LightsStaticShadowMapEnable[MAX_LIGHTS_BY_SHADER];

	D3DXHANDLE		m_ShadowLightLinNearFarParameter;
	D3DXHANDLE		m_ShadowWorldViewMatrixParameter;
	D3DXHANDLE		m_LightLinNearFarParameter;
	D3DXHANDLE		m_WorldMatrixParameter, 
					m_ViewMatrixParameter,
					m_ProjectionMatrixParameter;
	D3DXHANDLE		m_WorldViewMatrixParameter,
					m_ViewProjectionMatrixParameter,
					m_WorldViewProjectionMatrixParameter;
	D3DXHANDLE		m_ViewInverseMatrixParameter,
					m_WorldInverseMatrixParameter,
					m_ProjInverseMatrixParameter;
	D3DXHANDLE		m_ViewProjectionInverseMatrixParameter;
	D3DXHANDLE		m_ShadowViewProjectionMatrixParameter,
					m_ShadowCameraPositionParameter,
					m_ShadowWorldViewProjectionMatrixParameter;
	D3DXHANDLE		m_LightIntensityParameter;
	D3DXHANDLE		m_LightsTypeParameter, 
					m_LightsPositionParameter, 
					m_LightsDirectionParameter, 
					m_LightsAngleParameter, 
					m_LightsColorParameter;
	D3DXHANDLE		m_LightsFallOffParameter, 
					m_LightsStartRangeAttenuationParameter, 
					m_LightsEndRangeAttenuationParameter;
	D3DXHANDLE		m_NumLightsParameter;
	D3DXHANDLE		m_CameraPositionParameter;
	D3DXHANDLE		m_BonesParameter;
	D3DXHANDLE		m_TimeParameter;
	D3DXHANDLE		m_HalfPixelParameter;
	D3DXHANDLE		m_RenderTargetSizeParameter;
	D3DXHANDLE		m_LightShadowStaticEnableParameter;
	D3DXHANDLE		m_LightShadowDynamicEnableParameter;
	D3DXHANDLE		m_StaticShadowMapSamplerParameter[MAX_LIGHTS_BY_SHADER];
	D3DXHANDLE		m_DynamicShadowMapSamplerParameter[MAX_LIGHTS_BY_SHADER];

	std::vector<D3DXHANDLE> m_Parameters;
};

#endif
