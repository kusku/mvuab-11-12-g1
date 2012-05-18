#pragma once

#ifndef _RENDER_DEBUG_WAYPOINT_SCENE_RENDERER_COMMAND_H
#define _RENDER_DEBUG_WAYPOINT_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CRenderableObjectsManager;

class CRenderDebugWayPointSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugWayPointSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif