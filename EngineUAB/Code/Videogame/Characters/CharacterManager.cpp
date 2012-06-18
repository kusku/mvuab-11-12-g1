#define NOMINMAX

#include <windows.h>
#include <sstream>
#include <luabind/adopt_policy.hpp>

#include "CharacterManager.h"

#include "Math\Vector3.h"
#include "Utils\Random.h"
#include "Logger\Logger.h"
#include "Base.h"
#include "Core.h"

#include "Object3D.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObject.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "RenderManager.h"

#include "Particles\ParticleEmitterManager.h"
#include "Particles\ParticleEmitter.h"

#include "Fonts\FontManager.h"
#include "PhysicController.h"
#include "CharacterController.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"

#include "Cameras\Camera.h"

#include "characters\Properties\PropertiesManager.h"
#include "characters\Properties\Properties.h"
#include "characters\states\AnimationsStatesManager.h"
#include "characters\states\AnimationsStates.h"

#include "StatesMachine\EntityManager.h"
#include "StatesMachine\MessageDispatcher.h"

#include "Billboard\BillboardManager.h"
#include "Billboard\BillboardAnimation.h"

#include "Cameras\ThPSCamera.h"
#include "EngineProcess.h"
#include "GameProcess.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------
CCharactersManager::CCharactersManager ( void )
	: m_PropertiesFileName		( "" )
	, m_AnimatedFileName		( "" )
	, m_pPropertiesManager		( NULL )
	, m_pAnimatedStatesManager	( NULL )
	, m_pPlayer					( NULL )
	, m_pTargetEnemy			( NULL )
	, m_pPreviewTargetEnemy		( NULL )
{
}

CCharactersManager::~CCharactersManager(void)
{
	CleanUp();
	//CleanReloadScripts();
}

//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Initialize: Permite crear un nº de enemigos concretos. Si no se pasa el parámetro se coje toda la info
//			encontrada en el RenderableObjectsManager llamado "solid"
// ---------------------------------------------------------------------------------------------------------
bool CCharactersManager::Initialize ( int _NumEnemies )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CCharactersManager::Initialize-> Initializating characters manager..." );

	// Inicializamos el manager de propiedades por defecto de los personajes
	m_pPropertiesManager	 = new CPropertiesManager();
	m_pAnimatedStatesManager = new CAnimationsStatesManager();
	
	if ( !Load( "./data/xml/characters_properties.xml", "./data/xml/characters_animated_states.xml" ) )
	{
		LOGGER->AddNewLog ( ELL_ERROR, "CCharactersManager::Initialize-> Errors in the initialization when loading properties and states", _NumEnemies );
		return false;
	}
	
	LOGGER->AddNewLog ( ELL_INFORMATION, "CCharactersManager::Initialize-> Initialization succesful", _NumEnemies );
	return true;
}

void CCharactersManager::CleanReloadScripts ( void )
{
	CHECKED_DELETE ( m_pPropertiesManager );		// Eliminamos las propiedades por defecto
	CHECKED_DELETE ( m_pAnimatedStatesManager );	// Eliminamos los estados por defecto

	// Estos cambios són debido a la nueva forma y reload de cargar los caracteres mediante Lua
	CORE->GetPhysicsManager()->ReleasePhysicController(m_pPlayer->GetController());
	m_pPlayer = NULL;
	
	TVectorResources::iterator l_It = m_ResourcesVector.begin();
	TVectorResources::iterator l_End = m_ResourcesVector.end();
	
	for ( l_It; l_It < l_End; l_It++ )
	{
		CORE->GetPhysicsManager()->ReleasePhysicController((*l_It)->GetController());
		(*l_It) = NULL;
	}

	// Lua destruye los objetos pero aquí limpio las listas para evitar el error
	m_ResourcesMap.clear();
	m_ResourcesVector.clear();
}

