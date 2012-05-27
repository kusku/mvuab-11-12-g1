#include "BaseGameEntity.h"
#include <cassert>


int CBaseGameEntity::m_iNextValidID = 0;

void CBaseGameEntity::SetID ( int _Val )
{
	// Asegurem que el valor és igual o major que el proxim ID disponible
	assert ( ( _Val >= m_iNextValidID ) && "BaseGameEntity::SetID-> invalid ID" );

	m_ID = _Val;
	m_iNextValidID = m_ID + 1;
}

CBaseGameEntity::~CBaseGameEntity()
{
	if( m_iNextValidID > 0)
	{
		--m_iNextValidID;
	}
}
