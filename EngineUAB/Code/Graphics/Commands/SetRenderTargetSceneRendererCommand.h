#pragma once

#ifndef _SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define _SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CPoolRenderableObjectTechnique;

class CSetRenderTargetSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
	CSetRenderTargetSceneRendererCommand(CXMLTreeNode &Node);
	void	Execute				( CRenderManager &RM );
	void	UnsetRenderTaget	();
};

#endif