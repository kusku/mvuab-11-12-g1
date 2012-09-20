
#pragma once

#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include <string>
#include <map>

#include "Math\Vector3.h"
#include "Base.h"
#include "Utils\Types.h"
#include "Utils\Active.h"


class CParticleSystem;
class CParticleEmitterInstance;

class CParticleEmitter : public CActive
{
	typedef std::map<std::string, CParticleEmitterInstance*> EmitterInstanceMap;
	typedef EmitterInstanceMap::iterator EmitterInstanceMapIt;

public:
	CParticleEmitter(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition, bool useDis);
	virtual ~CParticleEmitter();

	void		Update		(float elapsedTime);

	//--Get & Set Methods----------------------------------------
	inline void SetParticlesEjectionCount(uint32 particleEjectionCount)	{ m_ParticlesEjectionCount = particleEjectionCount; }
	inline uint32 GetParticlesEjectionCount()	{ return m_ParticlesEjectionCount; }

	inline void SetOnLoop(bool onLoop)			{ m_OnLoop = onLoop; }
	inline bool GetOnLoop()						{ return m_OnLoop; }
	
	inline void		SetParticleSystem	( CParticleSystem *_pSystem )	{ assert(_pSystem); m_ParticleSystem = _pSystem; }


	CParticleEmitterInstance* GetParticleEmitterInstance(const std::string& name);

	inline std::string	GetName() const { return m_Name; }

protected:
	virtual Vect3f				CalculateParticlePosition()		= 0;

protected:

	EmitterInstanceMap			m_InstanceMap;

	std::string					m_Name;
	CParticleSystem*			m_ParticleSystem;
	Vect3f						m_InitialPosition;
	bool						m_UseDis;
	float						m_TimeBetweenParticles;
	bool						m_OnLoop;
	uint32						m_ParticlesEjectionCount;
};

#endif