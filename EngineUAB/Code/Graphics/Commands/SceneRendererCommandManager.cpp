#include "SceneRendererCommandManager.h"
#include "RenderManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSceneRendererCommandManager::CSceneRendererCommandManager()
	: m_Filename("")
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
	m_Filename = FileName;
	Reload();
}

void CSceneRendererCommandManager::Reload()
{
}

void CSceneRendererCommandManager::Execute(CRenderManager &RM)
{
}

std::string CSceneRendererCommandManager::GetNextName()
{
	return "";
}