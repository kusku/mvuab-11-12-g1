//----------------------------------------------------------------------------------
// CAnimatedModelManager class
// Author: Marc Cuscullola
//
// Description:
// Manager que gestiona todas las cores de los modelos animados
//----------------------------------------------------------------------------------

#pragma once

#ifndef _ANIMATED_MODEL_MANAGER_H
#define _ANIMATED_MODEL_MANAGER_H

class CAnimatedInstanceModel;

#include "Utils\MapManager.h"
#include "AnimatedCoreModel.h"
#include <string>

class CAnimatedModelManager : public CMapManager<CAnimatedCoreModel>
{
public:
	CAnimatedModelManager();
	virtual ~CAnimatedModelManager();

	void		Load		( const std::string &Filename );
	void		CleanUp		();

	CAnimatedCoreModel*			GetCore			( const std::string &Name, const std::string &Path, const std::string &XMLFilename );
	CAnimatedInstanceModel*		GetInstance		( const std::string &Name );
};

#endif