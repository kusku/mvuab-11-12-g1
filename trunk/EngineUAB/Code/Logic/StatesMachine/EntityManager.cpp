#include <cassert>

#include "EntityManager.h"
#include "BaseGameEntity.h"


//------------------------- GetEntityFromID -----------------------------------
//-----------------------------------------------------------------------------
CBaseGameEntity* CEntityManager::GetEntityFromID( int _Id ) const
{
  // Encuentra la entidada
  TEntityMap::const_iterator l_Ent = m_EntityMap.find(_Id);

  // Comprueba si la entidad es un miembro del mapa 
  assert ( ( l_Ent !=  m_EntityMap.end()) && "EntityManager::GetEntityFromID-> invalid ID" );

  return l_Ent->second;
}

//--------------------------- RemoveEntity ------------------------------------
//-----------------------------------------------------------------------------
void CEntityManager::RemoveEntity( CBaseGameEntity* _pEntity )
{    
	 m_EntityMap.erase( m_EntityMap.find( _pEntity->GetID() ) );
} 

//---------------------------- RegisterEntity ---------------------------------
//-----------------------------------------------------------------------------
void CEntityManager::RegisterEntity( CBaseGameEntity* _pNewEntity )
{
	m_EntityMap.insert( std::make_pair( _pNewEntity->GetID(), _pNewEntity ) );
}
