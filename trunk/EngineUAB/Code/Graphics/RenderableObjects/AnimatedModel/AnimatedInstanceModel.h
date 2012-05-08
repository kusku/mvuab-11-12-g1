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
class CXMLTreeNode;

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

	void		ReadDataXML			( CXMLTreeNode &Node );
	void		Initialize			( CAnimatedCoreModel *AnimatedCoreModel );

	void		Update				( float elapsedTime );

	void		Render						( CRenderManager *RM );
	void		RenderModelBySoftware		( CRenderManager *RM );
	void		RenderModelByHardware		( CRenderManager* RM );

	void		Destroy		();

	void		ExecuteAction		( uint32 Id, float Time );
	void		BlendCycle			( uint32 Id, float Time );
	void		ClearCycle			( uint32 Id, float Time );
	
private:
	CalModel*								m_CalModel;
	CAnimatedCoreModel*						m_AnimatedCoreModel;

	LPDIRECT3DVERTEXBUFFER9				m_pVB;
	LPDIRECT3DINDEXBUFFER9				m_pIB;
};

#endif