void CCharactersManager::CleanUp ( void )
{
	CHECKED_DELETE ( m_pPropertiesManager );		// Eliminamos las propiedades por defecto
	CHECKED_DELETE ( m_pAnimatedStatesManager );	// Eliminamos los estados por defecto
	CHECKED_DELETE ( m_pPlayer );
	Destroy();
}

//----------------------------------------------------------------------------
// Load : Per carregar un fitxer XML amb tots els enemics
//----------------------------------------------------------------------------
bool CCharactersManager::Load( const std::string &_PropertyFileName, const std::string &_AnimatedStatesFileName )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CCharactersManager::Load-->Loading characters, properties and states" );
	m_PropertiesFileName	= _PropertyFileName;
	m_AnimatedFileName		= _AnimatedStatesFileName;
	return LoadXML();
}

////----------------------------------------------------------------------------
//// Reload : Per recarregar un fitxer XML amb tots els enemics
////----------------------------------------------------------------------------
bool CCharactersManager::Reload ( void )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CCharactersManager::Reload-->Reloading characters, properties and states" );
	CleanUp();
	return LoadXML();
}

//----------------------------------------------------------------------------------------------------
// LoadXML : Tracta la càrrega dels fitxers XML amb tots els enemics, players i valors per defecte
//----------------------------------------------------------------------------------------------------
bool CCharactersManager::LoadXML( void )
{
	bool l_IsOk;

	LOGGER->AddNewLog ( ELL_INFORMATION, "CCharactersManager::LoadXML --> Loading character properties ..." );
	l_IsOk = LoadXMLProperties();
	l_IsOk &= LoadXMLAnimatedStates();
	return l_IsOk;
}

//----------------------------------------------------------------------------------------------------
// Update : Actualiza el player i los enemigos registrados en el manager
//----------------------------------------------------------------------------------------------------
void CCharactersManager::Update ( float _ElapsedTime )
{
	assert(m_pPlayer != NULL);

	// Actualitzem el player
	m_pPlayer->Update( _ElapsedTime );
	
	// Actualitzem l'enemic
	TVectorResources l_EnemyList = GetResourcesVector();
	for ( size_t i = 0; i < l_EnemyList.size(); i++ )
	{
		if ( l_EnemyList[i]->GetProperties()->GetActive() )
		{
			l_EnemyList[i]->Update( _ElapsedTime );
			if ( !l_EnemyList[i]->IsAlive() )
			{
				CORE->GetParticleEmitterManager()->GetResource("Explosions")->SetPosition(l_EnemyList[i]->GetPosition());
				CORE->GetParticleEmitterManager()->GetResource("Explosions")->EjectParticles();
				l_EnemyList[i]->SetEnable(false);
				//CHECKED_DELETE(l_EnemyList[i]);
				/*m_pTargetEnemy = NULL;
				m_pPreviewTargetEnemy = NULL;*/
			}
		}
	}

	//Actualiza el billboard del target enemy
	if( m_pTargetEnemy != NULL )
	{
		Vect3f l_Pos = m_pTargetEnemy->GetPosition();
		l_Pos.y += m_pTargetEnemy->GetController()->GetHeight() + 1.5f;

		CORE->GetBillboardManager()->GetBillboardInstance("target_enemy")->SetPosition(l_Pos);
		CORE->GetBillboardManager()->GetBillboardInstance("target_enemy")->SetVisible(true);
		CORE->GetBillboardManager()->GetBillboardInstance("preview_target_enemy")->SetVisible(false);
	}
	else
	{
		if ( m_pPreviewTargetEnemy != NULL )
		{
			Vect3f l_Pos = m_pPreviewTargetEnemy->GetPosition();
			l_Pos.y += m_pPreviewTargetEnemy->GetController()->GetHeight() + 1.5f;

			CORE->GetBillboardManager()->GetBillboardInstance("preview_target_enemy")->SetPosition(l_Pos);
			CORE->GetBillboardManager()->GetBillboardInstance("preview_target_enemy")->SetVisible(true);
		}
		else
		{
			CORE->GetBillboardManager()->GetBillboardInstance("preview_target_enemy")->SetVisible(false);
		}
		CORE->GetBillboardManager()->GetBillboardInstance("target_enemy")->SetVisible(false);
	}
}

