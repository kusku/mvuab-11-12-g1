#include "SetRenderTargetSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Textures\Texture.h"

CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CSetRenderTargetSceneRendererCommand::Execute(CRenderManager &RM)
{
	for(uint32 i = 0; i < m_StageTexturesVector.size(); ++i)
	{
		/*RM.GetDevice()->SetRenderTarget(m_StageTexturesVector[i].m_StageId,
										m_StageTexturesVector[i].m_pTexture->GetDXTexture());*/
	}
}
