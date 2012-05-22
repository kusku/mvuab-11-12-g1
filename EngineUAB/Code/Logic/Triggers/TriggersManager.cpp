#include <string>
#include <map>

#include "TriggersManager.h"
#include "PhysicsManager.h"
#include "PhysicUserData.h"
#include "PhysicActor.h"

#include "Utils\MapManager.h"
#include "Scripting\ScriptManager.h"

#include "Math\Vector3.h"
#include "Math\Color.h"

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

CTriggersManager::CTriggersManager( void )
	: m_szFilename	( "" )
{
}

CTriggersManager::~CTriggersManager( void )
{
	Destroy();
}


// -----------------------------------------
//			 MÈTODES PRINCIPALS
// -----------------------------------------

void CTriggersManager::Init()
{
	CORE->GetPhysicsManager()->SetTriggerReport ( this );
}

void CTriggersManager::Destroy ( void )
{
	std::map<std::string, TriggerInstance>::iterator l_It =	m_TriggersMap.begin();
	std::map<std::string, TriggerInstance>::iterator l_End = m_TriggersMap.end();
	
	// Eliminem els triggers que ha creat el manager
	for ( l_It ; l_It != l_End; l_It++ )
	{
		CPhysicActor *l_pActor = l_It->second.pTriggerActor;
		CORE->GetPhysicsManager()->ReleasePhysicActor ( l_pActor );
		CPhysicUserData* l_Data = l_pActor->GetUserData ();

		CHECKED_DELETE ( l_Data );
		CHECKED_DELETE ( l_pActor );
	}

	m_TriggersMap.clear();
}

bool CTriggersManager::Load ( const std::string &_Filename )
{
	m_szFilename = _Filename;
	return LoadXML();
}

bool CTriggersManager::Reload ( void )
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CTriggersManager::Reload--> Reloading triggers system." );
	Destroy();
	return LoadXML();
}

// Actualitzem totes les instancies d'emissors
void CTriggersManager::Update ( float _ElapsedTime )
{
	return;
}

void CTriggersManager::Render ( CRenderManager *_RM )
{
	return;
}

// -----------------------------------------
//				MÈTODES 
// -----------------------------------------

// Carreguem el fitxer d'emissors de partícules
bool CTriggersManager::LoadXML ( void )
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CTriggersManager::LoadXML --> Loading triggers System." );
	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_szFilename.c_str ( ) ) )
	{
		std::string msg_error = "CTriggersManager::LoadXML->Error when trying to load the triggers objects file: " + m_szFilename;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	CPhysicsManager* l_PM = CORE->GetPhysicsManager();
	CPhysicActor* l_FisicTrigger = NULL;
		
	CXMLTreeNode l_NodePare = newFile ["triggers"];
	if ( l_NodePare.Exists ( ) )
	{
		uint16 l_TotalNodes = l_NodePare.GetNumChildren ();
		// Recorro els triggers 
		for ( uint16 i = 0; i < l_TotalNodes; ++i )
		{
			std::string l_Node = l_NodePare(i).GetName();
			if ( l_Node == "trigger" ) 
			{
				CXMLTreeNode l_TriggerNode = l_NodePare(i);
				TriggerInstance l_Instance;
				l_Instance.Name			= static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "name", "" ) );
				l_Instance.TriggerName	= static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "triggername", "" ) );
				l_Instance.ActorName	= static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "actorname", "" ) );
				l_Instance.LayerName	= static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "layer", "" ) );
				l_Instance.TriggerType	= static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "type", "" ) );
				l_Instance.LuaCode		= static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "luacode", "" ) );
				l_Instance.Position		= static_cast<Vect3f> ( l_TriggerNode.GetVect3fProperty   ( "position", Vect3f(0.f,0.f,0.f) ) );
				l_Instance.Size			= static_cast<Vect3f> ( l_TriggerNode.GetVect3fProperty   ( "size",	    Vect3f(0.f,0.f,0.f) ) );
				l_Instance.Color		= static_cast<CColor> ( l_TriggerNode.GetVect4fProperty   ( "color",	Vect4f(0.f,0.f,0.f,0.f) ) );
				l_Instance.Group		= static_cast<uint32> ( l_TriggerNode.GetIntProperty	  ( "group",	0 ) );
				l_Instance.Radius		= static_cast<float>  ( l_TriggerNode.GetFloatProperty    ( "radius",	1.f ) );

				if ( !ExistTrigger ( l_Instance.Name ) ) 
				{
					TriggerInstance l_Trigger; //=GetTrigger( l_Instance.TriggerName );
				
					if ( l_Trigger.pTriggerActor == NULL )
					{
						// Creem un UserData per aquest físic trigger
						CPhysicUserData * l_UserData = new CPhysicUserData ( l_Instance.TriggerName, UD_IS_TRIGGER );
						l_UserData->SetColor ( l_Instance.Color );
						l_UserData->SetPaint ( true );

						// Creem un actor-trigger
						l_FisicTrigger = new CPhysicActor ( l_UserData );
						l_Instance.pTriggerActor = l_FisicTrigger;
						if ( l_Instance.TriggerType == "box" )
							l_FisicTrigger->CreateBoxTrigger ( l_Instance.Position, l_Instance.Size, l_Instance.Group );

						if ( l_Instance.TriggerType == "sphere" )
							l_FisicTrigger->CreateSphereTrigger ( l_Instance.Position, l_Instance.Radius, l_Instance.Group );
					
						if ( !l_PM->AddPhysicActor ( l_FisicTrigger ) )
							LOGGER->AddNewLog ( ELL_ERROR, "CTriggersManager::LoadXML->A físic trigger from file called %s could not be created or already exist", l_Instance.Name.c_str() );  

						m_TriggersMap.insert ( std::pair<std::string, TriggerInstance>( l_Instance.Name, l_Instance ) );
						l_UserData = NULL;
						l_FisicTrigger = NULL;
					}
					else
						LOGGER->AddNewLog ( ELL_WARNING, "CTriggersManager::LoadXML->A trigger from file called %s already exist a It won't be created", l_Instance.Name );  
				}
				else
					LOGGER->AddNewLog ( ELL_ERROR, "CTriggersManager::LoadXML->A trigger from file called %s already exist a It won't be created", l_Instance.Name );  
			}
			else if ( l_Node != "comment" ) 
			{
				std::string msg_error = "CTriggersManager::LoadXML->Error when trying to load a node : " + l_Node + " from file: " + m_szFilename;
				LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			}
		}
	}
	return true;
}

