#include "TurretEnemy.h"

CTurretEnemy::CTurretEnemy()
{
}


CTurretEnemy::~CTurretEnemy()
{
}

void CTurretEnemy::Init(const std::string &pos, float yaw, float pitch)
{
	m_Pos = pos;
	m_Yaw = yaw;
	m_Pitch = pitch;
}

void CTurretEnemy::Update(float ElapsedTime)
{
	printf("\tUpdating Turret Enemy.\n");
}

void CTurretEnemy::Render()
{
	printf("\tRendering Turret Enemy.\n");
}
