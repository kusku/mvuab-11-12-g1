#include "VideogameRegisterScript.h"
#include "Scripting/ScriptManager.h"
#include "GameProcess.h"
#include "Cameras/ThPSCharacterCamera.h"
#include "StatesMachine/BaseGameEntity.h"
#include "RenderableObjects/AnimatedModel/AnimatedInstanceModel.h"
#include "PhysicController.h"
#include "Characters/Character.h"
#include "Characters/CharacterManager.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "StatesMachine\StateMachine.h"
#include "StatesMachine\State.h"
#include "PhysicUserData.h"
#include "Utils/TemplatedVectorMapManager.h"
#include "Utils/Named.h"
#include "Object3D.h"
#include "Characters/Properties/Properties.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "cal3d\animcallback.h"

#include "_ScriptAPI\Wrappers\CharacterWrapper.h"
#include "_ScriptAPI\Wrappers\StateWrapper.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

namespace Videogame
{
namespace ScriptAPI
{
	void RegisterScript(lua_State *_pLua)
	{
		module(_pLua)
			[
				class_<CGameProcess>("CGameProcess")
					.def("get_character_manager", &CGameProcess::GetCharactersManager)
					.def("create_player_camera", &CGameProcess::CreatePlayerCamera)
					.def("get_time_between_clicks", &CGameProcess::GetTimeBetweenClicks)
					.def("change_to_gui_process", &CGameProcess::ChangeToGUIProcess)
					.def("get_animation_callback_manager", &CGameProcess::GetAnimationCallbackManager)
					.property("player_camera", &CGameProcess::GetPlayerCamera)
			];

		module(_pLua)
			[
				class_<CCharacter, CCharacter_Wrapper, bases<CBaseGameEntity, CObject3D, CNamed>> ("CCharacter")
					.def(constructor<>())
					.def(constructor<const std::string&>())
					.def(constructor<int, const std::string&>())
					.def("init", &CCharacter::Init, &CCharacter_Wrapper::Init)
					.def("update", &CCharacter::Update)
					.def("get_animation_model", &CCharacter::GetAnimatedModel)
					.def("get_animation_id", &CCharacter::GetAnimationID)
					.def("move_to", &CCharacter::MoveTo)
					.def("move_to2", &CCharacter::MoveTo2)
					.def("face_to", &CCharacter::FaceTo)
					.def("face_to2", &CCharacter::FaceTo2)
					.def("add_life", &CCharacter::AddLife)
					.def("rest_life", &CCharacter::RestLife)
					.def("add_speed", &CCharacter::AddSpeed)
					.def("rest_speed", &CCharacter::RestSpeed)
					.def("add_strong", &CCharacter::AddStrong)
					.def("rest_strong", &CCharacter::RestStrong)
					.def("get_front", &CCharacter::GetFront)
					.def("is_alive", &CCharacter::IsAlive)
					.property( "behaviors", &CCharacter::GetBehaviors)
					.property( "steering_entity", &CCharacter::GetSteeringEntity)
					.property("physic_controller", &CCharacter::GetController)	
					.property("animated_model", &CCharacter::GetAnimatedModel)
					.property("logic_fsm", &CCharacter::GetLogicFSM)
					.property("graphic_fsm", &CCharacter::GetGraphicFSM)
					.property("properties", &CCharacter::GetProperties, &CCharacter::SetProperties)
					.property("locked", &CCharacter::GetLocked, &CCharacter::SetLocked)
					.property("enable", &CCharacter::IsEnable, &CCharacter::SetEnable)
					.property("ready_to_attack", &CCharacter::GetReadyToAttack, &CCharacter::SetReadyToAttack)
			];

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
					.def("get_enemy_by_id", &CCharactersManager::GetCharacterById)
					.property("target_enemy", &CCharactersManager::GetTargetEnemy, &CCharactersManager::SetTargetEnemy)
					.property("preview_target_enemy", &CCharactersManager::GetPreviewTargetEnemy, &CCharactersManager::SetPreviewTargetEnemy)
			];

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

