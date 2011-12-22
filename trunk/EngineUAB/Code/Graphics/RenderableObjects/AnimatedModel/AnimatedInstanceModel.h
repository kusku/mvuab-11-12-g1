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

#include "cal3d\cal3d.h"
#include "Utils\BaseUtils.h"
#include "RenderableObjects/RenderableObject.h"
#include <vector>

class CAnimatedInstanceModel : public CRenderableObject
{
public:
	CAnimatedInstanceModel();
	virtual ~CAnimatedInstanceModel();

	void		Initialize			( CAnimatedCoreModel *AnimatedCoreModel );

	void		Update				( float elapsedTime );

	void		Render						( CRenderManager *RM );
	void		RenderModelBySoftware		( CRenderManager *RM );

	void		Destroy		();

	void		ExecuteAction		( uint32 Id, float Time );
	void		BlendCycle			( uint32 Id, float Time );
	void		ClearCycle			( float Time );
	
private:
	void			CalculateNumVtxsIdxs	();
	void			LoadTextures			();


	CalModel*								m_CalModel;
	CAnimatedCoreModel*						m_AnimatedCoreModel;
	std::vector<CTexture*>					m_TextureVector;

	uint32						m_NumVtxs;
	uint32						m_NumFaces;
};

#endif
