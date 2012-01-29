#include "UnsetRenderTargetSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SetRenderTargetRendererCommand, 
	CXMLTreeNode &Node)
	: m_pSetRenderTargetRendererCommand(SetRenderTargetRendererCommand)
{
}

void CUnsetRenderTargetSceneRendererCommand::Execute(CRenderManager &RM)
{
	
}
