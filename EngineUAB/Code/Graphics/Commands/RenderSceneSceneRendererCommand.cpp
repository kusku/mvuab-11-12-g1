#include "RenderSceneSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Core.h"
#include "Base.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"

CRenderSceneSceneRendererCommand::CRenderSceneSceneRendererCommand(CXMLTreeNode &Node)
	: m_pRenderableObjectsManager(NULL)
{
	//<render_scene layer="solid" active="true"/>
	std::string layer = Node.GetPszProperty("layer", "");
	m_pRenderableObjectsManager = CORE->GetRenderableObjectsLayersManager()->GetResource(layer);
}

CRenderSceneSceneRendererCommand::~CRenderSceneSceneRendererCommand()
{
}

void CRenderSceneSceneRendererCommand::Execute(CRenderManager &RM)
{
	m_pRenderableObjectsManager->Render(&RM);
}
