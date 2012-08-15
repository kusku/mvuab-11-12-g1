#include "RenderDebugWayPointSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Utils\Active.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"
#include "Movement\WayPointManager.h"
#include "Triggers\TriggersManager.h"
#include "Rails\RailManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderDebugWayPointSceneRendererCommand::CRenderDebugWayPointSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
	m_bRenderWaypoints	= Node.GetBoolProperty("waypoints", false);
	m_bRenderTriggers	= Node.GetBoolProperty("triggers", false);
	m_bRenderRails		= Node.GetBoolProperty("rails", false);
}

void CRenderDebugWayPointSceneRendererCommand::Execute(CRenderManager &RM)
{
	if(m_bRenderWaypoints)
	{
		CORE->GetWayPointManager()->DebugRender();
	}

	if(m_bRenderTriggers)
	{
		CORE->GetTriggersManager()->Render(&RM);
	}

	if(m_bRenderRails)
	{
		CORE->GetRailManager()->Render(RM);
	}
}
