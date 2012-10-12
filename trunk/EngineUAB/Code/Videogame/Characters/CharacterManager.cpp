#define NOMINMAX

#include <windows.h>
#include <sstream>
#include <luabind/adopt_policy.hpp>

#include "CharacterManager.h"
#include "Character.h"

#include "Enemies\Rabbit\Rabbit.h"
#include "Enemies\Wolf\Wolf.h"
#include "Enemies\Deer\Deer.h"

#include "Characters\CharactersDefs.h"
#include "Math\Vector3.h"
#include "Utils\BoostRandomHelper.h"
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
#include "Particles\ParticleEmitterInstance.h"
#include "Particles\ParticleEmitter.h"

#include "Fonts\FontManager.h"
#include "PhysicController.h"
#include "CharacterController.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"

#include "characters\Properties\PropertiesManager.h"
#include "characters\Properties\Properties.h"
#include "characters\states\AnimationsStatesManager.h"
#include "characters\states\AnimationsStates.h"
#include "Characters\Player\Player.h"
#include "Characters\Enemies\Wolf\LogicStates\WolfHowlEnemiesState.h"

#include "StatesMachine\EntityManager.h"
#include "StatesMachine\MessageDispatcher.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\Seek.h"

#include "Billboard\BillboardManager.h"
#include "Billboard\BillboardAnimation.h"

#include "Cameras\Camera.h"
#include "Cameras\ThPSCharacterCamera.h"

#include "Helpers\MathHelper.h"

#include "EngineProcess.h"
#include "GameProcess.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------
CCharactersManager::CCharactersManager ( void )
	: m_PropertiesFileName				( "" )
	, m_AnimatedFileName				( "" )
	, m_pPropertiesManager				( NULL )
	, m_pAnimatedStatesManager			( NULL )
	, m_pPlayer							( NULL )
	, m_pTargetEnemy					( NULL )
	, m_pPreviewTargetEnemy				( NULL )
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
bool CCharactersManager::Initialize( int _NumEnemies )
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

void CCharactersManager::CleanReloadScripts( void )
{
	CHECKED_DELETE ( m_pPropertiesManager );		// Eliminamos las propiedades por defecto
	CHECKED_DELETE ( m_pAnimatedStatesManager );	// Eliminamos los estados por defecto

	// Estos cambios són debido a la nueva forma y reload de cargar los caracteres mediante Lua
	CORE->GetPhysicsManager()->ReleasePhysicController(m_pPlayer->GetController());
	m_pPlayer = NULL;
	
	TVectorResources::iterator l_It = m_ResourcesVector.begin();
	TVectorResources::iterator l_End = m_ResourcesVector.end();
	
	for ( l_It; l_It < l_End; ++l_It )
	{
		CORE->GetPhysicsManager()->ReleasePhysicController((*l_It)->GetController());
		(*l_It) = NULL;
	}

	// Lua destruye los objetos pero aquí limpio las listas para evitar el error
	m_ResourcesMap.clear();
	m_ResourcesVector.clear();
}

void CCharactersManager::CleanUp( void )
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
bool CCharactersManager::Reload( void )
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
void CCharactersManager::Update( float _ElapsedTime )
{
	assert(m_pPlayer != NULL);

	// Actualitzem el player
	m_pPlayer->Update( _ElapsedTime );
	
	// Comprobamos qué enemigos deben atacar y cuales no
	CalculateEnemyOrderToAttack(m_pPlayer->GetPosition(), 20);

	// Actualitzem l'enemic
	TVectorResources l_EnemyList = GetResourcesVector();
	for ( size_t i = 0; i < l_EnemyList.size(); ++i )
	{
		CProperties * l_pProperties = l_EnemyList[i]->GetProperties();
		if ( l_pProperties->GetActive() )
		{
			CCharacter * l_Character = l_EnemyList[i];
			l_Character->UpdateIA( _ElapsedTime );
			l_Character->Update( _ElapsedTime );
			if ( !l_EnemyList[i]->IsAlive() )
			{
				if ( m_pTargetEnemy == l_EnemyList[i] )
				{
					m_pTargetEnemy = NULL;
				}
				if ( m_pPreviewTargetEnemy == l_EnemyList[i] )
				{
					m_pPreviewTargetEnemy = NULL;
				}
				l_EnemyList[i]->BeDead();

				/*Vect3f l_Pos = l_EnemyList[i]->GetPosition();
				CParticleEmitter *l_Emitter = CORE->GetParticleEmitterManager()->GetResource("DeathExplosion");
				CParticleEmitterInstance *l_Instance = l_Emitter->GetParticleEmitterInstance(l_EnemyList[i]->GetName() + "_Explosions");
				//CORE->GetParticleEmitterManager()->GetResource("DeathExplosion")->GetParticleEmitterInstance(l_EnemyList[i]->GetName() + "_Explosions")->SetPosition(l_Pos);
				//CORE->GetParticleEmitterManager()->GetResource("DeathExplosion")->GetParticleEmitterInstance(l_EnemyList[i]->GetName() + "_Explosions")->EjectParticles();
				l_Instance->SetPosition(l_Pos);
				l_Instance->EjectParticles();*/
			}
		}
	}
	l_EnemyList.clear();

	//SCRIPT->RunCode("collectgarbage(collect)");
	//lua_gc(SCRIPT->GetLuaState(), LUA_GCCOLLECT, 0);

	//Actualiza el billboard del target enemy
	/*if( m_pTargetEnemy != NULL )
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
	}*/
}

