
#pragma once

#ifndef _SKY_SPHERE_H
#define _SKY_SPHERE_H

#include "RenderableObjects\Primitive Meshes\SphereMesh.h"
#include "Math\Matrix44.h"

class CEffectTechnique;
class CXMLTreeNode;
class CTextureCube;

class CSkySphere
{
	private:
		CSphereMesh			m_SphereMesh;
		CEffectTechnique*	m_SkySphereTechnique;
		CTextureCube*		m_SkyTexture;
		D3DXHANDLE			m_SkyTextureParameter;
		Mat44f				m_World;

	public:
		CSkySphere();
		virtual ~CSkySphere();

		bool				Load						(CXMLTreeNode &XMLNode);
		bool				Render						();
};

#endif