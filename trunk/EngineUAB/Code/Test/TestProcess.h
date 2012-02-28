#pragma once

#ifndef _TESTPROCESS_H
#define _TESTPROCESS_H

class CPlayer;
class CCollisionManager;
class CItemManager;

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
};

#endif
