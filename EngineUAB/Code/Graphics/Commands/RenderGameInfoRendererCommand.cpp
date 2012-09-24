#include "RenderGameInfoRendererCommand.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "Base.h"
#include "EngineProcess.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderGameInfoRendererCommand::CRenderGameInfoRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
	m_Active = Node.GetBoolProperty("active", false);
}

void CRenderGameInfoRendererCommand ::Execute(CRenderManager &RM)
{	
	if( GetActive() )
	{
		CORE->GetProcess()->Render(RM);
	}
}