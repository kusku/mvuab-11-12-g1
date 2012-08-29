#pragma once

#ifndef _LENS_FLARE_SCENE_RENDERER_COMMAND_H
#define _LENS_FLARE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
class CLensFlarePostProcess;
//-------------------------//

class CLensFlareSceneRemdererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
					CLensFlareSceneRemdererCommand	( CXMLTreeNode &_Node );
	virtual			~CLensFlareSceneRemdererCommand	( void );

	//----Main Methods ---------------------------------------------------
	virtual void	Execute						( CRenderManager &_RM );

	//----Members --------------------------------------------------------
private:
	CLensFlarePostProcess*		m_LensFlare;
	int							m_LightIndex;
};

#endif