void CTriggersManager::RegisterMethods( void )
{
	lua_State * l_State = SCRIPT->GetLuaState();

	module(l_State) [
		class_<CTriggersManager>("CTriggersManager")
			.def("exist_fisic_trigger", &CTriggersManager::ExistFisicTrigger)			// Retorna si existe un trigger ya cargado
			.def("exist_trigger", &CTriggersManager::ExistTrigger)						// Retorna si existe un físic trigger asociado al trigger ya cargado
			.def("get_trigger", &CTriggersManager::GetTrigger)							// Obtiene el trigger del mapa de triggers
	];
}

// -----------------------------------------
//				 EVENTS 
// -----------------------------------------

void CTriggersManager::OnEnter ( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnEnter->Element 1 es : %s" , _Entity_Trigger1->GetName ().c_str () );
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnEnter->Element 2 es : %s" ,_Other_Shape->GetName ().c_str () );

	// Mirem si està marcat com a trigger el primer paràmetre i actor el segon per aplicar el que necessitem. En l'exemple s'usa a l'hora 
	// de renderitzar
	CPhysicActor* l_TriggerActor = _Entity_Trigger1->GetActor();
    if ( ( (CPhysicUserData*)(l_TriggerActor->GetUserData() ) )->GetFlags() & UD_IS_TRIGGER )
    {
       /* if (status & NX_TRIGGER_ON_STAY)
        {
		}*/
	}
}


void CTriggersManager::OnLeave ( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnLeave->Element 1 es : %s" , _Entity_Trigger1->GetName ().c_str () );
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnLeave->Element 2 es : %s" ,_Other_Shape->GetName ().c_str () );
}


void CTriggersManager::OnStay ( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnLeave->Element 1 es : %s" , _Entity_Trigger1->GetName ().c_str () );
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnLeave->Element 2 es : %s" ,_Other_Shape->GetName ().c_str () );
}

// -----------------------------------------
//				 PROPIETATS 
// -----------------------------------------
TriggerInstance	CTriggersManager::GetTrigger ( std::string _TriggerName )
{
	TriggerInstance l_FisicTrigger;
	bool blnTrobat = false;

	std::map<std::string, TriggerInstance>::iterator l_It = m_TriggersMap.begin();
	std::map<std::string, TriggerInstance>::iterator l_End = m_TriggersMap.end();
	
	/*for ( l_It ; l_It != l_End; ++l_It ) 
	{
		if ( _TriggerName == ((*(&(l_It)._Ptr->_Myval)).second).TriggerName )
		{
			blnTrobat = true;
			l_FisicTrigger = ((*(&(l_It)._Ptr->_Myval)).second);
			break;
		}
	}*/
	
	return l_FisicTrigger;
}

bool CTriggersManager::ExistFisicTrigger ( std::string _FisicTriggerName )
{
	TriggerInstance l_Trigger;
	
	std::map<std::string, TriggerInstance>::iterator l_It = m_TriggersMap.find(_FisicTriggerName);
	if ( l_It != m_TriggersMap.end () )
		return true;
	else 
		return false;
}

bool CTriggersManager::ExistTrigger ( std::string _TriggerName )
{
	TriggerInstance l_Trigger;
	
	std::map<std::string, TriggerInstance>::iterator l_It = m_TriggersMap.find(_TriggerName);
	if ( l_It != m_TriggersMap.end () )
		return true;
	else 
		return false;
}