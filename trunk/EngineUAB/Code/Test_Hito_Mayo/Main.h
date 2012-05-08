#pragma once

#include "TestProcess.h"

class CLogic;
class CPlayer;
class CScene;

class CMain : public CTestProcess
{
public:
	CMain(void);
	~CMain(void);


	bool			Init				( void );
	virtual void	Update				( float _ElapsedTime );
	virtual void	Render				( CRenderManager *_RM );

	void			UpdateInputs		( float _ElapsedTime );

	CLogic		*m_pLogic; 
	CPlayer		*m_pPlayer;
	CScene		*m_pScene;
};

