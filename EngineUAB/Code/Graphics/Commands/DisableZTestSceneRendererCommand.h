#pragma once

#ifndef _DISABLE_Z_TEST_SCENE_RENDERER_COMMAND_H
#define _DISABLE_Z_TEST_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CDisableZTestSceneRendererCommand : public CSceneRendererCommand
{
public:
	CDisableZTestSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif