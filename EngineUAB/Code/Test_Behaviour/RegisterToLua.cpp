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
#include "Characters/States/State_Wrapper.h"
#include "Characters\CharacterManager.h"
#include "PhysicController.h"

#include "Object3D.h"
#include "Utils\Named.h"
#include "Utils\TemplatedVectorMapManager.h"

#include "Foo.h"

#include "core.h"
#include "base.h"
#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

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

void RegisterToLuaCharacter(lua_State* _pLua)
{
	module(_pLua)
		[
			class_<CCharacter, CCharacter_Wrapper, bases<CBaseGameEntity, CObject3D, CNamed>> ("CCharacter")
				.def(constructor<int>())
				.def(constructor<int,const std::string&>())
				.def("init", &CCharacter::Init, &CCharacter_Wrapper::Init)
				.def("update", &CCharacter::Update, &CCharacter_Wrapper::Default_Update)
				.def("get_animation_id", &CCharacter::GetAnimationId)
				.def("get_animation_model", &CCharacter::GetAnimatedModel)
				.def("move_to", &CCharacter::MoveTo)
				.def("face_to", &CCharacter::FaceTo)
				.def("is_player_detected", &CCharacter::IsPlayerDetected)
				.def("add_life", &CCharacter::AddLife)
				.def("rest_Life", &CCharacter::RestLife)
				.def("add_speed", &CCharacter::AddSpeed)
				.def("rest_speed", &CCharacter::RestSpeed)
				.def("add_strong", &CCharacter::AddStrong)
				.def("rest_strong", &CCharacter::RestStrong)
				.def("get_front", &CCharacter::GetFront)
				.property("physic_controller", &CCharacter::GetController)	
				.property("animated_model", &CCharacter::GetAnimatedModel)
				.property("logic_fsm", &CCharacter::GetLogicFSM)
				.property("graphic_fsm", &CCharacter::GetGraphicFSM)
				.property("properties", &CCharacter::GetProperties, &CCharacter::SetProperties)
				.property("locked", &CCharacter::GetLocked, &CCharacter::SetLocked)
				.property("enable", &CCharacter::IsEnable, &CCharacter::SetEnable)
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
				.property("speed", &CProperties::GetSpeed, &CProperties::SetSpeed)	
				.property("rotation_speed", &CProperties::GetRotationSpeed, &CProperties::SetRotationSpeed)	
				.property("respawn_position", &CProperties::GetRespawnPosition, &CProperties::SetRespawnPosition)
				.property("detection_distance", &CProperties::GetDetectionDistance, &CProperties::SetDetectionDistance)
				.property("distance_chase", &CProperties::GetDistanceChase, &CProperties::SetDistanceChase)
				.property("attack_distance", &CProperties::GetAttackDistance, &CProperties::SetAttackDistance)
		];
}

void RegisterToLuaCharacterManager(lua_State* _pLua)
{
	module(_pLua) 
		[
			class_<CCharactersManager>("CCharactersManager")
				/*.def("add_enemy", &CCharactersManager::AddEnemy)
				.def("set_player", &CCharactersManager::SetPlayer)*/
				.def("get_player", &CCharactersManager::GetPlayer)
		];
}

void RegisterToLuaStateMachine(lua_State* _pLua){
	module( _pLua)
		[
			class_<CStateMachine<CCharacter> > ("CStateMachine")
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
		//	class_<CCharacter, CCharacter_Wrapper, bases<CBaseGameEntity, CObject3D, CNamed>> ("CCharacter")
		[
			class_<CState<CCharacter>, CState_Wrapper> ("CState")
				.def(constructor<>())
				.def("Execute", &CState<CCharacter>::Execute)
				.def("OnEnter", &CState<CCharacter>::OnEnter)
				.def("OnExit", &CState<CCharacter>::OnExit)
				.def("OnMessage", &CState<CCharacter>::OnMessage)
		];
}

void RegisterToLuaGlobals(lua_State* _pLua)
{
	/*globals(_pLua)["ent_caperucita"] = (int)ent_caperucita;
	globals(_pLua)["ent_lobo"] = (int)ent_lobo;*/

	/*luabind::module(_pLua) 
		[
			luabind::class_<CFoo>("Foo")
			.def(luabind::constructor<luabind::object, const char*>())
			.def("Think", &CFoo::Think)
			.def("Kill", &CFoo::Kill)
			.def_readwrite("name", &CFoo::m_Name)
		];*/
}

