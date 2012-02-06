#pragma once

#ifndef _STAGED_TEXTURED_RENDERER_COMMAND_H
#define _STAGED_TEXTURED_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include <vector>

class CRenderManager;
class CXMLTreeNode;
class CTexture;

class CStagedTexturedRendererCommand : public CSceneRendererCommand
{
protected:	
	class CStageTexture
	{
	public:
		int				m_StageId;
		CTexture		*m_pTexture;

		CStageTexture()
			: m_StageId(-1)
			, m_pTexture(NULL)
		{
		}

		CStageTexture(int StageId, CTexture *Texture)
			: m_StageId(StageId)
			, m_pTexture(Texture)
		{
		}

		void Activate();
		void Deactivate();
		void ActivateRenderTarget();
		void DeactivateRenderTarget();
	};

	std::vector<CStageTexture>			m_StageTexturesVector;

public:
	CStagedTexturedRendererCommand();
	CStagedTexturedRendererCommand(CXMLTreeNode &Node);
	virtual ~CStagedTexturedRendererCommand();

	void	ActivateTextures				();
	void	DeactivateTextures				();
	void	ActivateTexturesRenderTarget	();
	void	DeactivateTexturesRenderTarget	();
	void	AddStageTexture					(int StageId, CTexture *Texture);

	virtual void	Execute					( CRenderManager &RM ) = 0;


};

#endif