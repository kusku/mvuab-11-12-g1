#pragma once

#ifndef _BLOOM_POST_PROCESS_COMMAND_H
#define _BLOOM_POST_PROCESS_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
class CBloomPostProcess;
//-------------------------//

class CBloomPostProcessCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
					CBloomPostProcessCommand	( CXMLTreeNode &_Node );
	virtual			~CBloomPostProcessCommand	( void );

	//----Main Methods ---------------------------------------------------
	virtual void	Execute						( CRenderManager &_RM );

	//----Members --------------------------------------------------------
private:
	CBloomPostProcess*		m_Bloom;
};

#endif