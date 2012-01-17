#pragma once

#ifndef _PLAYER_H
#define _PLAYER_H

#include "Math\Vector2.h"
#include "..\defines.h"

class CPlayer
{
protected:
	Vect2f m_Position;
	Vect2f m_InitPosition;

	float m_Velocity;
	DIRECTION m_Direction;
	float m_TimeToAccelerate;

	int m_Lifes;
	bool m_bIsDead;

public:
	CPlayer();
	virtual ~CPlayer();

	void Init (const Vect2i &screen);
	void Update (float elapsedTime);
	void Render ();

	void	SubstractLife		()				{ --m_Lifes; m_bIsDead = true; }
	void	SetLife				(int lifes)		{ m_Lifes = lifes; }
	int		GetLife				() const		{ return m_Lifes; }
	float	GetSpeed			() const		{ return m_Velocity; }
	Vect2f	GetPosition			() const		{ return m_Position; }

	void	ResetPosition	();
};

#endif
