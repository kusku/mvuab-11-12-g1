#pragma once

#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include <vector>
#include <string>
#include "Character.h"
#include "Scripting\ScriptManager.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\MapManager.h"
#include "XML\XMLTreeNode.h"

//--- Foward Declarations ---
class CRenderableObjectsManager;
class CRenderManager;
class CFontManager;
class CPropertiesManager;
class CAnimationsStatesManager;
class CPhysicUserData;
//---------------------------

class CCharactersManager : public CTemplatedVectorMapManager<CCharacter>
{
public:
	CCharactersManager	();
	~CCharactersManager	();

	bool			Initialize		( int _NumEnemies = 0 );
	bool			Load			( const std::string &_PropertyFileName, const std::string &_AnimatedStatesFileName );
	bool			Reload			();
	void			CleanUp			();
	void			Update			( float _ElapsedTime );
	void			Render			( CRenderManager *_RM, CFontManager *_FM );

	// ------------- Methods --------------------------------
	void			AddEnemy					( CCharacter *_pEnemy );	// Afegeix un enemic ja creat
	void			CreateEnemy					();							// Crea i afegeix un enemic ja creat
	void			CreateRandomEnemy			();							// Crea i afegeix un enemic aleatori
	bool			LoadXMLProperties			();							// Carga el XML de propiedades
	bool			LoadXMLAnimatedStates		();							// Carga el XML de estados

	// ------------- Propietats ( Get / Set ) ----------------
	TVectorResources	GetEnemiesVector		() const				{ return m_ResourcesVector; }
	TMapResources		GetEnemiesMap			() const				{ return m_ResourcesMap; }
	inline int			GetTotalEnemies			() const				{ return m_ResourcesVector.size(); }
	inline CCharacter*	GetPlayer				() const				{ return m_pPlayer; }
	inline CCharacter*	GetTargetEnemy			() const				{ return m_pTargetEnemy; }

	inline void			SetPlayer				(CCharacter *player)	{ m_pPlayer = player; }
	inline void			SetTargetEnemy			( CCharacter *enemy )	{ m_pTargetEnemy = enemy; }

	CPhysicUserData*	ShootPlayerRaycast		();
	CCharacter*			ExistEnemyUserData		( CPhysicUserData *_userData );
	CCharacter*			IsPlayerNearEnemy		( float distance );
private:
	bool	LoadXML							();							// Carga el XML de propiedades y estados
	bool	LoadDefaultCoreProperties		( const CXMLTreeNode &_Node );		// Carga las propiedades por defecto del XML
	bool	LoadPlayerProperties			( const CXMLTreeNode &_Node );		// Carga una nueva instancia del player dependiendo de las propiedades obtenidas
	bool	LoadEnemiesProperties			( const CXMLTreeNode &_Node );		// Carga una enemigos dependiendo de las propiedades obtenidas
	bool	LoadDefaultCoreAnimatedStates	( const CXMLTreeNode &_Node );		// Carga los estados por defecto del XML
	bool	LoadPlayerAnimationStates		( const CXMLTreeNode &_Node );		// Carga una nueva instancia del player y mete los estados obtenidos
	bool	LoadEnemiesAnimationStates		( const CXMLTreeNode &_Node );		// Carga una enemigos y mete los estados obtenidos

	Vect3f	RandomVector					( const Vect3f &_Vect1, const Vect3f &_Vect2 );

private:
	std::string					m_PropertiesFileName;
	std::string					m_AnimatedFileName;

	CPropertiesManager			*m_pPropertiesManager;
	CAnimationsStatesManager	*m_pAnimatedStatesManager;

	CCharacter					*m_pPlayer;
	CCharacter					*m_pTargetEnemy;
};

#endif