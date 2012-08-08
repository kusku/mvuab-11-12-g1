
#pragma once

#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include <string>

#include "Math\Vector3.h"
#include "Base.h"
#include "Utils\Types.h"
#include "Utils\Active.h"

class CParticleSystem;

class CParticleEmitter : public CActive
{
public:
	CParticleEmitter(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition, bool useDis);
	virtual ~CParticleEmitter();

	void		Update		(float elapsedTime);

	//--Get & Set Methods----------------------------------------
	inline void SetParticlesEjectionCount(uint32 particleEjectionCount)	{ m_ParticlesEjectionCount = particleEjectionCount; }
	inline uint32 GetParticlesEjectionCount()	{ return m_ParticlesEjectionCount; }

	inline void SetOnLoop(bool onLoop)			{ m_OnLoop = onLoop; }
	inline bool GetOnLoop()						{ return m_OnLoop; }

	inline void EjectParticles()				{ m_EjectParticles = true; }

	inline void SetPosition(const Vect3f& position) 
	{ 
		if(position == m_CurrentPosition)
		{
			return;
		}

		m_HasNewPosition = true;
		m_PreviousPosition = m_CurrentPosition;
		m_CurrentPosition = position;
	}

	inline Vect3f GetPosition() const			{ return m_CurrentPosition; }
	inline void		SetParticleSystem	( CParticleSystem *_pSystem )	{ assert(_pSystem); m_ParticleSystem = _pSystem; }

protected:
	virtual Vect3f				CalculateParticlePosition()		= 0;

protected:
	std::string					m_Name;

	CParticleSystem*			m_ParticleSystem;
	float						m_TimeBetweenParticles;
	Vect3f						m_PreviousPosition;
	float						m_TimeLeftOver;

	bool						m_HasNewPosition;
	Vect3f						m_CurrentPosition;
	bool						m_UseDis;

	bool						m_OnLoop;
	bool						m_EjectParticles;
	uint32						m_ParticleCount;
	uint32						m_ParticlesEjectionCount;
};

#endif