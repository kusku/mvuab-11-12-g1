
#include <assert.h>
#include "ParticleEmitterSystemInstance.h"
#include "ParticleEmitterInstance.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitterSystemInstance::CParticleEmitterSystemInstance(const std::string& name)
	: m_Name(name)
{
}

CParticleEmitterSystemInstance::~CParticleEmitterSystemInstance()
{
}

CParticleEmitterInstance* CParticleEmitterSystemInstance::GetEmitterInstance( const std::string& name )
{
	EIMapIt it = m_EmitterInstanceMap.find(name);

	if(it != m_EmitterInstanceMap.end())
	{
		return it->second;
	}

	return NULL;
}

bool CParticleEmitterSystemInstance::AddEmitterInstance( const std::string& name, CParticleEmitterInstance* inst )
{
	assert(inst);

	if(inst == NULL)
	{
		return false;
	}

	EIMapIt it = m_EmitterInstanceMap.find(name);

	if(it != m_EmitterInstanceMap.end())
	{
		return false;
	}

	m_EmitterInstanceMap[name] = inst;

	return true;
}

void CParticleEmitterSystemInstance::SetPositionToAll( const Vect3f& pos )
{
	EIMapIt it = m_EmitterInstanceMap.begin();
	EIMapIt itEnd = m_EmitterInstanceMap.end();

	for (it; it != itEnd; ++it)
	{
		it->second->SetPosition(pos);
	}
}
