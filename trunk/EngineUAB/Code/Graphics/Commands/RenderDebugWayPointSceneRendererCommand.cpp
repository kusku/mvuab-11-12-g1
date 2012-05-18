#include "RenderDebugWayPointSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Utils\Active.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"
#include "Movement\WayPointManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderDebugWayPointSceneRendererCommand::CRenderDebugWayPointSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
	bool active = Node.GetBoolProperty("active", false);
	SetActive(active);
}

void CRenderDebugWayPointSceneRendererCommand::Execute(CRenderManager &RM)
{
	if( GetActive() )
	{
		CORE->GetWayPointManager()->DebugRender();
	}
}
