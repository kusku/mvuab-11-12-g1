#include "RenderDebugPhysicsSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Utils\Active.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderDebugPhysicsSceneRendererCommand::CRenderDebugPhysicsSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
	bool active = Node.GetBoolProperty("active", false);
	SetActive(active);
}

void CRenderDebugPhysicsSceneRendererCommand::Execute(CRenderManager &RM)
{
	if( GetActive() )
	{
		CORE->GetPhysicsManager()->SetDebugRenderMode ( true );
		CORE->GetPhysicsManager()->DebugRender(&RM);
	}
	else
		CORE->GetPhysicsManager()->SetDebugRenderMode ( false );
}