//----------------------------------------------------------------------------------------------------
// Render : Renderiza el player i algunos enemigos y sus fustrums, rayos, etc
//----------------------------------------------------------------------------------------------------
void CCharactersManager::Render(CRenderManager *_RM, CFontManager *_FM)
{
	if ( m_pPlayer ) 
	{
		int dy = 50;
		int life = m_pPlayer->GetProperties()->GetLife();
		Vect3f l_Pos = m_pPlayer->GetController()->GetPosition();
		dy += _FM->DrawDefaultText(10, dy, colWHITE, "Life: %d", life);
		dy += _FM->DrawDefaultText(10, dy, colWHITE, "Position: %f, %f, %f", l_Pos.x, l_Pos.y, l_Pos.z);
		dy += _FM->DrawDefaultText(10, dy, colWHITE, "Yaw: %f", m_pPlayer->GetAnimatedModel()->GetYaw() );
	}

	if ( CORE->GetPhysicsManager()->GetDrawFront() )
		DrawFront();

	if ( CORE->GetPhysicsManager()->GetDrawfrustum() )
		Drawfrustum();

	if ( CORE->GetPhysicsManager()->GetDrawNames() )
		DrawNames(_FM);

	if ( CORE->GetPhysicsManager()->GetDrawRays() )
		DrawRay();

	if ( CORE->GetPhysicsManager()->GetRenderPositions() )
		DrawPositions(_FM);

	if ( CORE->GetPhysicsManager()->GetDrawDistancesSPheres() )
		DrawDistancesSpheres();
}

//--------------------------------------------------
//					FUNCTIONS 
//--------------------------------------------------



void CCharactersManager::DrawPositions( CFontManager *_FM )
{
	// Dibuixem posicions de cada enemic
	uint32 l_FileNumber = 85;
	for ( size_t i = 0; i<	m_ResourcesVector.size(); ++i )
	{
		CCharacter* l_Enemy = m_ResourcesVector[i];
		if ( l_Enemy->IsEnable() )
		{
			_FM->DrawDefaultText(10, l_FileNumber, colWHITE, "Position %s: %f, %f, %f", l_Enemy->GetName().c_str(), l_Enemy->GetPosition().x, l_Enemy->GetPosition().y, l_Enemy->GetPosition().z);
			l_FileNumber += 20;
		}
	}
}

void CCharactersManager::DrawDistancesSpheres( void )
{
	Mat44f mat;
	mat.SetIdentity();
	CRenderManager * l_RM = CORE->GetRenderManager();
	
	TVectorResources::iterator l_It = m_ResourcesVector.begin();
	TVectorResources::iterator l_End = m_ResourcesVector.end();

	Vect3f l_FinalPosition;
	Vect3f l_InitialPosition;

	for ( l_It; l_It<l_End; l_It++ )
	{
		if ((*l_It)->GetProperties()->GetActive() )
		{
			// Dibuixem esferes
			mat.Translate((*l_It)->GetSteeringEntity()->GetPosition());
			l_RM->SetTransform(mat);
			l_RM->DrawSphere( (*l_It)->GetSteeringEntity()->GetBoundingRadius(), 10, colMAGENTA );		// Bounding box
			l_RM->DrawSphere( (*l_It)->GetProperties()->GetDetectionDistance(), 10, colCYAN );			// Detection distance
			l_RM->DrawSphere( (*l_It)->GetProperties()->GetAttackDistance(), 10, colRED);				// Impact distance
			l_RM->DrawSphere( (*l_It)->GetProperties()->GetChaseDistance(), 10, colGREEN);				// Chase distance
			l_RM->DrawSphere( (*l_It)->GetProperties()->GetPreparedAttackDistance(), 10, colYELLOW);	// Prepared distance
		}
	}
}

