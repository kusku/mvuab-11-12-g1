#pragma once

#ifndef _RENDER_PARTICLES_SCENE_RENDERER_COMMAND_H
#define _RENDER_PARTICLES_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CRenderParticlesSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderParticlesSceneRendererCommand (CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif