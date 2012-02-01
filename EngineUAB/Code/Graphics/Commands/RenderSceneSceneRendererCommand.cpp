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
{
	//<render_scene layer="solid" active="true"/>
	m_Layer = Node.GetPszProperty("layer", "");

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
		CRenderableObjectsManager *l_ROManager = CORE->GetRenderableObjectsLayersManager()->GetResource(m_Layer);
		l_ROManager->Render(&RM);
	}
}