void CCharactersManager::DrawNames( CFontManager *_FM )
{
	Mat44f mat;
	mat.SetIdentity();
	CRenderManager * l_RM = CORE->GetRenderManager();
	l_RM->SetTransform(mat);

	TVectorResources::iterator l_It = m_ResourcesVector.begin();
	TVectorResources::iterator l_End = m_ResourcesVector.end();
	
	Vect3f v;
	v.SetZero();
	for ( l_It; l_It!=l_End; l_It++ )
	{
		std::string s = (*l_It)->GetName().c_str();
		//_FM->DrawDefaultText( (float)(*l_It)->GetPosition().x, (float)(*l_It)->GetPosition().z, colWHITE, "POEPE" );
		
	}
}

// Dibuixem els fustrums dels enemics i del player
void CCharactersManager::Drawfrustum( void )
{
	Mat44f mat;
	mat.SetIdentity();
	CRenderManager * l_RM = CORE->GetRenderManager();
	
	TVectorResources::iterator l_It = m_ResourcesVector.begin();
	TVectorResources::iterator l_End = m_ResourcesVector.end();

	Vect3f l_FinalPosition;
	Vect3f l_InitialPosition;

	for ( l_It; l_It<l_End; l_It++ )
	{
		if ((*l_It)->GetProperties()->GetActive() )
		{
			l_InitialPosition = (*l_It)->GetSteeringEntity()->GetInitialPositionToThrowRay();
			l_FinalPosition = (*l_It)->GetSteeringEntity()->GetFinalPositionToThrowRay(0.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colMAGENTA );
	
			l_FinalPosition = (*l_It)->GetSteeringEntity()->GetFinalPositionToThrowRay(45.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colMAGENTA );

			l_FinalPosition = (*l_It)->GetSteeringEntity()->GetFinalPositionToThrowRay(-45.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colMAGENTA );
		}
	}
	
}

// Dibuixem la velocitat/direcció dels enemics 
void CCharactersManager::DrawVelocity( CCharacter * _Character, CRenderManager * _RM )
{
	/*Vect3f l_Pos; 
	Vect3f l_Velocity;
	float l_Radi = _Character->GetProperties()->GetBoundingRadious();
	
	l_Pos = _Character->GetPosition(); 
	l_Velocity = _Character->GetSteeringEntity()->GetVelocity();
	l_Velocity = l_Velocity.RotateY(mathUtils::PiTimes(1.f));
	_Character->GetBehaviors()->GetSeek()->SetTarget(l_Velocity);
	
	float l_Height = _Character->GetProperties()->GetHeightController();
	l_Pos = Vect3f ( l_Pos.x + l_Velocity.x * l_Radi , l_Velocity.y + l_Height, l_Velocity.z + l_Velocity.z * l_Radi );
	
	Vect3f l_InitialPosition = _Character->GetSteeringEntity()->GetInitialPositionToThrowRay();
	Vect3f l_FinalPosition = _Character->GetSteeringEntity()->GetFinalPositionToThrowRay(0.f);
	_RM->DrawLine( l_InitialPosition, l_FinalPosition, colMAGENTA );*/
}


// Dibuixem el front dels enemics i del player
void CCharactersManager::DrawFront( void )
{
	Mat44f mat;
	mat.SetIdentity();
	CRenderManager * l_RM = CORE->GetRenderManager();
	l_RM->SetTransform(mat);

	TVectorResources::iterator l_It = m_ResourcesVector.begin();
	TVectorResources::iterator l_End = m_ResourcesVector.end();
	
	Vect3f l_FinalPosition;
	Vect3f l_InitialPosition;

	l_FinalPosition .SetZero();
	for ( l_It; l_It!=l_End; l_It++ )
	{
		if ((*l_It)->GetProperties()->GetActive() )
		{
			/*mat.Translate((*l_It)->GetSteeringEntity()->GetPosition());
			l_RM->SetTransform(mat);
*/
			l_InitialPosition = (*l_It)->GetSteeringEntity()->GetInitialPositionToThrowRay();
			l_FinalPosition  = Vect3f ( (*l_It)->GetPosition().x + (*l_It)->GetFront().x, (*l_It)->GetPosition().y + (*l_It)->GetProperties()->GetHeightController(), (*l_It)->GetPosition().z + (*l_It)->GetFront().z);
			//l_FinalPosition = (*l_It)->GetSteeringEntity()->GetFinalPositionToThrowRay(0.f);
			
		/*	mat.Translate(l_InitialPosition);
			l_RM->SetTransform(mat);*/

			l_RM->DrawLine( l_InitialPosition, l_FinalPosition );
		}
	}
	
	// Ara el player
	if ( m_pPlayer )
	{
		/*mat.Translate(m_pPlayer->GetPosition());
		l_RM->SetTransform(mat);*/

		l_FinalPosition.SetZero();
		l_FinalPosition = Vect3f ( m_pPlayer->GetPosition().x + m_pPlayer->GetSteeringEntity()->GetHeading().x, m_pPlayer->GetPosition().y + m_pPlayer->GetProperties()->GetHeightController(), m_pPlayer->GetPosition().z + m_pPlayer->GetSteeringEntity()->GetHeading().z);
		l_RM->DrawLine( Vect3f( m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y + m_pPlayer->GetProperties()->GetHeightController(), m_pPlayer->GetPosition().z ) , l_FinalPosition );
	}
}

