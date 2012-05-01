#include <assert.h>

#include "UnsetRenderTargetSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Commands\SetRenderTargetSceneRendererCommand.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SetRenderTargetRendererCommand, CXMLTreeNode &Node)
	: CStagedTexturedRendererCommand ( Node )
	, m_pSetRenderTargetRendererCommand(SetRenderTargetRendererCommand)
{
}

void CUnsetRenderTargetSceneRendererCommand::Execute(CRenderManager &RM)
{
	assert(m_pSetRenderTargetRendererCommand);

	m_pSetRenderTargetRendererCommand->UnsetRenderTaget();
}
