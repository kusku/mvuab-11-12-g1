#pragma once

#ifndef _EFFECTTECHNIQUE_H
#define _EFFECTTECHNIQUE_H

class CEffect;
class CXMLTreeNode;

#include "Utils\Types.h"
#include <string>
#include <vector>
#include <d3dx9.h>

class CEffectTechnique
{
public:
	CEffectTechnique( CXMLTreeNode &XMLNode );
	virtual ~CEffectTechnique();

	bool	BeginRender();
	bool	Refresh();

	inline CEffect*		GetEffect() const		{ return m_Effect; }
	const std::string&		GetTechniqueName		() const		{ return m_TechniqueName; }
	
	//DirectX Methods Interface
	D3DXHANDLE		GetD3DTechnique() const		{ return m_D3DTechnique; }

	void		SetValueFromParam		( uint16 id, float value );
	float		GetValueFromParam		( uint16 id ) const				{ return m_Params[id]; }

private:
	bool	m_UseCameraPosition;
	bool	m_UseInverseProjMatrix;
	bool	m_UseInverseViewMatrix;
	bool	m_UseInverseViewProjMatrix;
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
	bool	m_UseShadowViewProjectionMatrix;
	bool	m_UseShadowCameraPosition;
	bool	m_UseShadowWorldViewProjMatrix;
	bool	m_UseShadowMaps;
	bool	m_UseTime;
	bool	m_UseHalfPixel;
	bool	m_UseRenderTargetSize;
	bool	m_UseParams;
	bool	m_FowardShading;

	uint16	m_NumOfParams;
	std::vector<float>	m_Params;

	CEffect *	m_Effect;
	D3DXHANDLE	m_D3DTechnique;
	std::string m_TechniqueName;
};

#endif