		module( _pLua)
			[
				class_<CState<CCharacter>, CState_Wrapper, CNamed> ("CState")
					.def(constructor<const std::string&>())
					.def("Execute", &CState<CCharacter>::Execute)
					.def("OnEnter", &CState<CCharacter>::OnEnter)
					.def("OnExit", &CState<CCharacter>::OnExit)
					.def("OnMessage", &CState<CCharacter>::OnMessage)
			];

		module(_pLua)
			[
				class_<CProperties, bases<CObject3D, CNamed>> ("CProperties")
					.def(constructor<>())
					.property("core", &CProperties::GetCore, &CProperties::SetCore)	
					.property("animated_instance_name", &CProperties::GetAnimationInstance, &CProperties::SetAnimationInstance)	
					.property("life", &CProperties::GetLife, &CProperties::SetLife)	
					.property("strong", &CProperties::GetStrong, &CProperties::SetStrong)	
					.property("mass", &CProperties::GetMass, &CProperties::SetMass)
					.property("respawn_position", &CProperties::GetRespawnPosition, &CProperties::SetRespawnPosition)
					.property("velocity", &CProperties::GetVelocity, &CProperties::SetVelocity)	
					.property("speed", &CProperties::GetSpeed, &CProperties::SetSpeed)	
					.property("max_speed", &CProperties::GetMaxSpeed, &CProperties::SetMaxSpeed)
					.property("min_speed", &CProperties::GetMinSpeed, &CProperties::SetMinSpeed)
					.property("max_acceleration", &CProperties::GetMaxAcceleration, &CProperties::SetMaxAcceleration)
					.property("max_rotation", &CProperties::GetMaxRotationSpeed, &CProperties::SetMaxRotationSpeed)
					.property("max_force", &CProperties::GetMaxForce, &CProperties::SetMaxForce)	
					.property("detection_distance", &CProperties::GetDetectionDistance, &CProperties::SetDetectionDistance)
					.property("chase_distance", &CProperties::GetChaseDistance, &CProperties::SetChaseDistance)
					.property("path_follow_distance", &CProperties::GetFollowPathDistance, &CProperties::SetFollowPathDistance)
					.property("impact_distance", &CProperties::GetImpactDistance, &CProperties::SetImpactDistance)
					.property("attack_distance", &CProperties::GetAttackDistance, &CProperties::SetAttackDistance)
					.property("aproximation_distance", &CProperties::GetAproximationDistance, &CProperties::SetAproximationDistance)
					.property("prepared_attack_distance", &CProperties::GetPreparedAttackDistance, &CProperties::SetPreparedAttackDistance)
					.property("tired_distance", &CProperties::GetTiredDistance, &CProperties::SetTiredDistance)
					.property("panic_distance", &CProperties::GetPanicDistance, &CProperties::SetPanicDistance)
					.property("bounding_radius", &CProperties::GetBoundingRadious, &CProperties::SetBoundingRadious)
					.property("animation_offset", &CProperties::GetAnimationOffset)
			];

		module(_pLua)
			[
				class_<CThPSCharacterCamera>("CThPSCharacterCamera")
					.def("get_direction", &CThPSCharacterCamera::GetDirection)
					.def("add_zoom", &CThPSCharacterCamera::AddZoom)
					.def("update", &CThPSCharacterCamera::Update)
					.property("zoom", &CThPSCharacterCamera::GetZoom, &CThPSCharacterCamera::SetZoom)
			];

		module(_pLua)
			[
				class_<CAnimationCallbackManager>("CAnimationCallbackManager")
					.def("create_callback", &CAnimationCallbackManager::CreateCallback)
					.def("remove_callback", &CAnimationCallbackManager::RemoveCallback)
					.def("get_callback", &CAnimationCallbackManager::GetCallback)
			];

		module(_pLua)
			[
				class_<CAnimationCallback, CalAnimationCallback>("CAnimationCallback")
					.def("start_animation", &CAnimationCallback::StartAnimation)
					.def("is_animation_finished", &CAnimationCallback::IsAnimationFinished)
					.property("animation_name", &CAnimationCallback::GetAnimationName)
					.property("animation_instance", &CAnimationCallback::GetAnimatedModel)
			];
	}

} //namespace ScriptAPI
} //namespace Videogame