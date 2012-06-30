#pragma once

#ifndef _PARTICLE_EMITTER_SPHERE_H_
#define _PARTICLE_EMITTER_SPHERE_H_

#include <string>

#include "Math\Vector3.h"
#include "Particles\ParticleEmitter.h"

class CParticleSystem;

class CParticleEmitterSphere : public CParticleEmitter
{
public:
	CParticleEmitterSphere(const std::string& _Name, CParticleSystem* _ParticleSystem, float _ParticlesPerSecond, 
						const Vect3f& _InitialPosition, bool _UseDis, float _Radius);
	virtual ~CParticleEmitterSphere();

protected:
	Vect3f	CalculateParticlePosition	();

protected:
	float	m_fRadius;

};

#endif