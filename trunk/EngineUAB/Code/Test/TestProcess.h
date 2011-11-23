#pragma once

#ifndef _TESTPROCESS_H
#define _TESTPROCESS_H

class CPlayer;
class CCollisionManager;
class CItemManager;

#include "Process.h"
#include "defines.h"
#include "Math\Vector2.h"

class CTestProcess : public CProcess
{
protected:
	CPlayer				*m_Player;
	CCollisionManager	*m_CollisionManager;
	CItemManager		*m_ItemManager;

	float	m_Time;
	float	m_TimeToStart;
		
	STATES m_State;

	int		m_NumItems;
	int		m_NumBonus;
	
	Vect2i	m_Screen;
	void	ResetValues		();

public:
	CTestProcess();
	virtual ~CTestProcess();

	void Init ();
	void Update (float elapsedTime);
	void Render (CRenderManager *RM);
};

#endif
