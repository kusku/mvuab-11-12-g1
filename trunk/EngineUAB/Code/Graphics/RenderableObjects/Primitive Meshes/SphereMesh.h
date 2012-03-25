#pragma once

#ifndef _SPHERE_MESH_H
#define _SPHERE_MESH_H

#include <vector>
#include <map>
#include <string>

#include "Base.h"
#include "Vertexs\VertexType.h"
#include "Math\Color.h"

class CRenderManager;
class CRenderableVertexs;
class CRenderableObjectTechnique;
class CEffectTechnique;

class CSphereMesh
{
	protected:
		std::string						m_MeshName;
		std::string						m_RenderableObjecTechniqueName;
		uint32							m_NumVertexs, m_NumFaces;
		CRenderableVertexs*				m_RV;
		CColor							m_Color;

		//Temp Variables
		uint32							m_RecursionLvl;
		std::map<std::string, uint16>	m_VtxMapTemp;
		std::vector<TCOLORED_VERTEX>	m_VtxBuffTemp;
		std::vector<uint16>				m_IdxBuffTemp;

		//Methods
		void							BuildSphere			();
		uint16							AddVtx				(TCOLORED_VERTEX& vtx);
		std::string						GetVtxString		(TCOLORED_VERTEX& vtx);
		uint16							GetMiddlePoint		(uint32 vtxIndex1, uint32 vtxIndex2);
		void							CreateRV			();

	public:
		CSphereMesh(uint32 recursionLvl, const CColor& color);
		virtual ~CSphereMesh();
		
		void							Render				(CRenderManager *RM, CEffectTechnique* technique) const;
		void							CreateSphere		(uint32 recursionLvl, const CColor& color);

};

#endif