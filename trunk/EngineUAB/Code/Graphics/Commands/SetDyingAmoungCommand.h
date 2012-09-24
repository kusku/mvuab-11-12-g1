#pragma once

#ifndef _SET_DYING_AMOUNT_COMMAND_H
#define _SET_DYING_AMOUNT_COMMAND_H

#include "SceneRendererCommand.h"
#include <d3dx9.h>

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
class CEffect;

//-------------------------//

class CSetDyingAmountCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
					CSetDyingAmountCommand	( CXMLTreeNode &_Node );
	virtual			~CSetDyingAmountCommand	( void );

	//----Main Methods ---------------------------------------------------
	virtual void	Execute						( CRenderManager &_RM );

	//----Members --------------------------------------------------------
private:
	D3DXHANDLE m_Amount;
	CEffect* m_Effect;
};

#endif