void CCharactersManager::DrawRay( void )
{
	Mat44f mat;
	mat.SetIdentity();
	CRenderManager * l_RM = CORE->GetRenderManager();
	l_RM->SetTransform(mat);
	
	Vect3f l_FinalPosition;
	Vect3f l_InitialPosition;
	Vect3f l_Front;

	float l_ObstacleDistanceRay = CORE->GetSteeringBehaviourSettingsManager()->GetCollisionDetectionFeelerLength();
	float l_RotacioRaigs = 45.f;

	CCharacter * l_Enemy = GetResource("enemy22");
	if (l_Enemy)
	{
		if ( l_Enemy->GetProperties()->GetActive() )
		{
			for ( float i = -45.f; i <= 45.f; i += l_RotacioRaigs ) 
			{
				l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
				l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(i);
				l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colMAGENTA );
			}

			/*l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(0.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colMAGENTA );

			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colMAGENTA );

			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(-30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colMAGENTA );*/
		}
	}

	l_Enemy = GetResource("enemy23");
	if (l_Enemy)
	{ 
		if ( l_Enemy->GetProperties()->GetActive() )
		{
			for ( float i = -45.f; i <= 45.f; i += l_RotacioRaigs ) 
			{
				l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
				l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(i);
				l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colGREEN );
			}

			/*l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(0.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colGREEN );
	
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colGREEN );

			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(-30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colGREEN );*/
		}
	}

	l_Enemy = GetResource("enemy24");
	if (l_Enemy)
	{ 
		if ( l_Enemy->GetProperties()->GetActive() )
		{
			/*l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(0.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colRED );
	
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colRED );

			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(-30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colRED );*/

			for ( float i = -45.f; i <= 45.f; i += l_RotacioRaigs ) 
			{
				l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
				l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(i);
				l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colRED );
			}
		}
	}

	l_Enemy = GetResource("enemy25");
	if (l_Enemy)
	{
		if ( l_Enemy->GetProperties()->GetActive() )
		{
			/*l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(0.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colYELLOW );
	
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colYELLOW );

			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(-30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colYELLOW );	*/	
		
			for ( float i = -45.f; i <= 45.f; i += l_RotacioRaigs ) 
			{
				l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
				l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(i);
				l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colYELLOW );
			}
		}
	}

	l_Enemy = GetResource("enemy26");
	if (l_Enemy)
	{
		if ( l_Enemy->GetProperties()->GetActive() )
		{
			/*l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(0.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colCYAN );
	
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colCYAN );

			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(-30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colCYAN );		*/
			for ( float i = -45.f; i <= 45.f; i += l_RotacioRaigs ) 
			{
				l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
				l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(i);
				l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colCYAN );
			}
		}
	}
	
	l_Enemy = GetResource("enemy27");
	if (l_Enemy)
	{
		if ( l_Enemy->GetProperties()->GetActive() )
		{
			/*l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(0.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colBLUE );
	
			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colBLUE );

			l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(-30.f);
			l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colBLUE );	*/
			for ( float i = -45.f; i <= 45.f; i += l_RotacioRaigs ) 
			{
				l_InitialPosition = l_Enemy->GetSteeringEntity()->GetInitialPositionToThrowRay();
				l_FinalPosition = l_Enemy->GetSteeringEntity()->GetFinalPositionToThrowRay(i);
				l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colBLUE );
			}
		}
	}
	
	//Vect3f l_InitialPosition = l_Enemy->GetPosition();
	//l_InitialPosition.y += 1;
	//
	//Vect3f l_FinalPosition = l_Enemy->GetPosition() + l_Enemy->GetFront();
	////l_FinalPosition.Normalize();
	//l_FinalPosition.y = l_InitialPosition.y;

	//Vect3f l_FinalPosition = Vect3f ( l_Enemy->GetPosition().x + l_Enemy->GetFront().x, l_Enemy->GetPosition().y + l_Enemy->GetProperties()->GetHeightController(), l_Enemy->GetPosition().z + l_Enemy->GetFront().z) * 2;
	//l_RM->DrawLine( Vect3f ( l_Enemy->GetPosition().x, l_Enemy->GetPosition().y + l_Enemy->GetProperties()->GetHeightController() , l_Enemy->GetPosition().z ), l_FinalPosition );

	//l_FinalPosition.Normalize();
	//l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colMAGENTA );
}

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

		// Creamos enemigos adicionales para los ataques del lobo. Así los creamos en tiempo de carga i no en tiempo de ejecución
		AssignDynamicEnemiesToHelp();
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

	// Jordi - 27/08/2012 : Ya no se usa. Pero quizás se use en un futuro.
	return l_IsOk;

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
		m_pPlayer = new CPlayer();
		//m_pPlayer = call_function<CCharacter*>(SCRIPT->GetLuaState(), "CPlayer", 0)[adopt(result)];
	}

	// Obtenemos las propiedades del player
	CProperties* l_PlayerProperties = m_pPropertiesManager->LoadPlayerProperties( _Node ); 
	if ( l_PlayerProperties )
	{
		//asignamos las propiedades al player
		m_pPlayer->SetProperties( l_PlayerProperties );
		
		// Inicializamos el player, sus estados, mayas animadas...
		m_pPlayer->Initialize( l_PlayerProperties->GetName(), l_PlayerProperties->GetCore(), m_pPlayer->GetProperties()->GetPosition(), ::ECG_PLAYER );
		l_IsOk &= m_pPlayer->InitializeAI();
		l_IsOk &= m_pPlayer->Init();		// Llamada a Lua
		ENTMGR->RegisterEntity(m_pPlayer);
		//m_pPlayer->SetEnable ( l_PlayerProperties->GetActive() );
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
	
	bool l_IsOk = true;
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

					if ( !l_Character )
					{

						if ( l_EnemyProperties->GetCore() == "lobo" )
						{
							/*std::string l_LUAClass = "CWolf";
							l_Character = call_function<CCharacter*>(SCRIPT->GetLuaState(), l_LUAClass.c_str(), l_NextIDValid, l_EnemyProperties->GetName() )[adopt(result)];*/
							std::string l_LUAClass = "CWolf";
							l_Character = new CWolf(l_NextIDValid, l_EnemyProperties->GetName());
							l_Character->SetCharacterType(WOLF);
						}
						if ( l_EnemyProperties->GetCore() == "ciervo" )
						{
							std::string l_LUAClass = "CDeer";
							l_Character = new CDeer(l_NextIDValid, l_EnemyProperties->GetName());
							l_Character->SetCharacterType(DEER);
						}
						if ( l_EnemyProperties->GetCore() == "conejo" ) 
						{
							std::string l_LUAClass = "CRabbit";
							//boost::ref(val));
							//l_Character = call_function<CCharacter*>(SCRIPT->GetLuaState(), l_LUAClass.c_str(), l_NextIDValid, l_EnemyProperties->GetName() )[adopt(result)];
							l_Character = new CRabbit(l_NextIDValid, l_EnemyProperties->GetName());
							l_Character->SetCharacterType(RABBIT);
						}
					}

					// Asignamos las propiedades
					CProperties *properties = new CProperties();
					memcpy(properties, l_EnemyProperties, sizeof(CProperties));
					l_Character->SetProperties(properties);
		
					// Inicializamos el player, sus estados, mayas animadas...
					l_IsOk = l_Character->Initialize( l_EnemyProperties->GetName(), l_EnemyProperties->GetCore(), l_Character->GetProperties()->GetPosition(), ::ECG_ENEMY );
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
			else if (  l_Type == "number_begin_dynamic_enemies" )
			{
				m_NumberFirstDynamicEnemy = l_EnemiesNode(i).GetIntKeyword("number_begin_dynamic_enemies");
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

CCharacter* CCharactersManager::GetCharacterById( int _Id )
{
	CCharacter * l_Character = dynamic_cast<CCharacter*> (ENTMGR->GetEntityFromID(_Id));
	return l_Character;
}

void CCharactersManager::AddEnemy ( CCharacter *_pEnemy )
{
	AddResource ( _pEnemy->GetName(), _pEnemy );
}

// ---------------------------------------------------------------------------------------------------------
// CreateRandomEnemy: Crea enemigo de forma aleatoria entre unas coordenadas especificas y lo añade al mapa
// ---------------------------------------------------------------------------------------------------------
CCharacter * CCharactersManager::CreateEnemy( const Vect3f &_Position )
{
	bool l_IsOk = false;

	std::stringstream out;
		out << "enemy";
		out << ( CBaseGameEntity::GetNextValidID() );
	
	std::string l_Name = out.str();;
	if ( GetResource(l_Name) != 0 )
	{
		return GetResource(l_Name);
	}
	
	int l_Rand = BoostRandomHelper::GetInt(2,3);

	eCharacterTypes l_TipoEnemigo = static_cast<eCharacterTypes> (l_Rand);
	CProperties* l_EnemyProperties = NULL;
	if ( l_Rand == ::DEER ) 
	{
		l_EnemyProperties = m_pPropertiesManager->GetResource("ciervo");
		l_EnemyProperties->SetCore( "ciervo" );
	}
	else if ( l_Rand == ::RABBIT ) 
	{
		l_EnemyProperties = m_pPropertiesManager->GetResource("conejo");
		l_EnemyProperties->SetCore( "conejo" );
	}

	if ( l_EnemyProperties )
	{
		l_EnemyProperties->SetName(l_Name);
		l_EnemyProperties->SetAnimationInstance(l_Name);
		l_EnemyProperties->SetPosition(_Position);
		/*l_EnemyProperties->SetActive(false);
		l_EnemyProperties->SetVisible(true);
		l_EnemyProperties->SetLocked(false);*/
		
		CCharacter* l_Character = GetResource(l_Name);
		if ( !l_Character )
		{
			if ( l_EnemyProperties->GetCore() == "ciervo" )
			{
				std::string l_LUAClass = "CDeer";
				l_Character = new CDeer(CBaseGameEntity::GetNextValidID(), l_Name);
				l_Character->SetCharacterType(DEER);
			}
			if ( l_EnemyProperties->GetCore() == "conejo" ) 
			{
				std::string l_LUAClass = "CRabbit";
				l_Character = new CRabbit(CBaseGameEntity::GetNextValidID(), l_Name);
				l_Character->SetCharacterType(RABBIT);
			}
		}
		
		// Asignamos las propiedades
		CProperties *l_Properties = new CProperties();
		memcpy(l_Properties, l_EnemyProperties, sizeof(CProperties));
		l_Character->SetProperties(l_Properties);
		
		// Inicializamos el player, sus estados, mayas animadas...
		//l_Character->SetID(CBaseGameEntity::GetNextValidID());
		l_IsOk = l_Character->Initialize( l_EnemyProperties->GetName(), l_EnemyProperties->GetCore(), l_Character->GetProperties()->GetPosition(), ::ECG_ENEMY );
		l_IsOk &= l_Character->InitializeAI();
		l_IsOk &= l_Character->Init();		// Llamada a Lua

		AddEnemy( l_Character );			// La meto dentro de la lista
		
		//l_NextIDValid += 1;					// Pròxim ID vàlid
		ENTMGR->RegisterEntity(l_Character);

		return l_Character;
	}
	
	return NULL;
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

	int mask = 1 << ECG_PLAYER;
	mask |= 1 << ECG_DYNAMIC_OBJECTS;
	mask |= 1 << ECG_ESCENE;
	mask |= 1 << ECG_ENEMY;

	CPhysicUserData *userdata = CORE->GetPhysicsManager()->RaycastClosestActor(l_Pos, l_Dir, mask, l_Info);
	return userdata;
}

CCharacter* CCharactersManager::IsPlayerNearEnemy(float distance)
{
	CCharacter *l_NearestEnemy = NULL;
	float l_NearestDistance = 100000.f;
	Vect3f l_Pos = m_pPlayer->GetPosition();

	TVectorResources l_EnemyList = GetResourcesVector();

	if( l_EnemyList.size() == 0 )
	{
		return NULL;
	}

	TVectorResources::iterator l_It = l_EnemyList.begin();
	TVectorResources::iterator l_End = l_EnemyList.end();
	for(; l_It != l_End; ++l_It)
	{
		if( (*l_It)->IsAlive() && (*l_It)->IsEnable() )
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

CCharacter* CCharactersManager::SearchTargetEnemy(float _Distance, float _AngleVisible, const Vect3f &_Front)
{
	assert( _Distance > 0.f );
	assert( _AngleVisible > 0.f );

	CCharacter *l_NearestEnemy = NULL;
	bool l_IsEnemyFound = false;
	float l_NearestDistance = 0.f;
	Vect3f l_Pos = m_pPlayer->GetPosition();

	TVectorResources l_EnemyList = GetResourcesVector();

	uint16 l_iNumEnemies = l_EnemyList.size();
  	if( l_iNumEnemies < 1 )
	{
		return NULL;
	}
	else if( l_iNumEnemies > 0 )
	{
		//Inicializamos los datos de enemigo para comparar
		l_NearestEnemy = l_EnemyList[0];
		l_NearestDistance = l_Pos.Distance(l_NearestEnemy->GetPosition());

		if( l_NearestDistance <= _Distance )
		{
			l_IsEnemyFound = true;
		}

		TVectorResources::iterator l_It = l_EnemyList.begin()+1;
		TVectorResources::iterator l_End = l_EnemyList.end();
		for(; l_It != l_End; ++l_It)
		{
			if( (*l_It)->IsAlive() && (*l_It)->GetProperties()->GetActive() )
			{
				//Mira si el enemigo está más cerca que el resto del player
				float l_CurrentDistance = l_Pos.Distance( (*l_It)->GetPosition() );
				if( l_CurrentDistance <= _Distance && l_CurrentDistance <= l_NearestDistance )
				{
					//Mira si el enemigo que está cerca está visible en un ángulo respecto el player
					if( EnemyIsVisibleInAngle( (*l_It), _AngleVisible, _Front) ) 
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

	return NULL;
}

bool CCharactersManager::EnemyIsVisibleInAngle(CCharacter *_Enemy, float _Angle, const Vect3f &_Front)
{
	assert( m_pPlayer );
	assert( _Enemy );
	assert( _Angle > 0.f );
	//Cogemos la dirección del player
	//Vect2f dir = Vect2f( mathUtils::Abs(mathUtils::Cos(m_pPlayer->GetYaw())), mathUtils::Abs(mathUtils::Sin(m_pPlayer->GetYaw() ) ) );
	//Vect3f l_DirPlayer = Vect3f( dir.x, 0.0f, dir.y);
	//Vect3f l_DirPlayer = static_cast<CGameProcess*>(CORE->GetProcess())->GetPlayerCamera()->GetDirection();
	Vect3f l_DirPlayer = _Front;
	l_DirPlayer.y = 0.f;

	//Calculamos el vector entre el player y el enemigo
	Vect3f l_DirEnemy = _Enemy->GetPosition() - m_pPlayer->GetPosition();
	l_DirEnemy.y = 0.f;
	l_DirEnemy.Normalize(1.f);

	//Calculamos el ángulo entre los dos vectores
	//float l_Angle = l_DirPlayer.AngleWithVector(l_DirEnemy);		// Jordi : Jo tinc això...
	float l_Angle = l_DirPlayer.Dot(l_DirEnemy);
	float l_AngleDegrees = mathUtils::Rad2Deg(l_Angle);
	l_Angle = mathUtils::ACos(l_Angle);

	if( l_Angle > _Angle )
	{
		return false;
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------
// --- Me indica si el enemigo se visualiza dentro la cámara o dentro de los grados en radianes
// -----------------------------------------------------------------------------------------------------
bool CCharactersManager::IsEnemyVisibleInAngleFromCamera(CCharacter *_Enemy, float _Angle )
{
	assert( m_pPlayer );
	assert( _Enemy );
	assert( _Angle > 0.f );
	
	//Cogemos la dirección de la cámara
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	Vect3f	l_DirCamera = l_Process->GetPlayerCamera()->GetDirection();
	l_DirCamera.y = 0.f;
	l_DirCamera.Normalize(1.f);

	//Calculamos el vector entre el player y el enemigo
	Vect3f l_DirEnemy = _Enemy->GetPosition() - l_Process->GetPlayerCamera()->GetPosition();
	l_DirEnemy.y = 0.f;
	l_DirEnemy.Normalize(1.f);

	//Calculamos el ángulo entre los dos vectores
	//float l_Angle = l_DirPlayer.AngleWithVector(l_DirEnemy);		// Jordi : Jo tinc això...
	float l_Angle = l_DirCamera.Dot(l_DirEnemy);
	float l_AngleDegrees = mathUtils::Rad2Deg(l_Angle);
	l_Angle = mathUtils::ACos(l_Angle);

	if( l_Angle > _Angle )
	{
		return false;
	}

	return true;
}


void CCharactersManager::CalculateEnemyOrderToAttack( const Vect3f & _Position, float _ViewDistance )
{
	// Obtengo el vector de enemigos
	TMapResources l_EnemyList = GetResourcesMap();
	if( l_EnemyList.size() == 0 )
	{
		return;
	}

	int l_Mask = 1 << ECG_ENEMY;
	//std::multimap<std::string, >	l_OrderedMap;
	std::vector<CCharacter *>	l_EnemyByDistance;

	l_EnemyByDistance.clear();
		
	// Calculamos los vecinos a una posición 
	CORE->GetPhysicsManager()->OverlapSphereActor( _ViewDistance, _Position, m_UserDatas, l_Mask );

	if ( m_UserDatas.size() > 0 )
	{
		CCharacter * l_Character = GetResource(m_UserDatas[0]->GetName());
		l_EnemyByDistance.push_back(l_Character);

		for ( size_t i = 1; i < m_UserDatas.size(); i++)
		{
			bool l_Trobat = false;

			CCharacter * l_Character = GetResource(m_UserDatas[i]->GetName());
			// Si encontrado
			if ( l_Character && l_Character->IsAlive() ) 
			{
				// Distancia del elemento q tratamos
				float l_CurrentDistance = m_UserDatas[i]->GetSteeringEntity()->GetPosition().Distance(m_pPlayer->GetPosition());

				std::vector<CCharacter*>::iterator l_Iter;

				for ( l_Iter = l_EnemyByDistance.begin(); l_Iter != l_EnemyByDistance.end(); l_Iter++ )
				{
					// Si la distancia es menor que la existente del elemento ya guardado le insertamos el nuevo en esta posición
					float l_Dist = ( (*l_Iter)->GetSteeringEntity()->GetPosition().Distance(m_pPlayer->GetPosition()));
					if ( l_CurrentDistance <= ( (*l_Iter)->GetSteeringEntity()->GetPosition().Distance(m_pPlayer->GetPosition())) )
					{
						l_Character = GetResource(m_UserDatas[i]->GetName());
						l_EnemyByDistance.insert(l_Iter, l_Character);
						l_Trobat = true;
						break;
					}
				}

				if ( !l_Trobat) 
				{
					l_EnemyByDistance.push_back(l_Character);
				}
			}
			else if ( !l_Character->IsAlive() )
			{
				l_Character->SetAvalaibleToAttack(false);
			}
		}	// For user data

		// Esto activa para el ataque los N primeros, los restantes se mantendran en Idle
		int l_NumPlayersAttackedSameTime = CORE->GetSteeringBehaviourSettingsManager()->GetNumberEnemiesToAttackAtSameTime();
		for ( int i = 0; i < static_cast<int>(l_EnemyByDistance.size()); i++ )
		{
			if ( i >= l_NumPlayersAttackedSameTime ) 
			{
				l_EnemyByDistance[i]->SetAvalaibleToAttack(false);
			}
			else
			{
				l_EnemyByDistance[i]->SetAvalaibleToAttack(true);
			}
		}

	}	// for if exist user data
}

CCharacter* CCharactersManager::GetPlayerAngleCorrection( float _fDistance, float _fMinAngle, float &fAngle_ )
{
	TVectorResources l_EnemyList		= GetResourcesVector();
	TVectorResources::iterator l_It		= l_EnemyList.begin();
	TVectorResources::iterator l_End	= l_EnemyList.end();

	Vect3f l_PlayerFront	= m_pPlayer->GetAnimatedModel()->GetFront();
	Vect3f l_PlayerPos		= m_pPlayer->GetPosition();

	float l_fMinimumAngle	= FLOAT_PI_VALUE;
	CCharacter *l_pEnemy	= NULL;

	//Miramos cada enemigo la distancia y el ángulo respecto el player
	for(; l_It != l_End; ++l_It)
	{
		if( (*l_It)->IsAlive() && (*l_It)->IsEnable() )
		{
			//Miramos la distancia respecto del player
			Vect3f l_EnemyPos	= (*l_It)->GetPosition();	
			float l_fEnemyDist	= l_EnemyPos.Distance( l_PlayerPos );

			if( l_fEnemyDist <= _fDistance )
			{
				//Miramos el ángulo respecto del front del player
				Vect3f l_EnemyDir = l_EnemyPos - l_PlayerPos;
				l_EnemyDir.Normalize();
				l_EnemyDir.y = 0.f;

				float l_fAngle	= l_EnemyDir.Dot( l_PlayerFront );
				l_fAngle		= Helper::LimitValue(l_fAngle, -1.f, 1.f);
				l_fAngle		= mathUtils::ACos<float>( l_fAngle );

				if( l_fAngle <= _fMinAngle && l_fAngle <= l_fMinimumAngle )
				{
					l_fMinimumAngle = l_fAngle;
					l_pEnemy = (*l_It);
				}
			}
		}
	}

	fAngle_ = (l_fMinimumAngle != FLOAT_PI_VALUE) ? l_fMinimumAngle : 0.f;
	return l_pEnemy;
}

// Esta función era para crear dinámicamente enemigos pero después en el reload peta incomprensiblemente. 
// Ahora se usa para asignar el nº del enemigo donde empiezan los enemigos llamados por el lobo.
void CCharactersManager::AssignDynamicEnemiesToHelp(void)
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	CWolf * l_Lobo = dynamic_cast<CWolf *> (GetResource("enemy1"));
	Vect3f l_InitialPosition = l_Lobo->GetPosition();			// Almaceno la posición del wolf que hará de lider a partir de la qual haré la formación
	
	// Almaceno el índice a partir del cual tenemos enemigos dinámicos en la lista del manager para el estado howl del lobo
	CWolfHowlEnemiesState *l_HowlState = l_Lobo->GetHowlEnemiesState();
	
	// Asigno a pelo!!!
	uint32 i = static_cast<uint32> (m_NumberFirstDynamicEnemy);
	l_HowlState->SetCurrentDynamicEnemyIndex(i);

	//l_InitialPosition.y = 0.f;
	//uint16 l_TotalEnemies = 20;								// Meto 20 enemigos más. Creo que suficientes
	//for ( float i = 1; i < l_TotalEnemies; i ++ ) 
	//{
	//	CreateEnemy(l_InitialPosition);
	//	//l_Process->GetCharactersManager()->SaveDynamicCharacterCreated(l_Character->GetName());
	//}
}