void CCharactersManager::Render(CRenderManager *_RM, CFontManager *_FM)
{
	int life = m_pPlayer->GetProperties()->GetLife();
	_FM->DrawDefaultText(10, 50, colBLACK, "Life: %d", life);
}

//--------------------------------------------------
//					FUNCTIONS 
//--------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// LoadXMLProperties : permite cargar las propiedades por defecto de los cores y de esta manera tener
//						valores por defecto para ese core
//-------------------------------------------------------------------------------------------------------------
bool CCharactersManager::LoadXMLProperties( void )
{
	bool l_IsOk = true;
	CXMLTreeNode l_File;
	if ( !l_File.LoadFile( m_PropertiesFileName.c_str() ) )
	{
		std::string msg_error = "CCharactersManager::LoadXML->Error trying to read the characters properties file : " + m_PropertiesFileName;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	// Por cada elemento cargamos las correspondientes propiedades
	CXMLTreeNode l_Characters = l_File["characters"];
	if( l_Characters.Exists() )
	{
		uint16 l_Count = l_Characters.GetNumChildren();
		for ( uint16 i = 0; i < l_Count; ++i )
		{
			std::string l_Type = l_Characters(i).GetName();
			if( l_Type == "default_properties" ) // Cargamos las propiedades de los cores
			{
				l_IsOk = LoadDefaultCoreProperties( l_Characters(i) );
			}
			else if( l_Type == "player" ) // Cargamos las propiedades del player
			{
				l_IsOk &= LoadPlayerProperties( l_Characters(i) );
			}
			else if( l_Type == "enemies" ) // Cargamos las propiedades de los enemigos
			{
				l_IsOk &= LoadEnemiesProperties( l_Characters(i) );
			}
		}
	}
	return l_IsOk;
}

//-------------------------------------------------------------------------------------------------------------
// LoadXMLAnimatedStates : permite cargar los estados por defecto de los cores y de esta manera tener
//						valores por defecto para ese core
//-------------------------------------------------------------------------------------------------------------
bool CCharactersManager::LoadXMLAnimatedStates( void )
{
	bool l_IsOk = true;
	CXMLTreeNode l_File;
	if ( !l_File.LoadFile( m_AnimatedFileName.c_str() ) )
	{
		std::string msg_error = "CCharactersManager::LoadXML->Error trying to read the characters states file : " + m_AnimatedFileName;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	// Por cada elemento cargamos los correspondientes estados
	CXMLTreeNode l_Characters = l_File["characters"];
	if( l_Characters.Exists() )
	{
		uint16 l_Count = l_Characters.GetNumChildren();
		for ( uint16 i = 0; i < l_Count; ++i )
		{
			std::string l_Type = l_Characters(i).GetName();
			if( l_Type == "default_states" ) // Cargamos los estados de los cores
			{
				l_IsOk = LoadDefaultCoreAnimatedStates( l_Characters(i) );
			}
			else if( l_Type == "player" ) // Cargamos los estados del player
			{
				l_IsOk &= LoadPlayerAnimationStates( l_Characters(i) );
			}
			else if( l_Type == "enemies" ) // Cargamos los estados de los enemigos
			{
				//l_IsOk &= LoadEnemiesAnimationStates( l_Characters(i) );
			}
		}
	}
	return l_IsOk;
}

//-------------------------------------------------------------------------------------------------------------
// LoadDefaultProperties : permite cargar las propiedades por defecto de los cores y de esta manera tener
//						valores por defecto para ese core
//-------------------------------------------------------------------------------------------------------------
bool CCharactersManager::LoadDefaultCoreProperties( const CXMLTreeNode &_Node )
{
	return m_pPropertiesManager->LoadDefaultProperties( _Node );
}

//-------------------------------------------------------------------------------------------------------------
// LoadPlayer : permite cargar un player con sus propiedades individuales y asociarle los componentes 
//			de física, lógica y renderizado
//-------------------------------------------------------------------------------------------------------------
bool CCharactersManager::LoadPlayerProperties( const CXMLTreeNode &_Node )
{
	bool l_IsOk = true;

	// Si no existe el player lo creamos en LUA
	if ( !m_pPlayer )
	{
		m_pPlayer = call_function<CCharacter*>(SCRIPT->GetLuaState(), "CPlayer", 0)[adopt(result)];
	}

	// Obtenemos las propiedades del player
	CProperties* l_PlayerProperties = m_pPropertiesManager->LoadPlayerProperties( _Node ); 
	if ( l_PlayerProperties )
	{
		//asignamos las propiedades al player
		m_pPlayer->SetProperties( l_PlayerProperties );
		
		// Inicializamos el player, sus estados, mayas animadas...
		m_pPlayer->Initialize( l_PlayerProperties->GetName(), m_pPlayer->GetProperties()->GetPosition(), ::ECG_PERSONATGE );
		l_IsOk = m_pPlayer->Init();		// Llamada a Lua
		ENTMGR->RegisterEntity(m_pPlayer);
	}
	else 
	{
		m_pPlayer->SetProperties( l_PlayerProperties );
		l_IsOk = false;
	}

	//l_IsOk &= m_pPlayer->Init();
	return l_IsOk;
}

//-------------------------------------------------------------------------------------------------------------
// LoadEnemies : permite cargar los enemigos con sus propiedades individuales  y asociarles los componentes 
//			de física, lógica y renderizado
//-------------------------------------------------------------------------------------------------------------
bool CCharactersManager::LoadEnemiesProperties( const CXMLTreeNode &_Node )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CCharactersManager::LoadEnemiesProperties-->Loading enemies and properties." );
	
	bool l_IsOk = false;
	int  l_NextIDValid = m_pPlayer->GetID() + 1;

	CXMLTreeNode l_EnemiesNode = _Node;
	std::string l_Type = l_EnemiesNode.GetName();		// Les propietats van primer
	if( l_Type == "enemies" )
	{
		uint16 l_Count = l_EnemiesNode.GetNumChildren();
		for ( uint16 i = 0; i < l_Count; ++i )
		{
			l_Type = l_EnemiesNode(i).GetName();
			if (  l_Type == "enemy" )
			{
				CProperties* l_EnemyProperties = m_pPropertiesManager->LoadEnemyProperties( l_EnemiesNode(i) ); 
				if ( l_EnemyProperties )
				{
					CCharacter* l_Character = GetResource( l_EnemyProperties->GetName() );
					/*CEnemy* l_Enemy = NULL;
					if ( !l_Character )
						l_Enemy = new CEnemy(l_NextIDValid);
					else
						l_Enemy = dynamic_cast<CEnemy*> (l_Character);*/

					//asignamos las propiedades al player
					/*l_Character->SetProperties ( l_EnemyProperties );
					l_Character->Init();
					AddEnemy ( l_Enemy );
					l_NextIDValid += 1;
					l_IsOk = true;*/

					if ( !l_Character )
					{
						//std::string l_Num = ConvertInt( l_NextIDValid );
						//SCRIPT->RunCode("add_enemy( " + l_Num + ", '" + l_EnemyProperties->GetName() + "')");
						if ( l_EnemyProperties->GetCore() == "lobo" )
							l_Character = call_function<CCharacter*>(SCRIPT->GetLuaState(), "CWolf", l_NextIDValid, l_EnemyProperties->GetName() )[adopt(result)];
						if ( l_EnemyProperties->GetCore() == "conejo" ) 
							l_Character = call_function<CCharacter*>(SCRIPT->GetLuaState(), "CRabbit", l_NextIDValid, l_EnemyProperties->GetName() )[adopt(result)];
					}

					// Asignamos las propiedades
					l_Character->SetProperties ( l_EnemyProperties );
		
					// Inicializamos el player, sus estados, mayas animadas...
					l_IsOk = l_Character->Initialize( l_EnemyProperties->GetName(), l_Character->GetProperties()->GetPosition(), ::ECG_ENEMICS );
					l_IsOk &= l_Character->InitializeAI();
					l_IsOk &= l_Character->Init();		// Llamada a Lua
					AddEnemy( l_Character );			// La meto dentro de la lista
					l_NextIDValid += 1;					// Pròxim ID vàlid
					ENTMGR->RegisterEntity(l_Character);
				}
				else 
				{
					std::string msg_error = "CCharactersManager::LoadEnemiesProperties--> Error when trying to load enemy properties : " + l_Type;
					LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
					l_IsOk = false;
				}
			}
			else if ( l_Type != "comment" ) 
			{
				std::string msg_error = "CCharactersManager::LoadEnemiesProperties--> Error, it cannot read the command line : " + l_Type;
				LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			}
		}
	}
	else 
	{
		std::string msg_error = "CCharactersManager::LoadEnemiesProperties --> Error when trying to load enemies properties : " + l_Type;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}
	return l_IsOk;
}

bool CCharactersManager::LoadDefaultCoreAnimatedStates( const CXMLTreeNode &_Node )
{
	return m_pAnimatedStatesManager->LoadDefaultAnimationsStates( _Node );
}

bool CCharactersManager::LoadPlayerAnimationStates( const CXMLTreeNode &_Node )
{
	bool l_IsOk;
	
	// Si no existe el player lo creamos en LUA
	if ( !m_pPlayer )
		SCRIPT->RunCode("add_player()");


	// Obtenemos los estados del player
	CAnimationsStates* l_PlayerAnimationsStates = m_pAnimatedStatesManager->LoadPlayerAnimationsStates( _Node ); 
	if ( l_PlayerAnimationsStates )
	{
		//asignamos los estados al player
		m_pPlayer->SetAnimationsStates( l_PlayerAnimationsStates );
		l_IsOk = true;
	}
	else 
	{
		m_pPlayer->SetAnimationsStates ( l_PlayerAnimationsStates );
		l_IsOk = false;
	}

	return l_IsOk;
}

bool CCharactersManager::LoadEnemiesAnimationStates( const CXMLTreeNode &_Node )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CCharactersManager::LoadEnemiesAnimationStates-->Loading enemies animations states." );
	
	bool l_IsOk = true;
	int  l_NextIDValid = m_pPlayer->GetID() + 1;

	CXMLTreeNode l_EnemiesNode = _Node;
	std::string l_Type = l_EnemiesNode.GetName();		// Les propietats van primer
	if( l_Type == "enemies" )
	{
		uint16 l_Count = l_EnemiesNode.GetNumChildren();
		for ( uint16 i = 0; i < l_Count; ++i )
		{
			/*l_Type = l_EnemiesNode(i).GetName();
			if (  l_Type == "enemy" )
			{
				CAnimationsStates* l_EnemyStates = m_pAnimatedStatesManager->LoadEnemyAnimationsStates( l_EnemiesNode(i) ); 
				if ( l_EnemyStates )
				{
					std::string l_EnemyName = l_EnemiesNode(i).GetPszProperty( "name", "", true );
					CCharacter* l_Character = GetResource( l_EnemyName );
					CEnemy* l_Enemy = NULL;
					if ( !l_Character )
					{
						std::string msg_error = "CCharactersManager::LoadEnemiesAnimationStates--> Error, enemy " + l_EnemyName + " no found";
						LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
						l_IsOk &= false;
					}
					else
					{
						l_Enemy = dynamic_cast<CEnemy*> (l_Character);
						l_Enemy->SetAnimationsStates( l_EnemyStates );
						l_IsOk &= true;
					}
				}
				else 
				{
					std::string msg_error = "CCharactersManager::LoadEnemiesProperties--> Error when trying to load enemy properties : " + l_Type;
					LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
					l_IsOk = false;
				}
			}
			else if ( l_Type != "comment" ) 
			{
				std::string msg_error = "CCharactersManager::LoadEnemiesProperties--> Error, it cannot read the command line : " + l_Type;
				LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			}*/
		}
	}
	else 
	{
		std::string msg_error = "CCharactersManager::LoadEnemiesProperties --> Error when trying to load enemies properties : " + l_Type;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}
	return l_IsOk;
}

