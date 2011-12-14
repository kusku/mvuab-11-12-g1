#pragma once

#ifndef _STATICMESH_H
#define _STATICMESH_H

class CRenderManager;
class CRenderableVertexs;
class CTexture;

#include <vector>
#include <string>
#include "Utils\Types.h"
#include "Math\Vector3.h"

class CStaticMesh
{
public:
	CStaticMesh();
	virtual ~CStaticMesh();

	bool		Load		( const std::string &FileName );
	bool		Reload		();
	void		Unload		();

	void		Render					( CRenderManager *RM ) const;

protected:
	std::vector<CRenderableVertexs*>		m_RVs;
	std::vector<std::vector<CTexture*>>		m_Textures;
	std::string								m_FileName;
	uint32									m_NumVertexs, m_NumFaces;
	Vect3f									m_MinBB, m_MaxBB;
	Vect3f									m_Center;
	float									m_Radius;

	bool		LoadFile		();
	template<class T>
	void*		LoadVtxs		(FILE *file, uint16 &VCount_);
};

#endif