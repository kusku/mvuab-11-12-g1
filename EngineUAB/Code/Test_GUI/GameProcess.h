#ifndef _GAME_PROCESS_H_
#define _GAME_PROCESS_H_

class CRenderManager;
class CThPSCamera;

#include "EngineProcess.h"
#include "Player.h"

class CGameProcess : public CEngineProcess
{
public:
	CGameProcess(HWND hWnd);
	~CGameProcess();

	bool	Init	();

	void				Update				( float _ElapsedTime );
	void				Render				( CRenderManager &_RM );

private:
	CPlayer		m_Player;
	CThPSCamera *m_pThPSCamera;

	HWND		m_hWnd;
};

#endif