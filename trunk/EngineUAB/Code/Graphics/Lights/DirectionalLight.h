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
#include "Math\Matrix44.h"
#include "Textures\Texture.h"

#include <d3dx9.h>
#include <d3d9.h>

class CFrustum;

class CDirectionalLight : public CLight
{
public:
	CDirectionalLight();
	CDirectionalLight( CXMLTreeNode &XMLNode );
	virtual ~CDirectionalLight();

	virtual void		Render						( CRenderManager* RM );
	virtual void		RenderShadows				(CRenderManager *RM);
	virtual void		SetShadowMap				(  );
	void				CalculateCascadeFrustums	();
	void				CalculateCascadeFrustumsVP	(float farClip, float frustumNearClip, float frustumFarClip, D3DXMATRIX& view, D3DXMATRIX& proj, Vect3f& shPos);

	void				SetDirection		( const Vect3f &Direction ) { m_Direction = Direction; }
	const Vect3f&		GetDirection		() const					{ return m_Direction; }

	Vect2f			GetShadowMapPixelSize () const { return m_ShadowMapPixelSize; }
	Vect3f			GetCascadeDistances () const { return m_CascadeDistances; }
	const Mat44f*	GetCascadeViewProjShadowMap() const { return m_CascadeViewProjShadowMap; }

	void			CalculateOrientationDebugRender		();

protected:
	void		CreateShadowTextures(bool staticMap, CTexture::TFormatType formatStatic, uint32 staticWidth, uint32 staticHeight, bool dynamicMap, CTexture::TFormatType formatDynamic, uint32 dynamicWidth, uint32 dynamicHeight);

	Vect3f			m_Direction;
	Vect3f			m_DirectionNormalize;
	Vect2i			m_OrthoShadowMapSize;

	Vect2i			m_DynamicShadowMapSize;
	Vect2f			m_ShadowMapPixelSize;
	Vect2i			m_StaticShadowMapSize;

	Vect3f			m_CascadeDistances;
	uint32			m_NumCascades;
	CFrustum**		m_ShadowFrustums;
	Mat44f*			m_CascadeViewProjShadowMap;
	Mat44f*			m_CascadeViewShadowMap;
	Mat44f*			m_CascadeProjectionShadowMap;
	Vect3f*			m_CascadeShadowPositions;
	float*			m_SplitDepthsTmp;
	Vect3f			m_FrustumCornersVS[8];
	Vect3f			m_SplitFrustumCornersVS[8];
};

#endif
