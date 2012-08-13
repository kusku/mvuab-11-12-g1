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
#include "Math\Vector3.h"

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
	// ------------- Constructors i Destructors --------------
						CCharactersManager					();
						~CCharactersManager					();

	// ------------- Main Methods ----------------------------
	bool				Initialize							( int _NumEnemies = 0 );
	bool				Load								( const std::string &_PropertyFileName, const std::string &_AnimatedStatesFileName );
	bool				Reload								();
	void				CleanUp								();
	void				CleanReloadScripts					();
	void				Update								( float _ElapsedTime );
	void				Render								( CRenderManager *_RM, CFontManager *_FM );

	// ------------- Methods --------------------------------
	void				AddEnemy							( CCharacter *_pEnemy );			// Afegeix un enemic ja creat
	void				CreateEnemy							();									// Crea i afegeix un enemic ja creat
	void				CreateRandomEnemy					();									// Crea i afegeix un enemic aleatori
	bool				LoadXMLProperties					();									// Carga el XML de propiedades
	bool				LoadXMLAnimatedStates				();									// Carga el XML de estados


	CCharacter*			GetCharacterById					( int _Id );
	CPhysicUserData*	ShootPlayerRaycast					();
	CCharacter*			ExistEnemyUserData					( CPhysicUserData *_userData );
	CCharacter*			IsPlayerNearEnemy					( float distance );
	CCharacter*			SearchTargetEnemy					( float _Distance, float _AngleVisible, const Vect3f &_Front );
	bool				EnemyIsVisibleInAngle				( CCharacter *_Enemy, float _Angle, const Vect3f &_Front );

	void				DrawFustrum							( void );
	void				DrawFront							( void );
	void				DrawNames							( CFontManager *_FM );
	void				DrawRay								( void );

	// ------------- Propietats ( Get / Set ) ----------------
	TVectorResources	GetEnemiesVector					() const					{ return m_ResourcesVector; }
	TMapResources		GetEnemiesMap						() const					{ return m_ResourcesMap; }
	inline int			GetTotalEnemies						() const					{ return m_ResourcesVector.size(); }
	inline CCharacter*	GetPlayer							() const					{ return m_pPlayer; }
	inline CCharacter*	GetTargetEnemy						() const					{ return m_pTargetEnemy; }
	inline CCharacter*	GetPreviewTargetEnemy				() const					{ return m_pPreviewTargetEnemy; }
	inline CCharacter*	GetEnemyByName						( const std::string &name )	{ return GetResource(name); }

	inline void			SetPlayer							( CCharacter *player )		{ m_pPlayer = player; }
	inline void			SetTargetEnemy						( CCharacter *enemy )		{ m_pTargetEnemy = enemy; }
	inline void			SetPreviewTargetEnemy				( CCharacter *enemy )		{ m_pPreviewTargetEnemy = enemy; }

	// ------------- Load Methods --------------------------------
private:
	bool				LoadXML								();									// Carga el XML de propiedades y estados
	bool				LoadDefaultCoreProperties			( const CXMLTreeNode &_Node );		// Carga las propiedades por defecto del XML
	bool				LoadPlayerProperties				( const CXMLTreeNode &_Node );		// Carga una nueva instancia del player dependiendo de las propiedades obtenidas
	bool				LoadEnemiesProperties				( const CXMLTreeNode &_Node );		// Carga una enemigos dependiendo de las propiedades obtenidas
	bool				LoadDefaultCoreAnimatedStates		( const CXMLTreeNode &_Node );		// Carga los estados por defecto del XML
	bool				LoadPlayerAnimationStates			( const CXMLTreeNode &_Node );		// Carga una nueva instancia del player y mete los estados obtenidos
	bool				LoadEnemiesAnimationStates			( const CXMLTreeNode &_Node );		// Carga una enemigos y mete los estados obtenidos

	Vect3f				RandomVector						( const Vect3f &_Vect1, const Vect3f &_Vect2 );

	void				CalculateEnemyOrderToAttack			( const Vect3f & _Position, float _ViewDistance );

	// ------------- Members --------------------------------
private:
	std::string					m_PropertiesFileName;
	std::string					m_AnimatedFileName;

	CPropertiesManager			*m_pPropertiesManager;
	CAnimationsStatesManager	*m_pAnimatedStatesManager;

	CCharacter					*m_pPlayer;
	CCharacter					*m_pTargetEnemy;
	CCharacter					*m_pPreviewTargetEnemy;

	std::vector<CPhysicUserData*> m_UserDatas;		// Permite almacenar aquellos enemigos en zona de detección del player para ordenarlos y decidir quien puede o no atacar
													// Básicamente se le pone un flag a la Steering Entity para determinar si va hacia el player o no
};

#endif
