#pragma once

#ifndef _PARTICLE_EMITTER_BOX_H_
#define _PARTICLE_EMITTER_BOX_H_

#include <string>

#include "Math\Vector3.h"
#include "Particles\ParticleEmitter.h"

class CParticleSystem;

class CParticleEmitterBox : public CParticleEmitter
{
public:
	CParticleEmitterBox(const std::string& _Name, CParticleSystem* _ParticleSystem, float _ParticlesPerSecond, 
						const Vect3f& _InitialPosition, bool _UseDis, const Vect3f& _BoxMin, const Vect3f& _BoxMax);
	virtual ~CParticleEmitterBox();

protected:
	Vect3f	CalculateParticlePosition	();

protected:
	Vect3f		m_BoxMin;
	Vect3f		m_BoxMax;
};

#endif