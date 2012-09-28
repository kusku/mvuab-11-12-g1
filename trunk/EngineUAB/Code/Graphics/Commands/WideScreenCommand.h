#pragma once

#ifndef _WIDE_SCREEN_COMMAND_H
#define _WIDE_SCREEN_COMMAND_H

#include "SceneRendererCommand.h"
#include <d3dx9.h>

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
class CTexture;
//-------------------------//

class CWideScreenCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
					CWideScreenCommand	( CXMLTreeNode &_Node );
	virtual			~CWideScreenCommand	( void );

	//----Main Methods ---------------------------------------------------
	virtual void	Execute						( CRenderManager &_RM );

	//----Members --------------------------------------------------------
private:
	CTexture* m_DiffuseRT;

	D3DXVECTOR3 m_DiffusePosition;
	D3DXVECTOR3 m_DiffuseCenter;

	ID3DXSprite*				m_SpriteBatch;
};

#endif