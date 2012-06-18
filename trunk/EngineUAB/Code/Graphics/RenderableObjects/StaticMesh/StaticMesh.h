#pragma once

#ifndef _STATIC_MESH_H
#define _STATIC_MESH_H

class CRenderManager;
class CRenderableVertexs;
class CRenderableObjectTechnique;
class CTexture;

#include <vector>
#include <string>
#include "Utils\Types.h"
#include "Math\Vector3.h"
#include "BoundingObjects.h"
#include <d3dx9.h>

class CEffectTechnique;

class CStaticMesh
{
public:
	CStaticMesh();
	virtual ~CStaticMesh();

	bool				Load							( const std::string &FileName );
	bool				Reload							();
	void				Unload							();

	void				RenderInstance					(CRenderManager* RM, LPDIRECT3DVERTEXBUFFER9 instanceBuffer, uint32 count) const;
	void				Render							( CRenderManager *RM ) const;
	void				Render							( CRenderManager *RM, CEffectTechnique* technique ) const;

	void				SetBoundingBox					(const TBoundingBox& boundingBox) { m_BoundingBox = boundingBox; }
	void				SetBoundingSphere				(const TBoundingSphere& boundingSphere) { m_BoundingSphere = boundingSphere; }

	TBoundingBox		GetBoundingBox					() const { return m_BoundingBox; }
	TBoundingSphere		GetBoundingSphere				() const { return m_BoundingSphere; }

	bool				GetRenderableObjectTechnique	();

	void				ClearTextures					();

	// TODO:: queria recuperar el vertex list del buffer de DX9
private:
	template <typename T>
	void				CreateVect3fVertexsList			( T* _VtxBuffer,  uint32 _NumVertex );
	void				CreateVect3fFacesList			( const void *_IndxBuffer, uint32 _NumIndex );

public:
	const std::vector<Vect3f>&	GetVertexBuffer			( void ) const		{ return m_VtxsBuffer; }
	const std::vector<uint32>&	GetFacesBuffer			( void ) const		{ return m_IndxBuffer; }

protected:	
	std::vector<CRenderableVertexs*>			m_RVs;
	std::vector<std::vector<CTexture*>>			m_Textures;
	std::vector<int>							m_VertexTypes;
	std::vector<CRenderableObjectTechnique*>	m_RenderableObjectsTechniques;
	std::vector<CRenderableObjectTechnique*>	m_RenderableObjectsTechniquesInstance;
	std::string									m_FileName;
	std::string									m_RenderableObjecTechniqueName;
	uint32										m_NumVertexs, m_NumFaces;
	TBoundingBox								m_BoundingBox;
	TBoundingSphere								m_BoundingSphere;
	std::string									m_MeshName;

	template<class T>
	void*				LoadCreateVertexBuffer	(FILE* modelFile, uint16 numVertex);
	void				ClearRenderableVertex	();
	bool				ExtractTexture			(FILE* modelFile, std::vector<CTexture*>& textVector);
	void				AddTexVecToTexManager	(std::vector<CTexture*>& textVector);
	void				ClearTextureVector		(std::vector<CTexture*>& textVector);
	CRenderableVertexs*	ReadCreateVertexBuffer	(FILE* modelFile, uint16 vertexType);
	bool				ExtractMesh				(FILE* modelFile);
	bool				GetBoundingBoxAndSphere	(FILE* modelFile);

private:
	std::vector<Vect3f> m_VtxsBuffer;
	std::vector<uint32> m_IndxBuffer;
};

#endif