#pragma once

#ifndef _SETUP_MATRICES_SCENE_RENDERER_COMMAND_H
#define _SETUP_MATRICES_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CSetupMatricesSceneRendererCommand : public CSceneRendererCommand
{
public:
	CSetupMatricesSceneRendererCommand(CXMLTreeNode &Node);
	void	Execute				( CRenderManager &RM );
};

#endif