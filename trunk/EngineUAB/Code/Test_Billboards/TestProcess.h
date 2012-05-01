#pragma once

#ifndef _TESTPROCESS_H
#define _TESTPROCESS_H

#include <vector>

#include "Player.h"
#include "EngineProcess.h"
#include "Math\Vector2.h"

//---FOWARD DECLARATIONS---//
class CCamera;
class CThPSCamera;
class CActionToInput;
//-------------------------//


class CTestProcess : public CEngineProcess
{
public:
			CTestProcess		( void );
	virtual ~CTestProcess		( void );

	bool	Init				( void );
	void	Update				( float _ElapsedTime );
	void	Render				( CRenderManager &_RM );

protected:
	void	UpdateInputs		( float elapsedTime );
	void	UpdateDebugInputs	( float elapsedTime, CActionToInput &action2Input );

	Vect2i	pos;
	Vect2i	screen;

	CPlayer m_Player;
	CThPSCamera *m_pThPSCamera;
};

#endif
 