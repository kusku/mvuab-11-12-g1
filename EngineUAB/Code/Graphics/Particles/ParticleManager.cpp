#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
	CleanUp();
}

void CParticleManager::CleanUp()
{
	Destroy();
}

bool CParticleManager::Load(const std::string &Filename)
{
	m_Filename = Filename;
	return LoadFile();
}

bool CParticleManager::Reload()
{
	CleanUp();
	return LoadFile();
}

bool CParticleManager::LoadFile()
{
	return true;
}

void CParticleManager::Update(float elapsedTime, CCamera &camera)
{
	TVectorResources l_ParticlesEmitter = GetResourcesVector();
	TVectorResources::iterator l_It = l_ParticlesEmitter.begin();
	TVectorResources::iterator l_End = l_ParticlesEmitter.end();

	for(; l_It != l_End; ++l_It)
	{
		(*l_It)->Update(elapsedTime, camera);
	}

}

void CParticleManager::Render(CRenderManager &RM)
{
	TVectorResources l_ParticlesEmitter = GetResourcesVector();
	TVectorResources::iterator l_It = l_ParticlesEmitter.begin();
	TVectorResources::iterator l_End = l_ParticlesEmitter.end();

	for(; l_It != l_End; ++l_It)
	{
		(*l_It)->Render(RM);
	}
}