#include "EnableZWriteSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEnableZWriteSceneRendererCommand::CEnableZWriteSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
}

void CEnableZWriteSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE ); 
}
