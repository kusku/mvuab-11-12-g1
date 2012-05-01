#pragma once

#ifndef _BEGIN_SCENE_RENDERER_COMMAND_H
#define _BEGIN_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CBeginSceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
					CBeginSceneRendererCommand ( CXMLTreeNode &_Node );
	
	//----Main Methods ---------------------------------------------------
	virtual void	Execute		( CRenderManager &RM );
};

#endif