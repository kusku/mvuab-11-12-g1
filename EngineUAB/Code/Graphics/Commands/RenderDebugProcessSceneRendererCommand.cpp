#include "RenderDebugProcessSceneRendererCommand.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "Base.h"
#include "EngineProcess.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderDebugProcessSceneRendererCommand::CRenderDebugProcessSceneRendererCommand(CXMLTreeNode &Node)
{
	m_Active = Node.GetBoolProperty("active", false);
}

void CRenderDebugProcessSceneRendererCommand ::Execute(CRenderManager &RM)
{	
	if( GetActive() )
	{
		CORE->GetProcess()->Render(RM);
	}
}
