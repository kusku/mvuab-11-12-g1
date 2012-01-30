#include "DisableZTestSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CDisableZTestSceneRendererCommand::CDisableZTestSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CDisableZTestSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );	
}
