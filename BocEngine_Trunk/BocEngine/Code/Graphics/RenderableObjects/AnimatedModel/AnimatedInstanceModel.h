//----------------------------------------------------------------------------------
// CAnimatedInstanceModel class
// Author: Marc Cuscullola
//
// Description:
// Crea y gestiona una instancia de una core de un modelo animado.
//----------------------------------------------------------------------------------

#pragma once

#ifndef _ANIMATED_INSTANCE_MODEL_H
#define _ANIMATED_INSTANCE_MODEL_H

class CTexture;
class CRenderManager;
class CAnimatedCoreModel;
class CEffectTechnique;

#include "cal3d\cal3d.h"
#include "Utils\BaseUtils.h"

#include "RenderableObjects/RenderableObject.h"

#include <vector>
#include <d3dx9.h>
class CAnimatedInstanceModel : public CRenderableObject
{
public:
	CAnimatedInstanceModel();
	virtual ~CAnimatedInstanceModel();

	void		Initialize			( CAnimatedCoreModel *AnimatedCoreModel );

	void		Update				( float elapsedTime );

	void		Render						( CRenderManager *RM );
	void		Render						( CRenderManager *RM, CEffectTechnique* technique );
	void		RenderModelBySoftware		( CRenderManager *RM );

	void		Destroy		();

	void		ExecuteAction		( uint32 Id, float Time );
	void		BlendCycle			( uint32 Id, float Time );
	void		ClearCycle			( float Time );
	
private:
	bool			CalculateNumVtxsIdxs	( CRenderManager *RM);
	void			LoadTextures			();


	CalModel*								m_CalModel;
	CAnimatedCoreModel*						m_AnimatedCoreModel;
	std::vector<CTexture*>					m_TextureVector;

	LPDIRECT3DVERTEXBUFFER9				m_pVB;
	LPDIRECT3DINDEXBUFFER9				m_pIB;

	uint32						m_NumVtxs;
	uint32						m_NumFaces;
};

#endif
