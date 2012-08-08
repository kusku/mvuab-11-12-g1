

#pragma once

#ifndef _PARTICLE_SYSTEM_MANAGER_H
#define _PARTICLE_SYSTEM_MANAGER_H

#include "Utils\MapManager.h"
#include "ParticleSystem.h"

class CParticleSystemManager : public CMapManager<CParticleSystem>
{
private:
	std::string m_Filename;

public:
	CParticleSystemManager();
	virtual ~CParticleSystemManager();

	void		Update			(float elapsedTime);

	void		Render			();

	bool		Load					(const std::string& filename);
	bool		Reload					();
	void		CleanUp					();
	void		Initialize				();

	void		AddNewSystem			(const std::string &name);
};

#endif