#pragma once

#ifndef _EFFECTTECHNIQUE_H
#define _EFFECTTECHNIQUE_H

class CEffect;

#include "Utils\Types.h"
#include <string>
#include <d3dx9.h>

class CEffectTechnique
{
public:
	CEffectTechnique();
	virtual ~CEffectTechnique();

	bool	BeginRender();
	bool	Refresh();

	inline CEffect*		GetEffect() const		{ return m_Effect; }
	
	//DirectX Methods Interface
	D3DXHANDLE		GetD3DTechnique() const		{ return m_D3DTechnique; }

	//Set & Get Methods
	void		SetUseCameraPosition				( bool cameraPosition )			{ m_UseCameraPosition = cameraPosition; }
	void		SetUseInversProjMatrix				( bool useMatrix )				{ m_UseInverseProjMatrix = useMatrix; }
	void		SetUseInversViewMatrix				( bool useMatrix )				{ m_UseInverseViewMatrix = useMatrix; }
	void		SetUseInversWorldMatrix				( bool useMatrix )				{ m_UseInverseWorldMatrix = useMatrix; }
	void		SetUseProjMatrix					( bool useMatrix )				{ m_UseProjMatrix = useMatrix; }
	void		SetUseViewMatrix					( bool useMatrix )				{ m_UseViewMatrix = useMatrix; }
	void		SetUseWorldMatrix					( bool useMatrix )				{ m_UseWorldMatrix = useMatrix; }
	void		SetUseWorldViewMatrix				( bool useMatrix )				{ m_UseWorldViewMatrix = useMatrix; }
	void		SetUseWorldViewProjectionMatrix		( bool useMatrix )				{ m_UseWorldViewProjectionMatrix = useMatrix; }
	void		SetUseViewProjectionMatrix			( bool useMatrix )				{ m_UseViewProjectionMatrix = useMatrix; }
	void		SetUseViewToLightProjectionMatrix	( bool useMatrix )				{ m_UseViewToLightProjectionMatrix = useMatrix; }
	void		SetUseLights						( bool useLights )				{ m_UseLights = useLights; }
	void		SetUseLightAmbientColor				( bool useLight )				{ m_UseLightAmbientColor = useLight; }
	void		SetNumOfLights						( uint32 numOfLights )			{ m_NumOfLights = numOfLights; }
	void		SetTechniqueName					( const std::string &Name )		{ m_TechniqueName = Name; }
	void		SetEffect							( CEffect *effect )				{ m_Effect = effect; }
	void		SetD3DTechnique						( D3DXHANDLE d3dTechnique )		{ m_D3DTechnique = d3dTechnique; }

private:
	bool	m_UseCameraPosition;
	bool	m_UseInverseProjMatrix;
	bool	m_UseInverseViewMatrix;
	bool	m_UseInverseWorldMatrix;
	bool	m_UseLights;
	uint32	m_NumOfLights;
	bool	m_UseLightAmbientColor;
	bool	m_UseProjMatrix;
	bool	m_UseViewMatrix;
	bool	m_UseWorldMatrix;
	bool	m_UseWorldViewMatrix;
	bool	m_UseWorldViewProjectionMatrix;
	bool	m_UseViewProjectionMatrix;
	bool	m_UseViewToLightProjectionMatrix;
	bool	m_UseTime;

	CEffect *	m_Effect;
	D3DXHANDLE	m_D3DTechnique;
	std::string m_TechniqueName;
};

#endif