#pragma once

#ifndef _COLLISIONMANAGER_H
#define _COLLISIONMANAGER_H

#include "Math\Vector2.h"

class CCollisionManager
{
protected:
	Vect2i m_Boards;

public:
	CCollisionManager();
	virtual ~CCollisionManager();

	void Init ( const Vect2i &screen );

	bool TestCollisionWithScreen ( const Vect2f &player );
};

#endif
