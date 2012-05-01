#pragma once

#ifndef _STAGED_TEXTURED_RENDERER_COMMAND_H
#define _STAGED_TEXTURED_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include <vector>

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
class CTexture;
//-------------------------//

class CStagedTexturedRendererCommand : public CSceneRendererCommand
{
protected:	
	class CStageTexture
	{
	public:
		int				m_StageId;
		CTexture		*m_pTexture;

		CStageTexture( void )
			: m_StageId		( -1 )
			, m_pTexture	 (NULL )
		{
		}

		CStageTexture(int _StageId, CTexture *_Texture)
			: m_StageId		( _StageId )
			, m_pTexture	( _Texture )
		{
		}

		void	Activate				( void );
		void	Deactivate				( void );
		void	ActivateRenderTarget	( void );
		void	DeactivateRenderTarget	( void );
		void	CaptureFrameBuffer		( void );
	};

	std::vector<CStageTexture>			m_StageTexturesVector;

public:
	CStagedTexturedRendererCommand(CXMLTreeNode &Node);

	void			ActivateTextures				( void );
	void			DeactivateTextures				( void );
	void			ActivateTexturesRenderTarget	( void );
	void			DeactivateTexturesRenderTarget	( void );
	void			AddStageTexture					( int StageId, CTexture *Texture );
	void			CaptureFrameBuffer				( void );

	virtual void	Execute							( CRenderManager &RM ) = 0;


};

#endif