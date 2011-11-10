#include "PatrolEnemy.h"


CPatrolEnemy::CPatrolEnemy()
{
}


CPatrolEnemy::~CPatrolEnemy()
{
}

void CPatrolEnemy::Init(const std::string &pos, float yaw, float pitch)
{
	m_Pos = pos;
	m_Yaw = yaw;
	m_Pitch = pitch;
}

void CPatrolEnemy::Update(float ElapsedTime)
{
	printf("\tUpdating Patrol Enemy.\n");
}

void CPatrolEnemy::Render()
{
	printf("\tRendering Patrol Enemy.\n");
}
