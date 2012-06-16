
#pragma once

#ifndef _PARTICLE_EMITTER_POINT_H
#define _PARTICLE_EMITTER_POINT_H

#include <string>

#include "Math\Vector3.h"
#include "Particles\ParticleEmitter.h"

class CParticleSystem;

class CParticleEmitterPoint : public CParticleEmitter
{
	protected:
		Vect3f				CalculateParticlePosition	();

	public:
		CParticleEmitterPoint(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition, bool useDis);
		virtual ~CParticleEmitterPoint();
};

#endif