#include <string>

#include "SteeringBehaviorsSeetingsManager.h"
#include "Scripting\ScriptManager.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "base.h"
#include "Core.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------

CSteeringBehaviorsSeetingsManager::CSteeringBehaviorsSeetingsManager( void )
	: m_szFilename	( "" )
{
}

CSteeringBehaviorsSeetingsManager::~CSteeringBehaviorsSeetingsManager( void )
{
	Destroy();
}


// -----------------------------------------
//			 MÈTODES PRINCIPALS
// -----------------------------------------

void CSteeringBehaviorsSeetingsManager::Init()
{
}

void CSteeringBehaviorsSeetingsManager::Destroy ( void )
{
}

bool CSteeringBehaviorsSeetingsManager::Load ( const std::string &_Filename )
{
	m_szFilename = _Filename;
	return LoadXML();
}

bool CSteeringBehaviorsSeetingsManager::Reload ( void )
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CSteeringBehaviorsSeetingsManager::Reload--> Reloading steering behaviors properties." );
	Destroy();
	return LoadXML();
}

// Actualitzem totes les instancies d'emissors
void CSteeringBehaviorsSeetingsManager::Update ( float _ElapsedTime )
{
	return;
}

void CSteeringBehaviorsSeetingsManager::Render ( CRenderManager *_RM )
{
	return;
}

// -----------------------------------------
//				MÈTODES 
// -----------------------------------------

