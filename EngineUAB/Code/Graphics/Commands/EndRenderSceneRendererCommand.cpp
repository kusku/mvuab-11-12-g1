#include "EndRenderSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEndSceneRendererCommand::CEndSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CEndSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->EndScene();
}
