
#pragma once

#ifndef _PARTICLE_EMITTER_INSTANCE_H
#define _PARTICLE_EMITTER_INSTANCE_H

#include <string>

#include "Math\Vector3.h"
#include "Base.h"
#include "Utils\Types.h"
#include "Utils\Active.h"

class CParticleSystem;

class CParticleEmitterInstance : public CActive
{
	//Friend Class
	friend class CParticleEmitter;
	
public:
	CParticleEmitterInstance(const std::string& name, const Vect3f& initialPosition);
	virtual ~CParticleEmitterInstance();
	
	//--Get & Set Methods----------------------------------------
	inline void EjectParticles()				{ m_EjectParticles = true; }

	inline void StopEjectParticles()
	{
		m_EjectParticles = false;
		m_ParticleCount = 0;
	}

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

	inline bool IsEjectingParticles() const { return m_EjectParticles; }

	inline Vect3f GetPosition() const { return m_CurrentPosition; }
	
protected:

	std::string					m_Name;
	Vect3f						m_PreviousPosition;
	float						m_TimeLeftOver;
	bool						m_HasNewPosition;
	Vect3f						m_CurrentPosition;
	bool						m_EjectParticles;
	uint32						m_ParticleCount;
};

#endif