
#include <assert.h>
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitter::CParticleEmitter(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition)
	: m_Name(name)
	, m_ParticleSystem(particleSystem)
	, m_TimeBetweenParticles(1.0f / particlesPerSecond)
	, m_CurrentPosition(initialPosition)
	, m_PreviousPosition(initialPosition)
	, m_HasNewPosition(false)
	, m_TimeLeftOver(0.0f)
{
	assert(particleSystem);
}

CParticleEmitter::~CParticleEmitter()
{
}

void CParticleEmitter::Update(float elapsedTime)
{
	if (elapsedTime > 0 && m_ParticleSystem != NULL)
	{
		Vect3f velocity = v3fZERO;

		if(m_HasNewPosition)
		{
			velocity = m_CurrentPosition - m_PreviousPosition;
			velocity = velocity / elapsedTime;
		}

		float timeToSpend = m_TimeLeftOver + elapsedTime;

		float currentTime = -m_TimeLeftOver;

		while (timeToSpend > m_TimeBetweenParticles)
		{
			currentTime += m_TimeBetweenParticles;
			timeToSpend -= m_TimeBetweenParticles;

			float mu = currentTime / elapsedTime;

			Vect3f position = m_PreviousPosition;
			position.Lerp(m_CurrentPosition, mu);

			position += CalculateParticlePosition();

			m_ParticleSystem->AddParticle(position, velocity);
		}

		m_TimeLeftOver = timeToSpend;
	}
} 