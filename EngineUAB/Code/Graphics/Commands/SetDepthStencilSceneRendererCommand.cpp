#include <assert.h>

#include "SetDepthStencilSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSetDepthStencilSceneRendererCommand::CSetDepthStencilSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand(Node)
{
	m_Name = Node.GetPszProperty("name", "");
	m_DepthStecilTexture = NULL;
	bool ret = false;

	if(CORE->GetTextureManager()->GetResource(m_Name) != NULL)
	{
		ret = CORE->GetTextureManager()->RemoveResource(m_Name);

		assert(ret);
	}

	m_DepthStecilTexture = new CTexture();

	m_DepthStecilTexture->SetName(m_Name);

	std::string format_type = Node.GetPszProperty("format_type", "D24S8");
	uint32 multiSamples = Node.GetIntProperty("multi_samples", 0);

	uint32 width = CORE->GetRenderManager()->GetScreenSize().x;
	uint32 height = CORE->GetRenderManager()->GetScreenSize().y;

	ret = m_DepthStecilTexture->CreateDepthStencil(width, height, m_DepthStecilTexture->GetFormatTypeFromString(format_type), static_cast<D3DMULTISAMPLE_TYPE>(multiSamples));

	if(!ret)
	{
		assert(0);

		CHECKED_DELETE(m_DepthStecilTexture);

		return;
	}

	ret = CORE->GetTextureManager()->AddResource(m_Name, m_DepthStecilTexture);

	assert(ret);
}

void CSetDepthStencilSceneRendererCommand::Execute(CRenderManager &RM)
{
	m_DepthStecilTexture->SetAsDepthStencil();
}

void CSetDepthStencilSceneRendererCommand::UnsetDepthStencil()
{
	m_DepthStecilTexture->UnsetAsDepthStencil();
}