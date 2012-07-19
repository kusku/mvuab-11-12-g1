#pragma once

#ifndef _UNSET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H
#define _UNSET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CPoolRenderableObjectTechnique;
class CSetDepthStencilSceneRendererCommand;

class CUnsetDepthStencilSceneRendererCommand : public CSceneRendererCommand
{
private:
	CSetDepthStencilSceneRendererCommand			*m_SetDepthStencilSceneRendererCommand;

public:
	CUnsetDepthStencilSceneRendererCommand(CSetDepthStencilSceneRendererCommand *setDepthStencilSceneRendererCommand, CXMLTreeNode &Node);
	void	Execute				( CRenderManager &RM );
};

#endif