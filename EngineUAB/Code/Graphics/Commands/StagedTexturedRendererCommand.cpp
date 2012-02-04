#include <assert.h>

#include "StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Utils\Types.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CStagedTexturedRendererCommand::CStagedTexturedRendererCommand()
{
}

CStagedTexturedRendererCommand::CStagedTexturedRendererCommand(CXMLTreeNode &Node)
{
}

CStagedTexturedRendererCommand::~CStagedTexturedRendererCommand()
{
}

void CStagedTexturedRendererCommand::ActivateTextures()
{
	for(uint32 i = 0; i < m_StageTexturesVector.size(); ++i)
	{
		m_StageTexturesVector[i].Activate();
	}
}

void CStagedTexturedRendererCommand::AddStageTexture(int StageId, CTexture *Texture)
{
	CStageTexture st(StageId, Texture);

	m_StageTexturesVector.push_back(st);
}

void CStagedTexturedRendererCommand::CStageTexture::Activate()
{
	assert(m_pTexture);

	m_pTexture->Activate(m_StageId);
}