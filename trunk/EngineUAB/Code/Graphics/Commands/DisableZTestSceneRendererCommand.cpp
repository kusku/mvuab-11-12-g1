#include "DisableZTestSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDisableZTestSceneRendererCommand::CDisableZTestSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
}

void CDisableZTestSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );	 
}
