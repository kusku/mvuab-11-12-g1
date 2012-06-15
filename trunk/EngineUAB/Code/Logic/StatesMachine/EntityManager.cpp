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

void CEntityManager::RemoveEntities( void )
{
	
	/*TEntityMap::iterator l_It	= m_EntityMap.begin();
	TEntityMap::iterator l_End	= m_EntityMap.end();
	for ( ; l_It != l_End; ++l_It )
	{
		m_EntityMap.erase (l_It);
	}*/
	m_EntityMap.clear();
}

void CEntityManager::RegisterMethods( void )
{
	lua_State* l_pLua = SCRIPT->GetLuaState();

	module( l_pLua)
		[
			class_<CEntityManager> ("CEntityManager")
				.def("get_entity_from_id",&CEntityManager::GetEntityFromID)
				.def("remove_entities", &CEntityManager::RemoveEntities)
		];
}
