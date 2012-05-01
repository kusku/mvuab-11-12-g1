#pragma once

#ifndef __CLASS_SCENE_RENDERER_COMMAND_H__
#define __CLASS_SCENE_RENDERER_COMMAND_H__

#include "Utils\Named.h"
#include "Utils\Active.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CSceneRendererCommand : public CNamed, public CActive
{
public:
	//--- Init and End protocols------------------------------------------
					CSceneRendererCommand	( CXMLTreeNode &_Node );
	virtual			~CSceneRendererCommand	( void );
	
	//----Main Methods ---------------------------------------------------
	virtual void	Execute					( CRenderManager &RM ) = 0;
};

#endif __CLASS_SCENE_RENDERER_COMMAND_H__