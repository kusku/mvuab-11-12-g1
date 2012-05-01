#pragma once

#ifndef __CLASS_RENDER_DEBUG_SOUNDS_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_DEBUG_SOUNDS_SCENE_RENDERER_COMMAND_H__

#include "Commands\SceneRendererCommand.h"

class CRenderManager;
class CRenderableObjectsManager;
class CXMLTreeNode;

class CRenderDebugSoundsSceneRendererCommand : public CSceneRendererCommand
{
public:

	//--- Init and End protocols------------------------------------------
			CRenderDebugSoundsSceneRendererCommand	( CXMLTreeNode &_atts);
	
	//--- Mètodes principals---------------------------------------
	void	Execute			( CRenderManager &_RM );
	
};

#endif __CLASS_RENDER_DEBUG_SOUNDS_SCENE_RENDERER_COMMAND_H__