#include "RenderDebugPhysicsSceneRendererCommand.h"
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

CRenderDebugPhysicsSceneRendererCommand::CRenderDebugPhysicsSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
	// <render_debug_physics active="true" draw_fronts="true" draw_frustum="true" />
	bool active = Node.GetBoolProperty("active", false);
	SetActive(active);

	m_DrawFront				= Node.GetBoolProperty("draw_fronts",	   false);
	m_Drawfrustum			= Node.GetBoolProperty("draw_frustum",     false);
	m_DrawNames				= Node.GetBoolProperty("draw_names",       false);
	m_DrawRays				= Node.GetBoolProperty("draw_rays",        false);
	m_RenderPositions		= Node.GetBoolProperty("render_positions", false);
	m_DrawDistancesSpheres	= Node.GetBoolProperty("draw_distances",   false);
	
	CORE->GetPhysicsManager()->SetDrawFront( Node.GetBoolProperty("draw_fronts", false) );
	CORE->GetPhysicsManager()->SetDrawfrustum( Node.GetBoolProperty("draw_frustum", false) );
	CORE->GetPhysicsManager()->SetDrawNames( Node.GetBoolProperty("draw_names", false) );
	CORE->GetPhysicsManager()->SetDrawRays( Node.GetBoolProperty("draw_rays", false) );
	CORE->GetPhysicsManager()->SetRenderPositions( m_RenderPositions );
	CORE->GetPhysicsManager()->SetDrawDistancesSPheres( m_DrawDistancesSpheres );
}

void CRenderDebugPhysicsSceneRendererCommand::Execute(CRenderManager &RM)
{
	if( GetActive() )
	{
		CORE->GetWayPointManager()->DebugRender();
		CORE->GetPhysicsManager()->SetDebugRenderMode ( true );
		CORE->GetPhysicsManager()->DebugRender(&RM);
	}
	else
		CORE->GetPhysicsManager()->SetDebugRenderMode ( false );
}