// Carreguem el fitxer d'emissors de partícules
bool CSteeringBehaviorsSeetingsManager::LoadXML ( void )
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CSteeringBehaviorsSeetingsManager::LoadXML --> Loading steering behaviors properties." );
	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_szFilename.c_str ( ) ) )
	{
		std::string msg_error = "CSteeringBehaviorsSeetingsManager::LoadXML->Error when trying to load the steering behaviors properties file: " + m_szFilename;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	CXMLTreeNode l_XMLPropertiesNode = newFile ["steering_properties"];
	if ( l_XMLPropertiesNode.Exists ( ) )
	{
		uint16 l_TotalNodes = l_XMLPropertiesNode.GetNumChildren ();
		// Recorro las propiedades
		for ( uint16 i = 0; i < l_TotalNodes; ++i )
		{
			std::string l_PropertyField = l_XMLPropertiesNode(i).GetName();
			
			if( l_PropertyField == "ObstacleWallDetectionFeelerLength" )
			{
				m_ObstacleWallDetectionFeelerLength = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "ObstacleWallDetectionFeelerLength", 2.f ) );
			}
			else if ( l_PropertyField == "ObstacleWallDetectionFeelerAngle" )
			{
				m_ObstacleWallDetectionFeelerAngle = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "ObstacleWallDetectionFeelerAngle", 60.f ) );
			}
			else if ( l_PropertyField == "DetectionFeelerAngleIncrement" )
			{
				m_DetectionFeelerAngleIncrement = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "DetectionFeelerAngleIncrement", 2.f ) );
			}
			else if ( l_PropertyField == "CollisionDetectionFeelerLength" )
			{
				m_CollisionDetectionFeelerLength = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "CollisionDetectionFeelerLength", 40.f ) );
			}
			else if ( l_PropertyField == "CollisionDetectionFeelerAngle" )
			{
				m_CollisionDetectionFeelerAngle = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "CollisionDetectionFeelerAngle", 60.f ) );
			}
			else if ( l_PropertyField == "SeparationDetectionFeelerLength" )
			{
				m_SeparationDetectionFeelerLength = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "SeparationDetectionFeelerLength", 40.f ) );
			}
			else if ( l_PropertyField == "SeparationDecayCoefficient" )
			{
				m_SeparationDecayCoefficient = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "SeparationDecayCoefficient", 40.f ) );
			}
			else if ( l_PropertyField == "ArriveDeceleration" )
			{
				std::string l_ArriveDecelaration = static_cast<std::string> ( l_XMLPropertiesNode(i).GetPszKeyword( "ArriveDeceleration", "normal" ) );

				if ( l_ArriveDecelaration == "slow" )
					m_ArriveDecelaration = ::slow;
				if ( l_ArriveDecelaration == "normal" )
					m_ArriveDecelaration = ::normal;
				if ( l_ArriveDecelaration == "fast" )
					m_ArriveDecelaration = ::fast;
			}
			else if ( l_PropertyField == "ArriveDecelerationDistance" )
			{
				m_ArriveDecelarationDistance = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "ArriveDecelerationDistance", 2.f ) );
			}
			else if ( l_PropertyField == "DecelerationTweaker" )
			{
				m_ArriveDecelarationTweaker = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "DecelerationTweaker", 2.f ) );
			}
			else if ( l_PropertyField == "flockingNeightbourRadius" )
			{
				m_NeightbourRadius = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "flockingNeightbourRadius", 2.f ) );
			}
			
			else if ( l_PropertyField == "WanderRefreshRate" )
			{
				m_WanderRefreshRate = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "WanderRefreshRate", 0.5f ) );
			}
			else if ( l_PropertyField == "WanderMinimumDistance" )
			{
				m_WanderMinimumDistance = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "WanderMinimumDistance", 0.5f ) );
			}
			else if ( l_PropertyField == "WanderDistance" )
			{
				m_WanderDistance = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "WanderDistance", 2.f ) );
			}
			else if ( l_PropertyField == "WanderRadius" )
			{
				m_WanderRadius = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "WanderRadius", 2.f ) );
			}
			else if ( l_PropertyField == "SeparationWeight" )
			{
				m_SeparationWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "SeparationWeight", 1.f ) );
			}
			else if ( l_PropertyField == "AlignmentWeight" )
			{
				m_AlignmentWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "AlignmentWeight", 1.f ) );
			}
			else if ( l_PropertyField == "CohesionWeight" )
			{
				m_CohesionWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "CohesionWeight", 2.f ) );
			}
			else if ( l_PropertyField == "CollisionAvoidanceWeight" )
			{
				m_CollisionAvoidanceWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "CollisionAvoidanceWeight", 10.f ) );
			}
			else if ( l_PropertyField == "ObstacleWallAvoidanceWeight" )
			{
				m_ObstacleObstacleWallAvoidanceWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "ObstacleWallAvoidanceWeight", 10.f ) );
			}
			else if ( l_PropertyField == "WanderWeight" )
			{
				m_WanderWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "WanderWeight", 1.f ) );
			}
			else if ( l_PropertyField == "SeekWeight" )
			{
				m_SeekWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "SeekWeight", 1.f ) );
			}
			else if ( l_PropertyField == "FleeWeight" )
			{
				m_FleeWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "FleeWeight", 1.f ) );
			}
			else if ( l_PropertyField == "ArriveWeight" )
			{
				m_ArriveWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "ArriveWeight", 1.f ) );
			}
			else if ( l_PropertyField == "PursuitWeight" )
			{
				m_PursuitWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "PursuitWeight", 1.f ) );
			}
			else if ( l_PropertyField == "OffsetPursuitWeight" )
			{
				m_OffsetPursuitWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "OffsetPursuitWeight", 1.f ) );
			}
			else if ( l_PropertyField == "InterposeWeight" )
			{
				m_InterposeWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "InterposeWeight", 1.f ) );
			}
			else if ( l_PropertyField == "HideWeight" )
			{
				m_HideWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "HideWeight", 1.f ) );
			}
			else if ( l_PropertyField == "EvadeWeight" )
			{
				m_EvadeWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "EvadeWeight", 0.01f ) );
			}
			else if ( l_PropertyField == "FollowPathWeight" )
			{
				m_FollowPathWeight = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "FollowPathWeight", 0.05f ) );
			}
			else if ( l_PropertyField == "NumberEnemiesCanAttack" )
			{
				m_NumberEnemiesToAttackAtSameTime = static_cast<int> ( l_XMLPropertiesNode(i).GetIntKeyword( "NumberEnemiesCanAttack", 3 ) );
			}
			else if ( l_PropertyField == "MinNumberEnemiesCanHelp" )
			{
				m_MinNumberEnemiesCanHelp = static_cast<int> ( l_XMLPropertiesNode(i).GetIntKeyword( "MinNumberEnemiesCanHelp", 2 ) );
			}
			else if ( l_PropertyField == "MaxNumberEnemiesCanHelp" )
			{
				m_MaxNumberEnemiesCanHelp = static_cast<int> ( l_XMLPropertiesNode(i).GetIntKeyword( "MaxNumberEnemiesCanHelp", 5 ) );
			}
			else if ( l_PropertyField == "CamaraRangeAngleForPrepared" )
			{
				m_CamaraRangeAngleForPrepared = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "CamaraRangeAngleForPrepared", 120 ) );
			}
			else if ( l_PropertyField == "CamaraRangeAngleForAttack" )
			{
				m_CamaraRangeAngleForAttack = static_cast<float> ( l_XMLPropertiesNode(i).GetFloatKeyword( "CamaraRangeAngleForAttack", 120 ) );
			}
			else if ( l_PropertyField != "comment" ) 
			{
				std::string msg_error = "CSteeringBehaviorsSeetingsManager::LoadXML->Error when trying to load a node : " + l_PropertyField + " from file: " + m_szFilename;
				LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			}
		}
	}
	return true;
}

// -----------------------------------------
//				 PROPIETATS 
// -----------------------------------------
