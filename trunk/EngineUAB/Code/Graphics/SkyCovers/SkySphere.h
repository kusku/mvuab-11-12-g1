
#pragma once

#ifndef _SKY_SPHERE_H
#define _SKY_SPHERE_H

#include "RenderableObjects\Primitive Meshes\SphereMesh.h"
#include "Math\Matrix44.h"

class CEffectTechnique;
class CXMLTreeNode;
class CTextureCube;
class CTexture;

class CSkySphere
{
	private:
		const float			m_CloudAnimSpeed;

		CSphereMesh			m_SphereMesh;
		CEffectTechnique*	m_SkySphereTechnique;
		CTextureCube*		m_SkyTexture;
		CTexture*			m_SkyCloudTexture;
		D3DXHANDLE			m_SkyTextureParameter;
		D3DXHANDLE			m_SkyCloudTextureParameter;
		D3DXHANDLE			m_CloudTimerParameter;
		Mat44f				m_World;
		bool				m_Clouds;
		float				m_CloudTimer;

	public:
		CSkySphere();
		virtual ~CSkySphere();

		bool				Load						(CXMLTreeNode &XMLNode);
		bool				Render						();
};

#endif