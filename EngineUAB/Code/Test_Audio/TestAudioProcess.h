#pragma once

#ifndef _TESTPROCESS_H
#define _TESTPROCESS_H

class CCamera;
class CThPSCamera;
class CActionToInput;
class CWwiseSoundManager;

#include "Player.h"
#include "EngineProcess.h"
#include "Math\Vector2.h"
#include <vector>

class CTestAudioProcess : public CEngineProcess
{
public:
						CTestAudioProcess		( void );
	virtual				~CTestAudioProcess	( void );

	bool				Init				( void );
	void				Done				( void );
	void				Update				( float _ElapsedTime );
	void				Render				( CRenderManager &_RM );

protected:
	virtual void		Release				( void );
	void				UpdateInputs		();

	Vect2i		pos;
	Vect2i		screen;

	CPlayer		m_Player;
	CThPSCamera *m_pThPSCamera;

	CWwiseSoundManager *m_pWwiseSoundManager;
};

#endif
