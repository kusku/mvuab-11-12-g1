#pragma once

#ifndef __CLASS_DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H__
#define __CLASS_DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H__

#include "StagedTexturedRendererCommand.h"

//---Foward Declarations---//
class CRenderableObjectTechnique;
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CDeferredShadingSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
			CDeferredShadingSceneRendererCommand( CXMLTreeNode &_Node );

	//----Main Methods ---------------------------------------------------
	void	Execute			( CRenderManager &_RM );
	void	SetLightsData	( CRenderManager &_RM );

	//--- Members ---------------------------------------------------------
private:
	CRenderableObjectTechnique		*m_pRenderableObjectTechnique;
};

#endif __CLASS_DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H__