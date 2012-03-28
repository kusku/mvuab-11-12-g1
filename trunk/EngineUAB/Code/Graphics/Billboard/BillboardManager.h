#pragma once

#ifndef _BILLBOARD_MANAGER_H
#define _BILLBOARD_MANAGER_H

class CCamera;
class CRenderManager;
class CBillboardAnimation;

#include "Utils\TemplatedVectorMapManager.h"
#include "BillboardDefs.h"
#include <string>
#include <vector>

class CBillboardManager : public CTemplatedVectorMapManager<CBillboardCore>
{
public:
	CBillboardManager();
	~CBillboardManager();

	bool		Load		( const std::string &filename );
	bool		Reload		();
	void		CleanUp		();

	void		Update		( float elapsedTime, CCamera &camera );
	void		Render		( CRenderManager &RM );

private:
	bool		LoadFile	();

	std::string								m_FileName;
	std::vector<CBillboardAnimation*>		m_BillboardInstancesVector;
};

#endif