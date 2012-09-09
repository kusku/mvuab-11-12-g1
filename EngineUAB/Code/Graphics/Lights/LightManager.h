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

#include "Utils\TemplatedVectorMapManager.h"
#include "Light.h"
#include "Utils\Types.h"
#include <string>
#include <vector>

class CFrustum;

class CLightManager : public CTemplatedVectorMapManager<CLight>
{
public:
	CLightManager();
	virtual ~CLightManager();

	bool		Load		( const std::string &Filename );
	bool		Reload		();
	void		CleanUp		();
	void		Render		( CRenderManager *RM );

	void		SetCurrentFrustum	(CFrustum* cf) { m_CurrentFrustum = cf; }
	inline CFrustum*	GetCurrentFrustum	() const { return m_CurrentFrustum; }

protected:
	bool		LoadFile	();

	CFrustum*		m_CurrentFrustum;
	std::string		m_FileName;
};

#endif

