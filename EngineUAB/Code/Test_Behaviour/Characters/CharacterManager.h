#pragma once

#include <vector>
#include <string>

//#include "Rail.h"
//#include "TestDef.h"
//#include "MathHelper.h"
//#include "RandomHelper.h"

#include "TestBehaviourDefs.h"
#include "Characters\Character.h"

#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\MapManager.h"
#include "XML\XMLTreeNode.h"

//--- Foward Declarations ---
class CRenderableObjectsManager;
class CPropertiesManager;
class CAnimationsStatesManager;
class CPlayer;
class CEnemy;
//---------------------------


class CCharactersManager : public CTemplatedVectorMapManager<CCharacter> //public CRandomHelper
{
public:
	// ------------- Constructors i Destructors --------------
								//CCharactersManager			( CActor *_pPlayer, int _NumEnemies );
								//CCharactersManager			( int _NumEnemies = 0 );
								CCharactersManager				( void );
								~CCharactersManager				( void );

	// ------------- Funcions Principals ---------------------
	bool						Initialize						( int _NumEnemies = 0 );
	bool						Load							( const std::string &_PropertyFileName, const std::string &_AnimatedStatesFileName );
	bool						Reload							( void );
	void						CleanUp							( void );
	void						Update							( float _ElapsedTime );
	void						Render							( void );

	// ------------- Funcions --------------------------------
	void						AddEnemy						( CCharacter *_pEnemy );			// Afegeix un enemic ja creat
	void						CreateEnemy						( void );							// Crea i afegeix un enemic ja creat
	void						CreateRandomEnemy				( void );							// Crea i afegeix un enemic aleatori
	bool						LoadXMLProperties				( void );							// Carga el XML de propiedades
	bool						LoadXMLAnimatedStates			( void );							// Carga el XML de estados

private:
	bool						LoadXML							( void );							// Carga el XML de propiedades y estados
	bool						LoadDefaultCoreProperties		( const CXMLTreeNode &_Node );		// Carga las propiedades por defecto del XML
	bool						LoadPlayerProperties			( const CXMLTreeNode &_Node );		// Carga una nueva instancia del player dependiendo de las propiedades obtenidas
	bool						LoadEnemiesProperties			( const CXMLTreeNode &_Node );		// Carga una enemigos dependiendo de las propiedades obtenidas
	bool						LoadDefaultCoreAnimatedStates	( const CXMLTreeNode &_Node );		// Carga los estados por defecto del XML
	bool						LoadPlayerAnimationStates		( const CXMLTreeNode &_Node );		// Carga una nueva instancia del player y mete los estados obtenidos
	bool						LoadEnemiesAnimationStates		( const CXMLTreeNode &_Node );		// Carga una enemigos y mete los estados obtenidos

	Vect3f						RandomVector					( const Vect3f &_Vect1, const Vect3f &_Vect2 );

	std::string					ConvertInt						( int _Number );

	
	// ------------- Propietats ( Get / Set ) ----------------
public:
	TVectorResources			GetEnemiesVector				( void ) const				{ return m_ResourcesVector; }
	TMapResources				GetEnemiesMap					( void ) const				{ return m_ResourcesMap; }

	int							GetTotalEnemies					( void ) const				{ return m_ResourcesVector.size(); }
	
	CCharacter*					GetPlayer						( void ) const				{ return m_pPlayer; }
	void						SetPlayer						( CCharacter *_pPlayer )	{ m_pPlayer = _pPlayer; }

private:
	std::string					m_PropertiesFileName;
	std::string					m_AnimatedFileName;

	CPropertiesManager			*m_pPropertiesManager;
	CAnimationsStatesManager	*m_pAnimatedStatesManager;

	CCharacter					*m_pPlayer;
};

