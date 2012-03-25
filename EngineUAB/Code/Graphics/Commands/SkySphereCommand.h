#pragma once

#ifndef _SKY_SPHERE_COMMAND_H
#define _SKY_SPHERE_COMMAND_H

#include "SceneRendererCommand.h"
#include "SkyCovers\SkySphere.h"

class CRenderManager;
class CXMLTreeNode;

class CSkySphereCommand : public CSceneRendererCommand
{
protected:
	CSkySphere		m_SkySphere;

public:
	CSkySphereCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif