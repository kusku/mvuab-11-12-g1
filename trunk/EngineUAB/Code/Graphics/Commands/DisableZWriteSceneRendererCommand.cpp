#include "DisableZWriteSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CDisableZWriteSceneRendererCommand::CDisableZWriteSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CDisableZWriteSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE ); 
}
