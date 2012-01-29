#pragma once

#ifndef _DISABLE_Z_WRITE_SCENE_RENDERER_COMMAND_H
#define _DISABLE_Z_WRITE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CDisableZWriteSceneRendererCommand : public CSceneRendererCommand
{
public:
	CDisableZWriteSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif