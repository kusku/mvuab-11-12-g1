#include "EnableZWriteSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CEnableZWriteSceneRendererCommand::CEnableZWriteSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CEnableZWriteSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
}
