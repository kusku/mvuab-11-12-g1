#include "RegisterToLua.h"
#include "Scripting\ScriptManager.h"

#include "GameProcess.h"

#include "StatesMachine\BaseGameEntity.h"
#include "StatesMachine\StateMachine.h"
#include "StatesMachine\Telegram.h"
#include "StatesMachine\MessageDispatcher.h"
#include "StatesMachine\BaseGameEntity.h"
#include "StatesMachine\StateMachine.h"
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Character_Wrapper.h"
#include "Characters/Properties/Properties.h"

#include "PhysicController.h"

#include "Object3D.h"
#include "Utils\Named.h"
#include "Utils\TemplatedVectorMapManager.h"

#include "Foo.h"

#include "core.h"
#include "base.h"

void RegisterToLuaTelegram(lua_State* _pLua)
{
	module( _pLua)
		[
			class_<Telegram> ("Telegram")
			.def_readwrite("Msg",&Telegram::Msg)
			.def_readwrite("ExtraInfo", &Telegram::ExtraInfo)
		];
}

void RegisterToLuaMessageDispacher(lua_State* _pLua)
{
	//module(_pLua)
	//	[
	//		class_<CMessageDispatcher> ("Dispatch")
	//		.def("DispatchStateMessage",&MessageDispatcher::DispatchMsg)
	//	];
	//globals( Lua)["Dispatch"] = Dispatch; // Es una class singleton
}

void RegisterToLuaBaseGameEntity(lua_State* _pLua)
{
	module(_pLua)
		[
			class_<CBaseGameEntity> ("CBaseGameEntity")
			.def("get_id",&CBaseGameEntity::GetID)
		];
}

void RegisterToLuaCNamed(lua_State* _pLua)
{
	module(_pLua)
		[
			class_<CNamed> ("CName")
			
		];
}

void RegisterToLuaCObject3D(lua_State* _pLua)
{
	module(_pLua)
		[
			class_<CObject3D> ("CObject3D")
		];
}

void RegisterToLuaCharacter(lua_State* _pLua)
{
	module(_pLua)
		[
			class_<CCharacter, CCharacter_Wrapper, bases<CBaseGameEntity, CObject3D, CNamed>> ("CCharacter")
				.def(constructor<int>())
				.def(constructor<int,const std::string&>())
				.def("init", &CCharacter::Init)
				.def("update", &CCharacter::Update, &CCharacter_Wrapper::default_update)
				.property("physic_controller", &CCharacter::GetController)	
				.property("animated_model", &CCharacter::GetAnimatedModel)
				.property("get_logic_fsm", &CCharacter::GetLogicFSM)
				.property("get_graphic_fsm", &CCharacter::GetLogicFSM)
				.property("properties", &CCharacter::GetProperties, &CCharacter::SetProperties)
				.property("locked", &CCharacter::GetLocked, &CCharacter::SetLocked)
		];
}

void RegisterToLuaProperties(lua_State* _pLua)
{
	module(_pLua)
		[
			class_<CProperties, bases<CObject3D, CNamed>> ("CProperties")
				.def(constructor<>())
				.property("core", &CProperties::GetCore, &CProperties::SetCore)	
				.property("animated_instance_name", &CProperties::GetAnimationInstance, &CProperties::SetAnimationInstance)	
				.property("life", &CProperties::GetLife, &CProperties::SetLife)	
				.property("strong", &CProperties::GetStrong, &CProperties::SetStrong)	
				.property("direction", &CProperties::GetDirection, &CProperties::SetDirection)	
				.property("respawn_position", &CProperties::GetRespawnPosition, &CProperties::SetRespawnPosition)	
		];
}

void RegisterToLuaCharacterManager(lua_State* _pLua)
{
	//module(_pLua)
	//	[
	//		class_<CCharactersManager, bases<CTemplatedVectorMapManager<CCharacter>> > ("CCharactersManager")
	//			.def("add_enemy", &CCharactersManager::AddEnemy)
	//	]
}

void RegisterToLuaStateMachine(lua_State* _pLua){
	module( _pLua)
		[class_<CStateMachine<CCharacter> > ("CStateMachine")
		.def("change_state", &CStateMachine<CCharacter>::ChangeState)
		.property("current_state", &CStateMachine<CCharacter>::GetCurrentState, &CStateMachine<CCharacter>::SetCurrentState)
		.property("previous_state", &CStateMachine<CCharacter>::GetPreviousState, &CStateMachine<CCharacter>::SetPreviousState)
		.property("global_state", &CStateMachine<CCharacter>::GetGlobalState, &CStateMachine<CCharacter>::SetGlobalState)
		.def("revert_to_previous_state",&CStateMachine<CCharacter>::RevertToPreviousState)
		.def("is_in_state",&CStateMachine<CCharacter>::isInState)
		.property("get_name_of_current_state", &CStateMachine<CCharacter>::GetNameOfCurrentState)
		.def("update",&CStateMachine<CCharacter>::Update)
		];
}

void RegisterToLuaState(lua_State* _pLua){
	module( _pLua)
		[class_<CState<CCharacter> > ("CState")
			.def(constructor<>())
			.def("execute", &CState<CCharacter>::Execute)
			.def("on_enter", &CState<CCharacter>::OnEnter)
			.def("on_exit", &CState<CCharacter>::OnExit)
			.def("on_message", &CState<CCharacter>::OnMessage)
		];
}

void RegisterToLuaGlobals(lua_State* _pLua)
{
	globals(_pLua)["ent_caperucita"] = (int)ent_caperucita;
	globals(_pLua)["ent_lobo"] = (int)ent_lobo;

	luabind::module(_pLua) 
		[
			luabind::class_<CFoo>("Foo")
			.def(luabind::constructor<luabind::object, const char*>())
			.def("Think", &CFoo::Think)
			.def("Kill", &CFoo::Kill)
			.def_readwrite("name", &CFoo::m_Name)
		];
}

