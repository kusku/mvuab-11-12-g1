#pragma once

#ifndef _RENDER_BILLBOARDS_SCENE_RENDERER_COMMAND_H
#define _RENDER_BILLBOARDS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CRenderBillboardsSceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
	CRenderBillboardsSceneRendererCommand (CXMLTreeNode &_Node);
	
	//----Main Methods ---------------------------------------------------
	virtual void	Execute		( CRenderManager &_RM );
};

#endif