void CCharactersManager::AddEnemy ( CCharacter *_pEnemy )
{
	AddResource ( _pEnemy->GetName(), _pEnemy );
}

// ---------------------------------------------------------------------------------------------------------
// CreateRandomEnemy: Crea enemigo de forma aleatoria entre unas coordenadas especificas y lo añade al mapa
// ---------------------------------------------------------------------------------------------------------
void CCharactersManager::CreateEnemy ( void )
{
	/*std::stringstream out;
		out << "_";
		out << GetTotalEnemies();
	
	CRandom	l_Randomize;
	int l_Rand= l_Randomize.getRandUnsigned( ::LOBO, ::CONEJO );*/

	//TIPO_ENEMIGO l_TipoEnemigo = static_cast<TIPO_ENEMIGO> (l_Rand);
//	CEnemy * l_Enemy = new CEnemy ( "Enemic" + out.str(), ENEMY, RandomVector ( Vect3f (-10,0,-10), Vect3f (10,0,10) ) );

	//l_Enemy->Initialize				( );			// Inicialitza els waypoints. En el Jugador no cal
	//l_Enemy->setPlayer				( m_pPlayer );
	//l_Enemy->setInitialPosition		( CreateRandomPosition() );
	/*l_Enemy->setRotationSpeed		( MATH.degToRad( ENEMY_ROTATION_SPEED ) );
	l_Enemy->setSpeed				( ENEMY_SPEED );
	l_Enemy->setDetectionDistance	( ENEMY_SOUND_DETECTION );
	l_Enemy->setFrustumAngle		( ENEMY_VISION_ANGLE );
	l_Enemy->setFrustumDistance		( ENEMY_VISION_DISTANCE );
	l_Enemy->setEstado				( ::PARADO );*/

	//AddEnemy ( l_Enemy );
}

