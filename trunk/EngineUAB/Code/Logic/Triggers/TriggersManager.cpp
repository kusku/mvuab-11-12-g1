#include "TriggersManager.h"
#include "Trigger.h"
#include "Types/BoxTrigger.h"
#include "Types\SphereTrigger.h"

#include "PhysicsManager.h"
#include "PhysicUserData.h"
#include "PhysicActor.h"

#include "Scripting\ScriptManager.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"

#include <string>
#include <map>
#include <luabind/adopt_policy.hpp>

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

//----------------------------------------------
CTriggersManager::CTriggersManager()
	: m_szFilename	( "" )
{
}

//----------------------------------------------
CTriggersManager::~CTriggersManager()
{
	Destroy();
}

//----------------------------------------------
void CTriggersManager::Init()
{
	CORE->GetPhysicsManager()->SetTriggerReport( this );

	TTRIGGERMAP::iterator l_It = m_TriggerMap.begin();
	TTRIGGERMAP::iterator l_End = m_TriggerMap.end();

	for(; l_It != l_End; ++l_It)
	{
		l_It->second->Init();
	}
}

//----------------------------------------------
void CTriggersManager::Destroy()
{
	TTRIGGERMAP::iterator l_It = m_TriggerMap.begin();
	TTRIGGERMAP::iterator l_End = m_TriggerMap.end();

	for(; l_It != l_End; ++l_It)
	{
		//El trigger creado en Scripting lo destruye la librería de script.
		l_It->second = NULL;
	}

	m_TriggerMap.clear();
}

//----------------------------------------------
bool CTriggersManager::Load( const std::string &_Filename )
{
	m_szFilename = _Filename;
	return LoadXML();
}

//----------------------------------------------
bool CTriggersManager::Reload()
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CTriggersManager::Reload--> Reloading triggers system." );
	Destroy();
	return LoadXML();
}

//----------------------------------------------
void CTriggersManager::Update( float _ElapsedTime )
{
	TTRIGGERMAP::iterator l_It = m_TriggerMap.begin();
	TTRIGGERMAP::iterator l_End = m_TriggerMap.end();

	for(; l_It != l_End; ++l_It)
	{
		l_It->second->Update(_ElapsedTime);
	}
}

//----------------------------------------------
void CTriggersManager::Render( CRenderManager *_RM )
{
	TTRIGGERMAP::iterator l_It = m_TriggerMap.begin();
	TTRIGGERMAP::iterator l_End = m_TriggerMap.end();

	for(; l_It != l_End; ++l_It)
	{
		l_It->second->Render(_RM);
	}
}

//----------------------------------------------
bool CTriggersManager::LoadXML()
{
	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_szFilename.c_str ( ) ) )
	{
		std::string msg_error = "CTriggersManager::LoadXML->Error when trying to load the triggers objects file: " + m_szFilename;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	CXMLTreeNode l_NodePare = newFile ["triggers"];
	if ( l_NodePare.Exists() )
	{
		uint16 l_TotalNodes = l_NodePare.GetNumChildren ();
		for ( uint16 i = 0; i < l_TotalNodes; ++i )
		{
			CXMLTreeNode l_TriggerNode = l_NodePare(i);

			std::string l_Node = l_NodePare(i).GetName();
			if ( l_Node == "trigger" ) 
			{
				std::string l_Type = static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "type", "" ) );
				std::string l_LUAClass = static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "scriptclass", "" ) );

				CTrigger *l_pTrigger = NULL;
				if( l_Type == "box" )
				{
					l_pTrigger = call_function<CBoxTrigger*>(SCRIPT->GetLuaState(), l_LUAClass.c_str(), 0)[adopt(result)];
				}
				else if( l_Type == "sphere" )
				{
					l_pTrigger = call_function<CSphereTrigger*>(SCRIPT->GetLuaState(), l_LUAClass.c_str(), 0)[adopt(result)];
				}

				l_pTrigger->ReadData(l_TriggerNode);
				m_TriggerMap[ l_pTrigger->GetName() ] = l_pTrigger;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

//----------------------------------------------
void CTriggersManager::OnEnter( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape )
{
	CTrigger *l_pTrigger = NULL;

	TTRIGGERMAP::iterator l_It = m_TriggerMap.begin();
	TTRIGGERMAP::iterator l_End = m_TriggerMap.end();

	for(; l_It != l_End; ++l_It)
	{
		l_pTrigger = l_It->second;
		if( l_pTrigger->IsActive() )
		{
			if( _Entity_Trigger1 == l_pTrigger->GetTriggerActor()->GetUserData() )
			{
				l_pTrigger->OnEnter();
				return;
			}
		}
	}
}

//----------------------------------------------
void CTriggersManager::OnLeave( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape )
{
	CTrigger *l_pTrigger = NULL;

	TTRIGGERMAP::iterator l_It = m_TriggerMap.begin();
	TTRIGGERMAP::iterator l_End = m_TriggerMap.end();

	for(; l_It != l_End; ++l_It)
	{
		l_pTrigger = l_It->second;
		if ( l_pTrigger->IsActive() )
		{
			if( _Entity_Trigger1 == l_pTrigger->GetTriggerActor()->GetUserData() )
			{
				l_pTrigger->OnExit();
				return;
			}
		}
	}
}

//----------------------------------------------
void CTriggersManager::OnStay( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape )
{
	CTrigger *l_pTrigger = NULL;

	TTRIGGERMAP::iterator l_It = m_TriggerMap.begin();
	TTRIGGERMAP::iterator l_End = m_TriggerMap.end();

	for(; l_It != l_End; ++l_It)
	{
		l_pTrigger = l_It->second;
		if( l_pTrigger )
		{
			if( _Entity_Trigger1 == l_pTrigger->GetTriggerActor()->GetUserData() )
			{
				l_pTrigger->OnStay();
				return;
			}
		}
	}
}

//----------------------------------------------
void CTriggersManager::RegisterMethods()
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		class_<CTriggersManager>("CTriggersManager")
			//.def("exist_fisic_trigger", &CTriggersManager::ExistFisicTrigger)			// Retorna si existe un trigger ya cargado
			//.def("exist_trigger", &CTriggersManager::ExistTrigger)						// Retorna si existe un físic trigger asociado al trigger ya cargado
			//.def("get_trigger", &CTriggersManager::GetTrigger)							// Obtiene el trigger del mapa de triggers
	];
}