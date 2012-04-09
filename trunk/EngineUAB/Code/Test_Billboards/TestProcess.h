#pragma once

#ifndef _TESTPROCESS_H
#define _TESTPROCESS_H

class CCamera;
class CThPSCamera;
class CActionToInput;

#include "Player.h"
#include "Process.h"
#include "Math\Vector2.h"
#include "Particles\ParticleEmitter.h"
#include <vector>

class CTestProcess : public CProcess
{
public:
	CTestProcess();
	virtual ~CTestProcess();

	void Init ();
	void Update (float elapsedTime);
	void Render (CRenderManager &RM);

protected:
	void UpdateInputs( float elapsedTime );
	void UpdateDebugInputs( float elapsedTime, CActionToInput &action2Input );

	Vect2i pos;
	Vect2i screen;

	CPlayer m_Player;
	CThPSCamera *m_pThPSCamera;

	CParticleEmitter m_ParticleEmitter;
};

#endif
