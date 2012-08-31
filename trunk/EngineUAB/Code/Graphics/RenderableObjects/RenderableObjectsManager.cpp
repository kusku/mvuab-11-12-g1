#include "RenderableObjectsManager.h"
#include "RenderManager.h"
#include "StaticMesh\InstanceMesh.h"
#include "AnimatedModel\AnimatedModelManager.h"
#include "AnimatedModel\AnimatedInstanceModel.h"
#include "Effects\EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Core.h"
#include "Effects\EffectTechnique.h"
#include "Logger\Logger.h"
#include "StaticMesh\InstanceMeshHW.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderableObjectsManager::CRenderableObjectsManager()
	: m_FileName("")
{
}


CRenderableObjectsManager::~CRenderableObjectsManager()
{
	CleanUp();
}

void CRenderableObjectsManager::Update(float elapsedTime)
{
	std::vector<CRenderableObject*>::iterator l_It = m_RenderableObjects.begin();
	std::vector<CRenderableObject*>::iterator l_End = m_RenderableObjects.end();

	for(; l_It != l_End; ++l_It)
	{
		if( (*l_It)->GetVisible() )
		{
			(*l_It)->Update(elapsedTime);
		}
	}
}

//void CRenderableObjectsManager::Render(CRenderManager *RM)
//{
//	std::vector<CRenderableObject*>::iterator l_It = m_RenderableObjects.begin();
//	std::vector<CRenderableObject*>::iterator l_End = m_RenderableObjects.end();
//
//	std::string l_TechniqueName = "";
//
//	for(; l_It != l_End; ++l_It)
//	{
//		if( (*l_It)->GetVisible() )
//		{
//			/*l_TechniqueName = (*l_It)->GetTechniqueName();
//			if( l_TechniqueName != "" )
//			{
//				(*l_It)->Render(RM, CORE->GetEffectManager()->GetEffectTechnique( l_TechniqueName ));
//			}
//			else
//			{
//				(*l_It)->Render(RM, NULL);
//			}*/
//			(*l_It)->Render(RM);
//		}
//	}
//}

void CRenderableObjectsManager::Render(CRenderManager *RM)
{
	std::vector<CRenderableObject*>::iterator l_It = m_RenderableObjects.begin();
	std::vector<CRenderableObject*>::iterator l_End = m_RenderableObjects.end();

	for(; l_It != l_End; ++l_It)
	{
		if( (*l_It)->GetVisible() )
		{
			(*l_It)->Render(RM);
		}
	}

	InstanceMeshHWMapIt it = m_InstanceMeshHWMap.begin();
	InstanceMeshHWMapIt itEnd = m_InstanceMeshHWMap.end();

	for(; it != itEnd; ++it)
	{
		it->second->Render(RM);
	}
}

CRenderableObject* CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position)
{
	CInstanceMesh *l_InstanceMesh = new CInstanceMesh(InstanceName, CoreMeshName);
	l_InstanceMesh->SetPosition( Position );

	AddResource( InstanceName, static_cast<CRenderableObject*>(l_InstanceMesh) );

	return static_cast<CRenderableObject*>(l_InstanceMesh);
}

CRenderableObject* CRenderableObjectsManager::AddAnimatedMeshInstance (const std::string &Name, const Vect3f &Position)
{
	CAnimatedInstanceModel* l_AnimatedInstanceModel = new CAnimatedInstanceModel();
	l_AnimatedInstanceModel->SetPosition( Position );
	
	AddResource( Name, static_cast<CRenderableObject*>(l_AnimatedInstanceModel) );

	return static_cast<CRenderableObject*>( l_AnimatedInstanceModel );
}

CRenderableObject* CRenderableObjectsManager::AddMeshInstance(CXMLTreeNode &Node)
{
	CRenderableObject *l_InstanceMesh = new CInstanceMesh(Node);

	AddResource( l_InstanceMesh->GetName(), l_InstanceMesh );

	return l_InstanceMesh;
}

bool CRenderableObjectsManager::AddMeshInstanceHw(CXMLTreeNode &Node)
{
	std::string core = Node.GetPszProperty("core", "");

	if(core.compare("") == 0)
	{
		return false;
	}

	CInstanceMeshHW* hwMesh = NULL;
	InstanceMeshHWMapIt it = m_InstanceMeshHWMap.find(core);

	if(it == m_InstanceMeshHWMap.end())
	{
		hwMesh = new CInstanceMeshHW(core);

		m_InstanceMeshHWMap[core] = hwMesh;
	}
	else
	{
		hwMesh = it->second;
	}

	hwMesh->AddHWInstance(Node);

	return true;
}

CRenderableObject* CRenderableObjectsManager::AddAnimatedMeshInstance(CXMLTreeNode &Node)
{
	std::string l_Core = Node.GetPszProperty("core", "");
	CAnimatedInstanceModel* l_AnimatedInstanceModel = CORE->GetAnimatedModelManager()->GetInstance(l_Core);
	l_AnimatedInstanceModel->ReadDataXML(Node);

	// Jordi 31/08/2012 -- No se miraba ningun atributo del nodo
	l_AnimatedInstanceModel->SetVisible(Node.GetBoolProperty("visible", "false"));

	AddResource( l_AnimatedInstanceModel->GetName(), static_cast<CRenderableObject*>(l_AnimatedInstanceModel) );

	return static_cast<CRenderableObject*>(l_AnimatedInstanceModel);
}

bool CRenderableObjectsManager::AddResource(const std::string &Name, CRenderableObject *RenderableObject)
{
	if( CMapManager<CRenderableObject>::AddResource(Name, RenderableObject) )
	{
		m_RenderableObjects.push_back(RenderableObject);
		return true;
	}
	return false;
}

CRenderableObject* CRenderableObjectsManager::GetInstance(const std::string &Name)
{
	return GetResource(Name);
}

CObject3D* CRenderableObjectsManager::GetInstanceHW( const std::string &Name )
{
	InstanceMeshHWMapIt it = m_InstanceMeshHWMap.begin();
	InstanceMeshHWMapIt itEnd = m_InstanceMeshHWMap.end();

	for(; it != itEnd; ++it)
	{
		CInstanceMeshHW* instanceHW = it->second;

		CObject3D* obj = instanceHW->GetInstance(Name);

		if(obj != NULL)
		{
			return obj;
		}
	}

	return NULL;
}

void CRenderableObjectsManager::CleanUp()
{
	Destroy();
	m_RenderableObjects.clear();

	InstanceMeshHWMapIt it = m_InstanceMeshHWMap.begin();
	InstanceMeshHWMapIt itEnd = m_InstanceMeshHWMap.end();

	for(; it != itEnd; ++it)
	{
		CHECKED_DELETE(it->second);
	}

	m_InstanceMeshHWMap.clear();
}