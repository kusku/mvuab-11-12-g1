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
#include "../BoundingObjects.h"

class CStaticMesh
{
public:
	CStaticMesh();
	virtual ~CStaticMesh();

	bool			Load				( const std::string &FileName );
	bool			Reload				();
	void			Unload				();

	void			Render				( CRenderManager *RM ) const;

	void			SetBoundingBox		(const TBoundingBox& boundingBox) { m_BoundingBox = boundingBox; }
	void			SetBoundingSphere	(const TBoundingSphere& boundingSphere) { m_BoundingSphere = boundingSphere; }

	TBoundingBox	GetBoundingBox		() const { return m_BoundingBox; }
	TBoundingSphere GetBoundingSphere	() const { return m_BoundingSphere; }

protected:
	std::vector<CRenderableVertexs*>		m_RVs;
	std::vector<std::vector<CTexture*>>		m_Textures;
	std::string								m_FileName;
	uint32									m_NumVertexs, m_NumFaces;
	TBoundingBox							m_BoundingBox;
	TBoundingSphere							m_BoundingSphere;

	template<class T>
	void*				LoadCreateVertexBuffer	(FILE* modelFile, uint16 numVertex);
	void				ClearRenderableVertex	();
	void				ClearTextures			();
	bool				ExtractTexture			(FILE* modelFile, std::vector<CTexture*>& textVector);
	void				ClearTextureVector		(std::vector<CTexture*>& textVector);
	CRenderableVertexs*	ReadCreateVertexBuffer	(FILE* modelFile, uint16 vertexType);
	bool				ExtractMesh				(FILE* modelFile);
	bool				GetBoundingBoxAndSphere	(FILE* modelFile);
};

#endif