#include "EndRenderSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CEndSceneRendererCommand::CEndSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CEndSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->EndScene();

	// Present the backbuffer contents to the display
	RM.GetDevice()->Present( NULL, NULL, NULL, NULL );
}
