#pragma once

#ifndef _PARTICLE_MANAGER_H
#define _PARTICLE_MANAGER_H

class CCamera;
class CRenderManager;

#include "Utils\TemplatedVectorMapManager.h"
#include "ParticleEmitter.h"
#include <string>

class CParticleManager : CTemplatedVectorMapManager<CParticleEmitter>
{
public:
	CParticleManager();
	virtual ~CParticleManager();

	bool	Load		(const std::string &Filename);
	bool	Reload		();
	void	CleanUp		();

	void	Update		(float elapsedTime, CCamera &camera);
	void	Render		(CRenderManager &RM);

private:
	bool	LoadFile	();

	std::string			m_Filename;

};

#endif