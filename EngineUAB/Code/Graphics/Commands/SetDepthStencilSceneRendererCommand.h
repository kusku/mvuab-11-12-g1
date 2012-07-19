#pragma once

#ifndef _SET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H
#define _SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CPoolRenderableObjectTechnique;
class CTexture;

class CSetDepthStencilSceneRendererCommand : public CSceneRendererCommand
{
private:
	CTexture* m_DepthStecilTexture;

public:
	CSetDepthStencilSceneRendererCommand(CXMLTreeNode &Node);
	void	Execute				( CRenderManager &RM );
	void	UnsetDepthStencil	();
};

#endif