// ---------------------------------------------------------------------------------------------------------
// CreateRandomEnemy: Crea enemigo de forma aleatoria entre unas coordenadas especificas y lo añade al mapa
// ---------------------------------------------------------------------------------------------------------
void CCharactersManager::CreateRandomEnemy ( void )
{
	std::stringstream out;
		out << "_";
		out << GetTotalEnemies();
	
	/*CEnemy * l_Enemy = new CEnemy ( "Enemic" + out.str(), ::ENEMY, RandomVector ( Vect3f (-10,0,-10), Vect3f (10,0,10) ) );
	AddEnemy ( l_Enemy );*/
}

////void CCharactersManager::UpdateDistanceToPlayer ( size_t _IndexRail, float _ElapsedTime, float _BackDistance )
////{
////	// --- Calculo la Posicion dins el Rail
////	Vect3f	l_PosPlayer = m_pPlayer->getPosition();
////	Vect3f	l_Origen, l_Desti; 
////
////	// Pillem el rail indicat i les seves coordenades
////	m_RailList[_IndexRail].GetRailLine ( l_Origen, l_Desti );
////	
////	// Permet allunyar un xic la càmera del player. Dona major prespectiva
////	if ( _BackDistance != 0.0f )
////		l_PosPlayer = l_PosPlayer + m_pPlayer->getFront () * _BackDistance;
////	
////	// Calculo la posició de la camera respecte el player dins del rail
////	Vect3f	l_PosProjectatRail = MATH.FindPointInLine( l_Origen, l_Desti, l_PosPlayer ); 
////
////	m_RailList[_IndexRail].SetPositionInRailToPlayer ( l_PosProjectatRail );
////	m_RailList[_IndexRail].SetDistanceToPlayer		 ( l_PosProjectatRail.distance( l_PosPlayer) );
////}
//
////void CCharactersManager::ActiveNearestRail ( float _ElapsedTime, float _BackDistance )
////{
////	size_t	l_Index = 0;
////	float	l_Distance;
////
////	if ( m_RailList.size () == 0 )
////		return;
////
////	UpdateDistanceToPlayer( 0, _ElapsedTime, _BackDistance );
////	l_Distance =  m_RailList[0].GetDistanceToPlayer();
////	m_IndexNearestIndexRail = 0;
////	m_IndexActiveRail		= 0;
////
////	for ( size_t i = 1; i < m_RailList.size(); i++ )
////	{
////		// Actualizo la distancia i posició del punt on està la càmera sobre el rai respecte el player
////		UpdateDistanceToPlayer( i, _ElapsedTime, _BackDistance );
////
////		float l = m_RailList[i].GetDistanceToPlayer();
////		// Si el seguent rail fa que el player el tingui més aprop asigno el nou rail
////		if (  ( m_RailList[i].GetDistanceToPlayer() ) < l_Distance )
////		{
////			l_Distance = m_RailList[i].GetDistanceToPlayer();
////			m_IndexNearestIndexRail = i;		// deixo activat aquest rail
////			m_IndexActiveRail		= i; 
////		}
////	}
////}
//
//void CCharactersManager::DrawActors ( void )
//{
//	/*m_pPlayer->Render();
//
//	for ( size_t i = 0; i < m_EnemyList.size(); i++ )
//		m_EnemyList[i]->Render();*/
//}
//

