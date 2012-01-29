#include "ClearSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CClearSceneRendererCommand::Execute(CRenderManager &RM)
{
	//m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
}
