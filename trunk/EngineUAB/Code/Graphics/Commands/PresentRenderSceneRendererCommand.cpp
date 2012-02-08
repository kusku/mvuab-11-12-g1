#include "PresentRenderSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CPresentSceneRendererCommand::CPresentSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CPresentSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->Present( NULL, NULL, NULL, NULL );
}
