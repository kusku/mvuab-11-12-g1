#pragma once

#ifndef _RENDER_SCENE_SCENE_RENDERER_COMMAND_H
#define _RENDER_SCENE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CRenderableObjectsManager;

class CRenderSceneSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderSceneSceneRendererCommand(CXMLTreeNode &Node);
	virtual ~CRenderSceneSceneRendererCommand();

	virtual void	Execute		( CRenderManager &RM );

private:
	CRenderableObjectsManager		*m_pRenderableObjectsManager;
};

#endif