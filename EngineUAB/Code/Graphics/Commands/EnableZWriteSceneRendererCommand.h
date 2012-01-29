#pragma once

#ifndef _ENABLE_Z_WRITE_SCENE_RENDERER_COMMAND_H
#define _ENABLE_Z_WRITE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CEnableZWriteSceneRendererCommand : public CSceneRendererCommand
{
public:
	CEnableZWriteSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif