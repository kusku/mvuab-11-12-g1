
#include <assert.h>
#include "ParticleEmitterSystem.h"
#include "ParticleEmitterSystemInstance.h"
#include "ParticleEmitter.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitterSystem::CParticleEmitterSystem(const std::string& name)
	: m_Name(name)
{
}

CParticleEmitterSystem::~CParticleEmitterSystem()
{
	SystemInstanceMapIt it = m_SystemInstance.begin();
	SystemInstanceMapIt itEnd = m_SystemInstance.end();

	for (it; it != itEnd; ++it)
	{
		CHECKED_DELETE(it->second)
	}

	m_SystemInstance.clear();
}

CParticleEmitterSystemInstance* CParticleEmitterSystem::GetParticleEmitterSystemInstance( const std::string& name )
{
	SystemInstanceMapIt isFound = m_SystemInstance.find(name);

	if(isFound != m_SystemInstance.end())
	{
		return isFound->second;
	}

	CParticleEmitterSystemInstance* inst = new CParticleEmitterSystemInstance(name);
	
	EmitterMapIt it = m_EmitterMap.begin();
	EmitterMapIt itEnd = m_EmitterMap.end();

	for (it; it != itEnd; ++it)
	{
		std::string nameEI = name + "_" + it->second->GetName();
		inst->AddEmitterInstance(nameEI, it->second->GetParticleEmitterInstance(nameEI));
	}

	m_SystemInstance[name] = inst;

	return inst;
}

bool CParticleEmitterSystem::AddParticleEmitter( const std::string& name, CParticleEmitter* emitter )
{
	assert(emitter);

	if(emitter == NULL)
	{
		return false;
	}

	EmitterMapIt it = m_EmitterMap.find(name);

	if(it != m_EmitterMap.end())
	{
		return false;
	}

	m_EmitterMap[name] = emitter;

	return true;
}

