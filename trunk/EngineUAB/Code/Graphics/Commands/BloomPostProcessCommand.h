#pragma once

#ifndef _BLOOM_POST_PROCESS_COMMAND_H
#define _BLOOM_POST_PROCESS_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CBloomPostProcess;

class CBloomPostProcessCommand : public CSceneRendererCommand
{
public:
	CBloomPostProcessCommand(CXMLTreeNode &Node);
	virtual ~CBloomPostProcessCommand();
	virtual void	Execute		( CRenderManager &RM );

private:
	CBloomPostProcess*		m_Bloom;
};

#endif