
#pragma once

#ifndef _PARTICLE_STARTUP_INSTANCES_H
#define _PARTICLE_STARTUP_INSTANCES_H

#include <string>

#include <map>
#include "Math\Vector3.h"
#include "Base.h"
#include "Utils\Types.h"
#include "Utils\Active.h"

class CParticleStartUpInstances
{

public:
	CParticleStartUpInstances();
	virtual ~CParticleStartUpInstances();

	bool			Load					(const std::string& filename);
	bool			Reload					();

private:
	std::string		m_Filename;
};

#endif