#pragma once

#ifndef _SKY_SPHERE_COMMAND_H
#define _SKY_SPHERE_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CSkySphere;

class CSkySphereCommand : public CSceneRendererCommand
{
protected:
	CSkySphere*		m_SkySphere;

public:
	CSkySphereCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif