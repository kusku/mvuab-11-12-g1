#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class CEnemy
{
protected:
	float m_Yaw, m_Pitch;
	std::string m_Pos;

public:
	CEnemy(void)
	{
	}
	virtual ~CEnemy(void)
	{
	}

	virtual void Init(const std::string &pos, float yaw, float pitch) = 0;
	virtual void Update(float ElapsedTime) = 0;
	virtual void Render() = 0;
};

#endif

