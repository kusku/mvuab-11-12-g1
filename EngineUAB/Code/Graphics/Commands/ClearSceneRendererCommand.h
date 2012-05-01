#pragma once

#ifndef _CLEAR_SCENE_RENDERER_COMMAND_H
#define _CLEAR_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CClearSceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
			CClearSceneRendererCommand(CXMLTreeNode &Node);
			
	//----Main Methods ---------------------------------------------------
	virtual void	Execute		( CRenderManager &RM );

	//----Members ---------------------------------------------------
protected:
	bool		m_Color;
	bool		m_Depth;
	bool		m_Stencil;
};

#endif