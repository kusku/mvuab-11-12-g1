#pragma once

#ifndef PATROLENEMY_H
#define PATROLENEMY_H

#include "Enemy.h"
#include <string>

class CPatrolEnemy : public CEnemy
{
protected:
public:
	CPatrolEnemy();
	~CPatrolEnemy();

	void Init(const std::string &pos, float yaw, float pitch);
	void Update(float ElapsedTime);
	void Render();
};

#endif

