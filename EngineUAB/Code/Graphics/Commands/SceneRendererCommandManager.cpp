#include "SceneRendererCommandManager.h"
#include "RenderManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSceneRendererCommandManager::CSceneRendererCommandManager()
{
}

CSceneRendererCommandManager::~CSceneRendererCommandManager()
{
	CleanUp();
}

void CSceneRendererCommandManager::CleanUp()
{
	m_SceneRendererCommands.Destroy();
}

void CSceneRendererCommandManager::Load(const std::string &FileName)
{
}

void CSceneRendererCommandManager::Execute(CRenderManager &RM)
{
}

std::string CSceneRendererCommandManager::GetNextName()
{
	return "";
}