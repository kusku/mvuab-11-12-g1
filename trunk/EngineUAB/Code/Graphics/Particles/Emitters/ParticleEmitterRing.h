
#pragma once

#ifndef _PARTICLE_EMITTER_RING_H
#define _PARTICLE_EMITTER_RING_H

#include <string>

#include "Math\Vector3.h"
#include "Particles\ParticleEmitter.h"

class CParticleSystem;

class CParticleEmitterRing : public CParticleEmitter
{
	protected:
		float				m_Radius;
		float				m_Height;
		float				m_Width;
		bool				m_Horizontal;

		Vect3f				CalculateParticlePosition	();

	public:
		CParticleEmitterRing(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition, bool horizontal = false, float radius = 30.0f, float height = 40.0f, float width = 40.0f);
		virtual ~CParticleEmitterRing();
};

#endif