
#include <assert.h>
#include "ParticleEmitterPoint.h"
#include "Particles\ParticleSystem.h"
#include "Utils\BoostRandomHelper.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


CParticleEmitterPoint::CParticleEmitterPoint(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition)
	: CParticleEmitter(name, particleSystem, particlesPerSecond, initialPosition)
{

}


CParticleEmitterPoint::~CParticleEmitterPoint()
{
}

Vect3f CParticleEmitterPoint::CalculateParticlePosition()
{
	return v3fZERO;
}
