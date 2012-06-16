//----------------------------------------------------------------------------------
// CStaticMeshManager class
// Author: Marc Cuscullola
//
// Description:
// Gestiona y carga todos los Static Meshes
//----------------------------------------------------------------------------------
#pragma once

#ifndef _STATICMESHMANAGER_H
#define _STATICMESHMANAGER_H

#include "RenderManager.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "StaticMesh.h"
#include <string>

class CStaticMeshManager : public CTemplatedVectorMapManager<CStaticMesh>
{
public:
	CStaticMeshManager();
	virtual ~CStaticMeshManager();

	bool		Load		( const std::string &FileName );
	bool		Reload		();
	void		CleanUp		();

	void		CreateRenderableObjectsTechniques	();

protected:
	std::string			m_FileName;

	bool		LoadXML	();
};

#endif
