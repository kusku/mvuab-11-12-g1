#include <assert.h>
#include "ParticleEmitterLine.h"
#include "Particles\ParticleSystem.h"
#include "Utils\BoostRandomHelper.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


CParticleEmitterLine::CParticleEmitterLine(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition, bool useDis, const Vect3f& lineStart, const Vect3f& lineEnd)
	: CParticleEmitter(name, particleSystem, particlesPerSecond, initialPosition, useDis)
	, m_LineStart(lineStart)
	, m_LineEnd(lineEnd)
{

}


CParticleEmitterLine::~CParticleEmitterLine()
{
}

Vect3f CParticleEmitterLine::CalculateParticlePosition()
{
	Vect3f pos = m_LineStart;

	float mu = BoostRandomHelper::GetNextFloat();

	pos.Lerp(m_LineEnd, mu);

	return pos;
}
