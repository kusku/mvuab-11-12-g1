#include "ParticleEmitter.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitter::CParticleEmitter()
	: m_Particles(50)
	, m_Position(Vect3f(0.f, 0.f, 0.f))
	, m_StartSpawnDir(Vect3f(0.f, 0.f, 0.f))
	, m_EndSpawnDir(Vect3f(0.f, 0.f, 0.f))
{
}

CParticleEmitter::~CParticleEmitter()
{
}