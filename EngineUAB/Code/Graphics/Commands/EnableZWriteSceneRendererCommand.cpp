#include "EnableZWriteSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CEnableZWriteSceneRendererCommand::CEnableZWriteSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CEnableZWriteSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
	RM.GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	RM.GetDevice()->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
}
