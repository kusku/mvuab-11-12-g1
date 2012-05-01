#pragma once

#ifndef _RENDER_PARTICLES_SCENE_RENDERER_COMMAND_H
#define _RENDER_PARTICLES_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CRenderParticlesSceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
	CRenderParticlesSceneRendererCommand (CXMLTreeNode &_Node);
	
	//----Main Methods ---------------------------------------------------
	virtual void	Execute		( CRenderManager &_RM );
};

#endif