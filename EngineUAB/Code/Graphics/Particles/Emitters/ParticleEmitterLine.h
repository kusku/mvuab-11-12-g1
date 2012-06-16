
#pragma once

#ifndef _PARTICLE_EMITTER_LINE_H
#define _PARTICLE_EMITTER_LINE_H

#include <string>

#include "Math\Vector3.h"
#include "Particles\ParticleEmitter.h"

class CParticleSystem;

class CParticleEmitterLine : public CParticleEmitter
{
	protected:
		Vect3f				m_LineStart;
		Vect3f				m_LineEnd;

		Vect3f				CalculateParticlePosition	();

	public:
		CParticleEmitterLine(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition, const Vect3f& lineStart, const Vect3f& lineEnd);
		virtual ~CParticleEmitterLine();
};

#endif