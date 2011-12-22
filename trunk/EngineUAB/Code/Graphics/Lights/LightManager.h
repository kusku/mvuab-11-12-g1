//----------------------------------------------------------------------------------
// CLightManager class
// Author: Marc Cuscullola
//
// Description:
// Gestiona todas las luces
//----------------------------------------------------------------------------------
#pragma once

#ifndef _LIGHTMANAGER_H
#define _LIGHTMANAGER_H

class CRenderManager;

#include "Utils\MapManager.h"
#include "Light.h"
#include <string>

class CLightManager : public CMapManager<CLight>
{
public:
	CLightManager();
	virtual ~CLightManager();

	bool		Load		( const std::string &Filename );
	void		Render		( CRenderManager *RM );
};

#endif

