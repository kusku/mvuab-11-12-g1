#include "CollisionManager.h"


CCollisionManager::CCollisionManager()
	: m_Boards(0, 0)
{
}


CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Init(const Vect2i &screen)
{
	m_Boards = screen;
}

bool CCollisionManager::TestCollisionWithScreen(const Vect2f &player)
{
	if( player.x < 0.0f )
	{
		return true;
	}

	if( player.x > static_cast<float>(m_Boards.x) )
	{
		return true;
	}

	if( player.y < 0.0f)
	{
		return true;
	}

	if( player.y > static_cast<float>(m_Boards.y) )
	{
		return true;
	}
	return false;
}