Vect3f CCharactersManager::RandomVector( const Vect3f &_Vect1, const Vect3f &_Vect2 )
{
	CRandom	l_Randomize;

	// determine a random vector between _vect1 and _vect2
	float fRandX = l_Randomize.getRandFloat( _Vect1.x, _Vect2.x );
	float fRandY = l_Randomize.getRandFloat( _Vect1.y, _Vect2.y );
	float fRandZ = l_Randomize.getRandFloat( _Vect1.z, _Vect2.z );
	
	return ( Vect3f ( fRandX, fRandY, fRandZ ) );
}

CCharacter* CCharactersManager::ExistEnemyUserData(CPhysicUserData *_userData)
{
	if( _userData != NULL )
	{
		TVectorResources l_EnemyList = GetResourcesVector();
		for(uint16 i=0; i<l_EnemyList.size(); ++i)
		{
			if( l_EnemyList[i]->GetPhysicUserData() == _userData )
			{
				return l_EnemyList[i];
			}
		}
	}
	return NULL;
}

CPhysicUserData* CCharactersManager::ShootPlayerRaycast()
{
	SCollisionInfo l_Info;
	Vect3f l_Pos = m_pPlayer->GetPosition();
	Vect3f l_Dir = CORE->GetCamera()->GetDirection();
	l_Dir.y = 0.0f;
	l_Dir.Normalize();

	l_Pos.y += m_pPlayer->GetController()->GetHeight()/2;
	l_Pos += l_Dir;

	int mask = 1 << ECG_PERSONATGE;
	mask |= 1 << ECG_OBJECTES_DINAMICS;
	mask |= 1 << ECG_ESCENARI;
	mask |= 1 << ECG_ENEMICS;

	CPhysicUserData *userdata = CORE->GetPhysicsManager()->RaycastClosestActor(l_Pos, l_Dir, mask, l_Info);
	return userdata;
}

