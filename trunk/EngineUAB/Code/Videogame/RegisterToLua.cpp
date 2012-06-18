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
#include "Steering Behaviours\SteeringBehaviours.h"
#include "Steering Behaviours\SteeringEntity.h"

#include "Characters\Character.h"
#include "Characters\CharacterManager.h"
#include "Characters\CharacterWrapper.h"
#include "Characters/Properties/Properties.h"
#include "Characters\States\State_Wrapper.h"
#include "PhysicController.h"
#include "PhysicUserData.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "Object3D.h"

#include "Utils\Named.h"
#include "Utils\TemplatedVectorMapManager.h"

#include "core.h"
#include "base.h"
#include <string>

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

void RegisterToLuaTelegram(lua_State* _pLua)
{
	module( _pLua)
		[
			class_<Telegram> ("Telegram")
			.def_readwrite("msg",&Telegram::Msg)
			.def_readwrite("extra_info", &Telegram::ExtraInfo)
		];
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
				.def(constructor<>())
				.def(constructor<const std::string&>())
				.def(constructor<int, const std::string&>())
				.def("init", &CCharacter::Init, &CCharacter_Wrapper::Init)
				.def("update", &CCharacter::Update, &CCharacter_Wrapper::Default_Update)
				.def("get_animation_model", &CCharacter::GetAnimatedModel)
				.def("get_animation_id", &CCharacter::GetAnimationID)
				.def("move_to", &CCharacter::MoveTo)
				.def("face_to", &CCharacter::FaceTo)
				.def("add_life", &CCharacter::AddLife)
				.def("rest_life", &CCharacter::RestLife)
				.def("add_speed", &CCharacter::AddSpeed)
				.def("rest_speed", &CCharacter::RestSpeed)
				.def("add_strong", &CCharacter::AddStrong)
				.def("rest_strong", &CCharacter::RestStrong)
				.def("get_front", &CCharacter::GetFront)
				.property( "behaviours", &CCharacter::GetBehaviours)
				.property( "steering_entity", &CCharacter::GetSteeringEntity)
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
				.property("distance_chase", &CProperties::GetChaseDistance, &CProperties::SetChaseDistance)
				.property("distance_path_follow", &CProperties::GetFollowPathDistance, &CProperties::SetFollowPathDistance)
				.property("attack_distance", &CProperties::GetAttackDistance, &CProperties::SetAttackDistance)
				.property("tired_distance", &CProperties::GetTiredDistance, &CProperties::SetTiredDistance)
				.property("panic_distance", &CProperties::GetPanicDistance, &CProperties::SetPanicDistance)
				.property("bounding_radius", &CProperties::GetBoundingRadious, &CProperties::SetBoundingRadious)
				.property("max_speed", &CProperties::GetMaxSpeed, &CProperties::SetMaxSpeed)
				.property("animation_offset", &CProperties::GetAnimationOffset)
				
		];
}

void RegisterToLuaCharacterManager(lua_State* _pLua)
{
	module(_pLua) 
		[
			class_<CTemplatedVectorMapManager<CCharacter>>("CTemplatedVectorMapManager")
		];

	module(_pLua)
		[
			class_<CCharactersManager, bases<CTemplatedVectorMapManager<CCharacter>> > ("CCharactersManager")
				.def("add_enemy", &CCharactersManager::AddEnemy)
				.def("set_player", &CCharactersManager::SetPlayer)
				.def("get_player", &CCharactersManager::GetPlayer)
				.def("exist_enemy_user_data", &CCharactersManager::ExistEnemyUserData)
				.def("shoot_player_raycast", &CCharactersManager::ShootPlayerRaycast)
				.def("is_player_near_enemy", &CCharactersManager::IsPlayerNearEnemy)
				.def("search_target_enemy", &CCharactersManager::SearchTargetEnemy)
				.def("get_enemy_by_name", &CCharactersManager::GetEnemyByName)
				.property("target_enemy", &CCharactersManager::GetTargetEnemy, &CCharactersManager::SetTargetEnemy)
				.property("preview_target_enemy", &CCharactersManager::GetPreviewTargetEnemy, &CCharactersManager::SetPreviewTargetEnemy)
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

	CSteeringBehaviours::RegisterLUAMethods();
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
	globals(_pLua)["ent_caperucita"] = (int)ent_caperucita;
	globals(_pLua)["ent_lobo"] = (int)ent_lobo;

	/*luabind::module(_pLua) 
		[
			luabind::class_<CFoo>("Foo")
			.def(luabind::constructor<luabind::object, const char*>())
			.def("Think", &CFoo::Think)
			.def("Kill", &CFoo::Kill)
			.def_readwrite("name", &CFoo::m_Name)
		];*/
}

