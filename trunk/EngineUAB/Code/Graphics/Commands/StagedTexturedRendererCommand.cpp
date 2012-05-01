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

CStagedTexturedRendererCommand::CStagedTexturedRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
}

void CStagedTexturedRendererCommand::ActivateTextures()
{
	for(uint32 i = 0; i < m_StageTexturesVector.size(); ++i)
	{
		m_StageTexturesVector[i].Activate();
	}
}

void CStagedTexturedRendererCommand::DeactivateTextures()
{
	for(uint32 i = 0; i < m_StageTexturesVector.size(); ++i)
	{
		m_StageTexturesVector[i].Deactivate();
	}
}

void CStagedTexturedRendererCommand::ActivateTexturesRenderTarget()
{
	for(uint32 i = 0; i < m_StageTexturesVector.size(); ++i)
	{
		m_StageTexturesVector[i].ActivateRenderTarget();
	}
}

void CStagedTexturedRendererCommand::DeactivateTexturesRenderTarget()
{
	for(uint32 i = 0; i < m_StageTexturesVector.size(); ++i)
	{
		m_StageTexturesVector[i].DeactivateRenderTarget();
	}
}

void CStagedTexturedRendererCommand::CaptureFrameBuffer()
{
	for(uint32 i = 0; i < m_StageTexturesVector.size(); ++i)
	{
		m_StageTexturesVector[i].CaptureFrameBuffer();
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

void CStagedTexturedRendererCommand::CStageTexture::Deactivate()
{
	assert(m_pTexture);

	m_pTexture->Deactivate(m_StageId);
}

void CStagedTexturedRendererCommand::CStageTexture::ActivateRenderTarget()
{
	assert(m_pTexture);

	m_pTexture->SetAsRenderTarget(m_StageId);
}

void CStagedTexturedRendererCommand::CStageTexture::DeactivateRenderTarget()
{
	assert(m_pTexture);

	m_pTexture->UnsetAsRenderTarget(m_StageId);
}

void CStagedTexturedRendererCommand::CStageTexture::CaptureFrameBuffer()
{
	m_pTexture->CaptureFrameBuffer( m_StageId );
}