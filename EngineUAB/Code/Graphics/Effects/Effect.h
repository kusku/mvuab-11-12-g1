#pragma once

#ifndef _EFFECT_H
#define _EFFECT_H

class CXMLTreeNode;

#include "EffectDefs.h"
#include "Math\Vector3.h"
#include <string>
#include <d3dx9.h>

class CEffect
{
public:
	CEffect( CXMLTreeNode *XMLNode );
	virtual ~CEffect();

	bool		SetLights		( size_t NumOfLights );
	bool		Load			();
	bool		Load			( const std::string &Filename );
	bool		Reload			();

	//DirectX Methods Interface
	LPD3DXEFFECT	GetD3DEffect			() const		{ return m_Effect; }
	D3DXHANDLE		GetTechniqueByName		( const std::string &TechniqueName );

	//Get Methods
	const std::string&	GetEffectName		() const		{ return m_EffectName; }

	D3DXHANDLE		GetWorldMatrix						() const		{ return m_WorldMatrixParameter; }
	D3DXHANDLE		GetViewMatrix						() const		{ return m_ViewMatrixParameter; }
	D3DXHANDLE		GetProjectionMatrix					() const		{ return m_ProjectionMatrixParameter; }
	D3DXHANDLE		GetWorldViewMatrix					() const		{ return m_WorldViewMatrixParameter; }
	D3DXHANDLE		GetViewProjectionMatrix				() const		{ return m_ViewProjectionMatrixParameter; }
	D3DXHANDLE		GetWorldViewProjectionMatrix		() const		{ return m_WorldViewProjectionMatrixParameter; }
	D3DXHANDLE		GetViewToLightProjectionMatrix		() const		{ return m_ViewToLightProjectionMatrixParameter; }
	D3DXHANDLE		GetViewInverseMatrix				() const		{ return m_ViewInverseMatrixParameter; }
	D3DXHANDLE		GetWorldInverseMatrix				() const		{ return m_WorldInverseMatrixParameter; }
	D3DXHANDLE		GetProjInverseMatrix				() const		{ return m_ProjInverseMatrixParameter; }
	D3DXHANDLE		GetLightEnabledMatrix				() const		{ return m_LightEnabledParameter; }
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

	const BOOL*		GetLightEnabled						() const		{ return m_LightsEnabled; }
	const int*		GetLightType						() const		{ return m_LightsType; }
	const float*	GetLightAngle						() const		{ return m_LightsAngle; }
	const float*	GetLightFallOff						() const		{ return m_LightsFallOff; }
	const float*	GetLightStartAtt					() const		{ return m_LightsStartRangeAttenuation; }
	const float*	GetLightEndAtt						() const		{ return m_LightsEndRangeAttenuation; }
	const Vect3f*	GetLightPosition					() const		{ return m_LightsPosition; }
	const Vect3f*	GetLightDirection					() const		{ return m_LightsDirection; }
	const Vect3f*	GetLightColor						() const		{ return m_LightsColor; }

private:
	void		SetNullParameters			();
	void		GetParameterBySemantic		( const std::string &SemanticName, D3DXHANDLE &l_Handle );
	bool		LoadEffect					();
	void		Unload						();

	std::string		m_EffectName;
	std::string		m_FileName;
	LPD3DXEFFECT	m_Effect;

	BOOL			m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
	int				m_LightsType[MAX_LIGHTS_BY_SHADER];
	float			m_LightsAngle[MAX_LIGHTS_BY_SHADER];
	float			m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
	float			m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	float			m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	Vect3f			m_LightsPosition[MAX_LIGHTS_BY_SHADER];
	Vect3f			m_LightsDirection[MAX_LIGHTS_BY_SHADER];
	Vect3f			m_LightsColor[MAX_LIGHTS_BY_SHADER];

	D3DXHANDLE		m_WorldMatrixParameter, 
					m_ViewMatrixParameter,
					m_ProjectionMatrixParameter;
	D3DXHANDLE		m_WorldViewMatrixParameter,
					m_ViewProjectionMatrixParameter,
					m_WorldViewProjectionMatrixParameter;
	D3DXHANDLE		m_ViewInverseMatrixParameter,
					m_WorldInverseMatrixParameter,
					m_ProjInverseMatrixParameter;
	D3DXHANDLE		m_ViewToLightProjectionMatrixParameter;
	D3DXHANDLE		m_LightEnabledParameter, 
					m_LightsTypeParameter, 
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
};

#endif
