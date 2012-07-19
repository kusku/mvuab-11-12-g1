#include <assert.h>

#include "UnsetDepthStencilSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Commands\SetDepthStencilSceneRendererCommand.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CUnsetDepthStencilSceneRendererCommand::CUnsetDepthStencilSceneRendererCommand(CSetDepthStencilSceneRendererCommand *setDepthStencilSceneRendererCommand, CXMLTreeNode &Node)
	: CSceneRendererCommand(Node)
	, m_SetDepthStencilSceneRendererCommand(setDepthStencilSceneRendererCommand)
{
}

void CUnsetDepthStencilSceneRendererCommand::Execute(CRenderManager &RM)
{
	assert(m_SetDepthStencilSceneRendererCommand);

	m_SetDepthStencilSceneRendererCommand->UnsetDepthStencil();
}
