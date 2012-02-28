#pragma once

#ifndef _TESTPROCESS_H
#define _TESTPROCESS_H

class CCamera;
class CThPSCamera;

#include "Player.h"
#include "Process.h"
#include "Math\Vector2.h"

class CTestProcess : public CProcess
{
public:
	CTestProcess();
	virtual ~CTestProcess();

	void Init ();
	void Update (float elapsedTime);
	void Render (CRenderManager *RM);

protected:
	void UpdateInputs( float elapsedTime );

	Vect2i pos;
	Vect2i screen;

	CPlayer m_Player;
	CThPSCamera *m_pThPSCamera;
};

#endif
