#include "RenderDebugLightsSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Lights\LightManager.h"
#include "Base.h"
#include "Core.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RM)
{
	CORE->GetLightManager()->Render(&RM);
}
