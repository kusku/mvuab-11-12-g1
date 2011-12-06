#pragma once

#ifndef _STATICMESH_H
#define _STATICMESH_H

class CRenderManager;
class CRenderableVertexs;
class CTexture;

#include <vector>
#include <string>
#include "Utils\Types.h"

class CStaticMesh
{
public:
	CStaticMesh();
	~CStaticMesh();

	bool		Load		( const std::string &FileName );
	bool		Reload		();
	void		Unload		();
	void		Render		( CRenderManager *RM ) const;

protected:
	std::vector<CRenderableVertexs*>		m_RVs;
	std::vector<std::vector<CTexture*>>		m_Textures;
	std::string								m_FileName;
	uint32									m_NumVertexs, m_NumFaces;

	bool		LoadFile		();
};

#endif