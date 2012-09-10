#pragma once

#ifndef _RENDER_DEBUG_PHYSICS_SCENE_RENDERER_COMMAND_H
#define _RENDER_DEBUG_PHYSICS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CRenderableObjectsManager;

class CRenderDebugPhysicsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugPhysicsSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
	
private:
	bool	m_Drawfrustum;
	bool	m_DrawFront;
	bool	m_DrawNames;
	bool	m_DrawRays;
};

#endif