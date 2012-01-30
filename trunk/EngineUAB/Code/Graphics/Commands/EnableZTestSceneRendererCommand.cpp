#include "EnableZTestSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CEnableZTestSceneRendererCommand::CEnableZTestSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CEnableZTestSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZENABLE, TRUE ); 
	RM.GetDevice()->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
}
