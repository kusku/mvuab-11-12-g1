#pragma once

#ifndef _UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define _UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CPoolRenderableObjectTechnique;
class CSetRenderTargetSceneRendererCommand;

class CUnsetRenderTargetSceneRendererCommand : public CStagedTexturedRendererCommand
{
private:
	CSetRenderTargetSceneRendererCommand			*m_pSetRenderTargetRendererCommand;
public:
	CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SetRenderTargetRendererCommand, CXMLTreeNode &Node);
	void	Execute				( CRenderManager &RM );
};

#endif