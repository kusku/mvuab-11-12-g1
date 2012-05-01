#include <cassert>

#include "EntityManager.h"
#include "BaseGameEntity.h"


//------------------------- GetEntityFromID -----------------------------------
//-----------------------------------------------------------------------------
CBaseGameEntity* CEntityManager::GetEntityFromID( int _Id ) const
{
  //find the entity
  TEntityMap::const_iterator ent = m_EntityMap.find(_Id);

  //assert that the entity is a member of the map
  assert ( ( ent !=  m_EntityMap.end()) && "EntityManager::GetEntityFromID-> invalid ID" );

  return ent->second;
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
