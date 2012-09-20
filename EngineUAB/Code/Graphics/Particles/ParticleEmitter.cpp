
#include <assert.h>
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "ParticleEmitterInstance.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitter::CParticleEmitter(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition, bool useDis)
	: m_Name(name)
	, m_ParticleSystem(particleSystem)
	, m_TimeBetweenParticles(1.0f / particlesPerSecond)
	, m_InitialPosition(initialPosition)
	, m_OnLoop(true)
	, m_ParticlesEjectionCount(0)
	, m_UseDis(useDis)
{
	assert(particleSystem);
}

CParticleEmitter::~CParticleEmitter()
{
	EmitterInstanceMapIt it = m_InstanceMap.begin();
	EmitterInstanceMapIt itEnd = m_InstanceMap.end();

	for (it; it != itEnd; ++it)
	{
		CHECKED_DELETE(it->second);
	}

	m_InstanceMap.clear();
}

void CParticleEmitter::Update(float elapsedTime)
{
	if(!m_Active || elapsedTime == 0.0f || m_ParticleSystem == NULL)
	{
		return;
	}

	EmitterInstanceMapIt it = m_InstanceMap.begin();
	EmitterInstanceMapIt itEnd = m_InstanceMap.end();

	for (it; it != itEnd; ++it)
	{
		CParticleEmitterInstance* inst = it->second;

		if(inst->GetActive() && (m_OnLoop || inst->IsEjectingParticles()) )
		{
			Vect3f velocity = v3fZERO;
			Vect3f previous = inst->m_CurrentPosition;

			if(inst->m_HasNewPosition && m_UseDis)
			{
				previous = inst->m_PreviousPosition;
				velocity = inst->m_CurrentPosition - inst->m_PreviousPosition;
				velocity = velocity / elapsedTime;
				inst->m_HasNewPosition = false;
				inst->m_PreviousPosition = inst->m_CurrentPosition;
			}

			float timeToSpend = inst->m_TimeLeftOver + elapsedTime;

			float currentTime = -(inst->m_TimeLeftOver);

			while (timeToSpend > m_TimeBetweenParticles)
			{
				currentTime += m_TimeBetweenParticles;
				timeToSpend -= m_TimeBetweenParticles;

				float mu = currentTime / elapsedTime;

				Vect3f position = previous;
				position.Lerp(inst->m_CurrentPosition, mu);

				position += CalculateParticlePosition();

				m_ParticleSystem->AddParticle(position, velocity);
				inst->m_ParticleCount += 1;

				if(!m_OnLoop && inst->m_ParticleCount > m_ParticlesEjectionCount)
				{
					inst->m_EjectParticles = false;
					inst->m_ParticleCount = 0;
					timeToSpend = 0.0f;
				}
			}

			inst->m_TimeLeftOver = timeToSpend;
		}
	}
} 

CParticleEmitterInstance* CParticleEmitter::GetParticleEmitterInstance( const std::string& name )
{
	EmitterInstanceMapIt it = m_InstanceMap.find(name);

	if(it != m_InstanceMap.end())
	{
		return it->second;
	}

	CParticleEmitterInstance* inst = new CParticleEmitterInstance(name, m_InitialPosition);

	m_InstanceMap[name] = inst;

	return inst;
}
