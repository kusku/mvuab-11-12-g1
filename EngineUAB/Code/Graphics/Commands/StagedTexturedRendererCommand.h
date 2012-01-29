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
	class CKGStageTexture
	{
	public:
		int				m_StageId;
		CTexture		*m_pTexture;

		CKGStageTexture()
		{
		}

		CKGStageTexture(int StageId, CTexture *Texture)
			: m_StageId(StageId)
			, m_pTexture(Texture)
		{
		}

		void Activate();
	};

	std::vector<CKGStageTexture>			m_StageTexturesVector;

public:
	CStagedTexturedRendererCommand();
	CStagedTexturedRendererCommand(CXMLTreeNode &Node);
	virtual ~CStagedTexturedRendererCommand();

	void	ActivateTextures	();
	void	AddStageTexture		(int StageId, CTexture *Texture);

	virtual void	Execute		( CRenderManager &RM ) = 0;


};

#endif