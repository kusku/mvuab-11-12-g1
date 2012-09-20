
#pragma once

#ifndef _PARTICLE_EMITTER_SYSTEM_INSTANCE_H
#define _PARTICLE_EMITTER_SYSTEM_INSTANCE_H

#include <string>

#include <map>
#include "Math\Vector3.h"
#include "Base.h"
#include "Utils\Types.h"
#include "Utils\Active.h"

class CParticleEmitterInstance;

class CParticleEmitterSystemInstance
{
public:
	CParticleEmitterSystemInstance(const std::string& name);
	virtual ~CParticleEmitterSystemInstance();
	
	bool AddEmitterInstance(const std::string& name, CParticleEmitterInstance* inst);

	CParticleEmitterInstance* GetEmitterInstance(const std::string& name);

	void SetPositionToAll(const Vect3f& pos);

protected:
	typedef std::map<std::string, CParticleEmitterInstance*> EIMap;
	typedef EIMap::iterator EIMapIt;

	EIMap	m_EmitterInstanceMap;

	std::string m_Name;
};

#endif