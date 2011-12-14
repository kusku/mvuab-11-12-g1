#pragma once

#ifndef _STATICMESHMANAGER_H
#define _STATICMESHMANAGER_H

#include "../RenderManager.h"
#include "Utils\MapManager.h"
#include "StaticMesh.h"
#include <string>

class CStaticMeshManager : public CMapManager<CStaticMesh>
{
public:
	CStaticMeshManager();
	virtual ~CStaticMeshManager();

	bool		Load		( const std::string &FileName );
	bool		Reload		();

	void		Render		(CRenderManager *RM, const std::string &name);

protected:
	std::string			m_FileName;

	bool		LoadXML	();
};

#endif
