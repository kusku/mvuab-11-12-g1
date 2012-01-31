#include "RenderSceneSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Core.h"
#include "Base.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderSceneSceneRendererCommand::CRenderSceneSceneRendererCommand(CXMLTreeNode &Node)
	: m_pRenderableObjectsManager(NULL)
{
	//<render_scene layer="solid" active="true"/>
	std::string layer = Node.GetPszProperty("layer", "");
	m_pRenderableObjectsManager = CORE->GetRenderableObjectsLayersManager()->GetResource(layer);

	bool active = Node.GetBoolProperty("active", false);
	SetActive(active);
}

CRenderSceneSceneRendererCommand::~CRenderSceneSceneRendererCommand()
{
}

void CRenderSceneSceneRendererCommand::Execute(CRenderManager &RM)
{
	if( GetActive() )
	{
		m_pRenderableObjectsManager->Render(&RM);
	}
}
