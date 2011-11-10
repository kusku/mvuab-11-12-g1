#pragma once

#ifndef TURRETENEMY_H
#define TURRETENEMY_H

#include "Enemy.h"
#include <string>

class CTurretEnemy : public CEnemy
{
protected:
public:
	CTurretEnemy();
	~CTurretEnemy();

	void Init(const std::string &pos, float yaw, float pitch);
	void Update(float ElapsedTime);
	void Render();
};

#endif

