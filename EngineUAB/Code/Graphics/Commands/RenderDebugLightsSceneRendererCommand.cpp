#include "RenderDebugLightsSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Lights\LightManager.h"
#include "Utils\Active.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
	bool active = Node.GetBoolProperty("active", false);
	SetActive(active);
}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RM)
{
	if( GetActive() )
	{
		CORE->GetLightManager()->Render(&RM);
	}
}
