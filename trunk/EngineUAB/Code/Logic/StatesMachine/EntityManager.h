#pragma once

#ifndef __CLASS_ENTITY_MANAGER_H__
#define __CLASS_ENTITY_MANAGER_H__

#define ENTITYMANAGER	CEntityManager::GetSingletonPtr()

#include <map>

#include "Utils\Singleton.h"
#include "Scripting\ScriptManager.h"
#include "base.h"

//--- Foward Declaration ---//
class CBaseGameEntity;
//--------------------------//

class CEntityManager : public CSingleton<CEntityManager>
{
	// ------------- Types ------------------------------------
private:
	typedef std::map<int, CBaseGameEntity*> TEntityMap;

public:
	// ------------- Constructors i Destructors --------------
							CEntityManager	( void )		{};
							~CEntityManager	( void )		{};

	//static	CEntityManager*	Instance();

	// ------------- Funcions Principals ---------------------
	// Almacena un puntero a la entidad en el std::vector m_Entities en la posición inbdicada por el ID de la entidad. Permite un acceso rápido
	void					RegisterEntity	( CBaseGameEntity* _pNewEntity );
	void					RemoveEntities	( void );

	static void				RegisterMethods	( void );

	// ------------- Propietats ( Get / Set ) ----------------
	// Devuelve el puntero de la entidad a partir del ID dado 
	CBaseGameEntity*		GetEntityFromID		( int _Id ) const;		

	// Elimina la entidad de la lista
	void					RemoveEntity	( CBaseGameEntity* _pEntity );

	// ------------- Operators -------------------------------
private:
	CEntityManager ( const CEntityManager& );
	CEntityManager& operator=( const CEntityManager& );

	// ------------- Membres ----------------------------------
private:
	TEntityMap				m_EntityMap;
};

// Definimos el acceso a la instancia del EntityManager
//#define EntityMgr EntityManager::Instance()

#endif __CLASS_ENTITY_MANAGER_H__