CCharacter* CCharactersManager::IsPlayerNearEnemy(float distance)
{
	CCharacter *l_NearestEnemy = NULL;
	float l_NearestDistance = 0.f;
	Vect3f l_Pos = m_pPlayer->GetPosition();

	TVectorResources l_EnemyList = GetResourcesVector();

	if( l_EnemyList.size() == 0 )
	{
		return NULL;
	}

	l_NearestEnemy = l_EnemyList[0];
	l_NearestDistance = l_Pos.Distance(l_NearestEnemy->GetPosition());

	TVectorResources::iterator l_It = l_EnemyList.begin();
	TVectorResources::iterator l_End = l_EnemyList.end();
	for(; l_It != l_End; ++l_It)
	{
		if( (*l_It)->IsAlive() )
		{
			float l_CurrentDistance = l_Pos.Distance( (*l_It)->GetPosition() );
			if( l_CurrentDistance <= distance && l_CurrentDistance < l_NearestDistance )
			{
				l_NearestDistance = l_CurrentDistance;
				l_NearestEnemy = (*l_It);
			}
		}
	}

	return l_NearestEnemy;
}

CCharacter* CCharactersManager::SearchTargetEnemy(float _Distance, float _AngleVisible)
{
	assert( _Distance > 0.f );
	assert( _AngleVisible > 0.f );

	CCharacter *l_NearestEnemy = NULL;
	bool l_IsEnemyFound = false;
	float l_NearestDistance = 0.f;
	Vect3f l_Pos = m_pPlayer->GetPosition();

	TVectorResources l_EnemyList = GetResourcesVector();

	if( l_EnemyList.size() == 0 )
	{
		return NULL;
	}

	//Inicializamos los datos de enemigo para comparar
	l_NearestEnemy = l_EnemyList[0];
	l_NearestDistance = l_Pos.Distance(l_NearestEnemy->GetPosition());

	TVectorResources::iterator l_It = l_EnemyList.begin();
	TVectorResources::iterator l_End = l_EnemyList.end();
	for(; l_It != l_End; ++l_It)
	{
		if( (*l_It)->IsAlive() )
		{
			//Mira si el enemigo está más cerca que el resto del player
			float l_CurrentDistance = l_Pos.Distance( (*l_It)->GetPosition() );
			if( l_CurrentDistance <= _Distance && l_CurrentDistance <= l_NearestDistance )
			{
				//Mira si el enemigo que está cerca está visible en un ángulo respecto el player
				if( EnemyIsVisibleInAngle( (*l_It), _AngleVisible) ) 
				{
					l_NearestDistance = l_CurrentDistance;
					l_NearestEnemy = (*l_It);
					l_IsEnemyFound = true;
				}
			}
		}
	}

	if( !l_IsEnemyFound )
	{
		return NULL;
	}

	return l_NearestEnemy;
}

bool CCharactersManager::EnemyIsVisibleInAngle(CCharacter *_Enemy, float _Angle)
{
	assert( m_pPlayer );
	assert( _Enemy );
	assert( _Angle > 0.f );
	//Cogemos la dirección del player
	//Vect2f dir = Vect2f( mathUtils::Abs(mathUtils::Cos(m_pPlayer->GetYaw())), mathUtils::Abs(mathUtils::Sin(m_pPlayer->GetYaw() ) ) );
	//Vect3f l_DirPlayer = Vect3f( dir.x, 0.0f, dir.y);
	Vect3f l_DirPlayer = static_cast<CGameProcess*>(CORE->GetProcess())->GetPlayerCamera()->GetDirection();
	l_DirPlayer.y = 0.f;

	//Calculamos el vector entre el player y el enemigo
	Vect3f l_DirEnemy = _Enemy->GetPosition() - m_pPlayer->GetPosition();
	l_DirEnemy.y = 0.f;
	l_DirEnemy.Normalize(1.f);

	//Calculamos el ángulo entre los dos vectores
	float l_Angle = l_DirPlayer.AngleWithVector(l_DirEnemy);

	if( l_Angle > _Angle )
	{
		return false;
	}

	return true;
}
