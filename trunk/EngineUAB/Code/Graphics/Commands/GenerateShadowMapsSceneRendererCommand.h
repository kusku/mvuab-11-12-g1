#pragma once

#ifndef _GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H
#define _GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CGenerateShadowMapsSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
	CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &Node);
	void	Execute				( CRenderManager &RM );
};

#endif