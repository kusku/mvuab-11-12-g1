#include "RenderDebugLightsSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Lights\LightManager.h"
#include "Utils\Active.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &Node)
{
	bool active = Node.GetBoolProperty("active", false);
	SetActive(active);
}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RM)
{
	if( GetActive() )
	{
		Mat44f mat;
		mat.SetIdentity();
		CORE->GetRenderManager()->SetTransform(mat);
		CORE->GetRenderManager()->DrawAxis(1.0f);

		CORE->GetLightManager()->Render(&RM);
	}
}
