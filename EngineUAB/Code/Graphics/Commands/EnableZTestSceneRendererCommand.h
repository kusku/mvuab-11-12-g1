#pragma once

#ifndef _ENABLE_Z_TEST_SCENE_RENDERER_COMMAND_H
#define _ENABLE_Z_TEST_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CEnableZTestSceneRendererCommand : public CSceneRendererCommand
{
public:
	CEnableZTestSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif