#pragma once

#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <vector>

class CEnemy;

class CEnemyManager
{
protected:
	std::vector<CEnemy *> m_EnemiesVector;

public:
	CEnemyManager(void);
	~CEnemyManager(void);

	void LoadXML(const std::string &filename);
	void Update(float ElapsedTime);
	void Render();
};

#endif
