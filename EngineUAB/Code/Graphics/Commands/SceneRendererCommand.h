#pragma once

#ifndef _SCENE_RENDERER_COMMAND_H
#define _SCENE_RENDERER_COMMAND_H

#include "Utils\Named.h"
#include "Utils\Active.h"

class CRenderManager;
class CXMLTreeNode;

class CSceneRendererCommand : public CNamed, public CActive
{
public:
	CSceneRendererCommand();
	CSceneRendererCommand(CXMLTreeNode &Node);
	virtual ~CSceneRendererCommand();

	virtual void Execute(CRenderManager &RM) = 0;
};

#endif