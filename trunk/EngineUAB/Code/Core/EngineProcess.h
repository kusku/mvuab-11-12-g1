#pragma once

#ifndef _PROCESS_H
#define _PROCESS_H

class CRenderManager;
class CCamera;

class CEngineProcess
{
protected:
	CCamera*	m_Camera;

public:
	CEngineProcess();
	virtual ~CEngineProcess();

	virtual void Init();
	virtual void Update(float ElapsedTime);
	virtual void Render( CRenderManager &RM);

	virtual CCamera* GetCamera () const { return m_Camera; }
};

#endif