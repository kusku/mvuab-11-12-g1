#include "RenderableObjectsLayersManager.h"
#include "RenderableObjectsManager.h"
#include "RenderManager.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager()
	: m_pDefaultRenderableObjectManager(NULL)
{
}

CRenderableObjectsLayersManager::~CRenderableObjectsLayersManager()
{
	Destroy();
}

void CRenderableObjectsLayersManager::Destroy()
{
	CTemplatedVectorMapManager::Destroy();
	CHECKED_DELETE(m_pDefaultRenderableObjectManager);
}

void CRenderableObjectsLayersManager::Load(const std::string &FileName)
{
	m_FileName = FileName;
	LoadFile();
}

void CRenderableObjectsLayersManager::Reload()
{
	Destroy();
	LoadFile();
}

void CRenderableObjectsLayersManager::LoadFile()
{
}

void CRenderableObjectsLayersManager::Update(float ElapsedTime)
{
	TVectorResources l_VectorResources = GetResourcesVector();

	TVectorResources::iterator l_It = l_VectorResources.begin();
	TVectorResources::iterator l_End = l_VectorResources.end();
	for(; l_It != l_End; ++l_It)
	{
		(*l_It)->Update(ElapsedTime);
	}
}

void CRenderableObjectsLayersManager::Render(CRenderManager *RM)
{
	TVectorResources l_VectorResources = GetResourcesVector();

	TVectorResources::iterator l_It = l_VectorResources.begin();
	TVectorResources::iterator l_End = l_VectorResources.end();
	for(; l_It != l_End; ++l_It)
	{
		//TODO: Mirar lo de la technique
		(*l_It)->Render(RM, NULL);
	}
}

void CRenderableObjectsLayersManager::Render(CRenderManager *RM, const std::string &LayerName)
{
}

CRenderableObjectsManager* CRenderableObjectsLayersManager::GetRenderableObjectManager(CXMLTreeNode &Node)
{
	return NULL;
}
