#pragma once

#ifndef _TESTPROCESS_H
#define _TESTPROCESS_H

class CCamera;
class CThPSCamera;
class CActionToInput;

#include "Player.h"
#include "EngineProcess.h"
#include "Math\Vector2.h"
#include <vector>

class CTestGUIProcess : public CEngineProcess
{
public:
						CTestGUIProcess		( void );
	virtual				~CTestGUIProcess	( void );

	bool				Init				( void );
	void				Done				( void );
	void				Update				( float _ElapsedTime );
	void				Render				( CRenderManager &_RM );

protected:
	virtual void		Release				( void );

	void				UpdateInputs		( float _ElapsedTime );
	void				UpdateDebugInputs	( float _ElapsedTime, CActionToInput &_Action2Input );

	Vect2i		pos;
	Vect2i		screen;

	CPlayer		m_Player;
	CThPSCamera *m_pThPSCamera;
};

#endif
