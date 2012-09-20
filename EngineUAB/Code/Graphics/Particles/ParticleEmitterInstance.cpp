
#include <assert.h>
#include "ParticleEmitterInstance.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitterInstance::CParticleEmitterInstance(const std::string& name, const Vect3f& initialPosition)
	: m_Name(name)
	, m_CurrentPosition(initialPosition)
	, m_PreviousPosition(initialPosition)
	, m_TimeLeftOver(0.0f)
	, m_HasNewPosition(false)
	, m_ParticleCount(0)
	, m_EjectParticles(false)
{
}

CParticleEmitterInstance::~CParticleEmitterInstance()
{
}