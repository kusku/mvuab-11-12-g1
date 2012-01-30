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
	uint16 l_Count = Node.GetNumChildren();
	for(uint16 i=0; i<l_Count; ++i)
	{
		std::string l_Type = Node(i).GetName();
		if( l_Type == "render_scene" )
		{
			//<render_scene renderable_object_manager="solid" active="true"/>
			std::string layer = Node(i).GetPszProperty("renderable_object_manager", "");
			m_pRenderableObjectsManager = CORE->GetRenderableObjectsLayersManager()->GetResource(layer);
		}
	}
}

CRenderSceneSceneRendererCommand::~CRenderSceneSceneRendererCommand()
{
}

void CRenderSceneSceneRendererCommand::Execute(CRenderManager &RM)
{
}
