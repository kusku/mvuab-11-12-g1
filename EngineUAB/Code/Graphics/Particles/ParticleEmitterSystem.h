
#pragma once

#ifndef _PARTICLE_EMITTER_SYSTEM_H
#define _PARTICLE_EMITTER_SYSTEM_H

#include <string>

#include <map>
#include "Math\Vector3.h"
#include "Base.h"
#include "Utils\Types.h"
#include "Utils\Active.h"

class CParticleEmitter;
class CParticleEmitterSystemInstance;

class CParticleEmitterSystem
{

public:
	CParticleEmitterSystem(const std::string& name);
	virtual ~CParticleEmitterSystem();

	bool AddParticleEmitter(const std::string& name, CParticleEmitter* emitter);

	CParticleEmitterSystemInstance* GetParticleEmitterSystemInstance(const std::string& name);

private:
	typedef std::map<std::string, CParticleEmitter*> EmitterMap;
	typedef EmitterMap::iterator EmitterMapIt;
	
	typedef std::map<std::string, CParticleEmitterSystemInstance*> SystemInstanceMap;
	typedef SystemInstanceMap::iterator SystemInstanceMapIt;

	std::string			m_Name;
	SystemInstanceMap	m_SystemInstance;
	EmitterMap			m_EmitterMap;

};

#endif