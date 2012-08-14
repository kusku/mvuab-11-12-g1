#include "LogicRegisterScript.h"
#include "Scripting\ScriptManager.h"
#include "Object3D.h"
#include "Triggers/Types/BoxTrigger.h"
#include "Triggers\Types\SphereTrigger.h"
#include "Triggers\Trigger.h"
#include "_ScriptAPI\Wrappers\TriggerWrapper.h"
#include "StatesMachine\EntityManager.h"
#include "StatesMachine\MessageDispatcher.h"
#include "StatesMachine\BaseGameEntity.h"
#include "StatesMachine\Telegram.h"
#include "Utils\Named.h"
#include "Movement\WayPointManager.h"
#include "Movement\WayPoint.h"

#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\Steering.h"
#include "Steering Behaviors\Smoother.h"
#include "Steering Behaviors\Seek.h"
//#include "Steering Behaviors\Flee.h"	
#include "Steering Behaviors\Pursuit.h"
#include "Steering Behaviors\Arrive.h"

#include "Steering Behaviors\Smoother.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

namespace Logic
{
namespace ScriptAPI
{
	void RegisterScript(lua_State *_pLua)
	{
		//TRIGGERS
		module(_pLua)
			[
				class_<CTrigger, CNamed>("CTrigger")
					.property("trigger_object", &CTrigger::GetTriggerObject, &CTrigger::SetTriggerObject)
					.property("active", &CTrigger::IsActive, &CTrigger::SetActive)
			];

		module(_pLua)
			[
				class_<CBoxTrigger, Box_Trigger_Wrapper, CTrigger>("CBoxTrigger")
					.def(constructor<>())
					.def("update", &CBoxTrigger::Update, &Box_Trigger_Wrapper::Update)
					.def("on_enter", &CBoxTrigger::OnEnter, &Box_Trigger_Wrapper::OnEnter)
					.def("on_stay", &CBoxTrigger::OnStay, &Box_Trigger_Wrapper::OnStay)
					.def("on_exit", &CBoxTrigger::OnExit, &Box_Trigger_Wrapper::OnExit)
			];

		module(_pLua)
			[
				class_<CSphereTrigger, Sphere_Trigger_Wrapper, CTrigger>("CSphereTrigger")
					.def(constructor<>())
					.def("update", &CSphereTrigger::Update, &Sphere_Trigger_Wrapper::Update)
					.def("on_enter", &CSphereTrigger::OnEnter, &Sphere_Trigger_Wrapper::OnEnter)
					.def("on_stay", &CSphereTrigger::OnStay, &Sphere_Trigger_Wrapper::OnStay)
					.def("on_exit", &CSphereTrigger::OnExit, &Sphere_Trigger_Wrapper::OnExit)
			];

		module(_pLua)
			[
				class_<CEntityManager>("CEntityManager")
					.def("get_entity_from_id",&CEntityManager::GetEntityFromID)
					.def("remove_entities", &CEntityManager::RemoveEntities)
			];

		module(_pLua)
			[
				class_<CMessageDispatcher> ("CMessageDispatcher")
					.def("dispatch_state_message",&CMessageDispatcher::DispatchStateMessage)
			];

		globals(_pLua)["_dispatchM"]	= DISPATCH;

		globals(_pLua)["msg_idle"]		= (int)Msg_Idle;
		globals(_pLua)["msg_ready"]		= (int)Msg_Ready;
		globals(_pLua)["msg_sleep"]		= (int)Msg_Sleep;
		globals(_pLua)["msg_attack"]	= (int)Msg_Attack;
		globals(_pLua)["msg_run_away"]	= (int)Msg_RunAway;
		globals(_pLua)["msg_patrol"]	= (int)Msg_Patrol;
		globals(_pLua)["msg_pursuit"]	= (int)Msg_Pusuit;
		globals(_pLua)["msg_roam"]		= (int)Msg_Roam;
		globals(_pLua)["msg_evade"]		= (int)Msg_Evade;
		globals(_pLua)["msg_chase"]		= (int)Msg_Chase;

		module(_pLua) 
			[
				class_<CWayPoint>("CWayPoint")
				.property("position", &CWayPoint::GetPosition, &CWayPoint::SetPosition)
				.property("name", &CWayPoint::GetName, &CWayPoint::SetName)
				.def("get_brothers", &CWayPoint::GetBrothers)
			];

		module(_pLua) 
			[
				class_<CWayPointManager>("CWayPointManager")
				.def("get_closest_way_point", &CWayPointManager::GetClosestWayPoint)
				.def("get_way_point", &CWayPointManager::GetWayPoint)
			];

		module(_pLua)
			[
				class_<CBaseGameEntity> ("CBaseGameEntity")
				.def("get_id",&CBaseGameEntity::GetID)
			];

		module(_pLua)
			[
				class_<STelegram> ("Telegram")
				.def_readwrite("msg",&STelegram::Msg)
				.def_readwrite("sender",&STelegram::Sender)
				.def_readwrite("receiver",&STelegram::Receiver)
				.def_readwrite("extra_info", &STelegram::ExtraInfo)
			];

		module(_pLua) [
			class_<CSteering>("CSteering")
				.property( "type", &CSteering::GetType)
				.property( "target", &CSteering::GetTarget, &CSteering::SetTarget)
		];

		module(_pLua) [
			class_<CSteeringEntity>("CSteeringEntity")
				.def(constructor<>())
				.def("get_front", &CSteeringEntity::GetFront)
				.def("get_yaw", &CSteeringEntity::GetYaw)
				.def("get_side", &CSteeringEntity::GetSide)
				.def("get_angle", &CSteeringEntity::GetAngle)
				.def("get_angle2", &CSteeringEntity::GetAngle2)
				.def("get_initial_position_to_throwRay", &CSteeringEntity::GetInitialPositionToThrowRay)
				.def("get_final_position_to_throwRay", &CSteeringEntity::GetFinalPositionToThrowRay)
				.property("name", &CSteeringEntity::GetName, &CSteeringEntity::SetName)
				.property("mass", &CSteeringEntity::GetMass, &CSteeringEntity::SetMass)
				.property("yaw", &CSteeringEntity::GetYaw, &CSteeringEntity::SetYaw)
				.property("velocity", &CSteeringEntity::GetVelocity, &CSteeringEntity::SetVelocity)
				.property("heading", &CSteeringEntity::GetHeading, &CSteeringEntity::SetHeading)
				.property("side", &CSteeringEntity::GetSide, &CSteeringEntity::SetSide)
				.property("speed", &CSteeringEntity::GetSpeed )
				.property("max_speed", &CSteeringEntity::GetMaxSpeed, &CSteeringEntity::SetMaxSpeed)
				.property("max_force", &CSteeringEntity::GetMaxForce, &CSteeringEntity::SetMaxForce)
				.property("max_rotation_speed", &CSteeringEntity::GetMaxRotation, &CSteeringEntity::SetMaxRotation)
				.property("position", &CSteeringEntity::GetPosition, &CSteeringEntity::SetPosition)
				.property("previous_position", &CSteeringEntity::GetPreviousPosition, &CSteeringEntity::SetPreviousPosition)
				.property("bounding_radius", &CSteeringEntity::GetBoundingRadius, &CSteeringEntity::SetBoundingRadius)
				.property("height", &CSteeringEntity::GetHeight, &CSteeringEntity::SetHeight)
				/*.property("smoothing_heading", &CSteeringEntity::GetSmoothedHeading, &CSteeringEntity::SetSmoothedHeading)
				.property("heading_smoother", &CSteeringEntity::GetHeadingSmoother)*/
				.def("is_smoothing_on", &CSteeringEntity::isSmoothingOn)
				.def("toggle_smoothing", &CSteeringEntity::ToggleSmoothing)
				.def("smoothing_on", &CSteeringEntity::SmoothingOn)
				.def("smoothing_off", &CSteeringEntity::SmoothingOff)
		];

		// STEERINGS 
		module(_pLua) [
			class_<CArrive>("CArrive")
		];

		/*module(_pLua) [
			class_<CFlee>("CFlee")
		];*/

		module(_pLua) [
			class_<CPursuit>("CPursuit")
				.def(constructor<>())
				.def(constructor<eDeceleration, float>())
				.def("update_evader_entity", &CPursuit::UpdateEvaderEntity )
		];

		module(_pLua) [
			class_<CSeek, CSteering>("CSeek")
				.def(constructor<>())
		];

		/*module(_pLua) [
			class_<CSmoother>("CSmoother")
				.def("update", &CSmoother::Update)

		];*/
		

		module(_pLua) [
			class_<CSteeringBehaviors>("CSteeringBehaviors")
				.def("has_behavior", &CSteeringBehaviors::HasBehavior)
				.def("update", &CSteeringBehaviors::Update)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CSeek*)) &CSteeringBehaviors::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CPursuit*)) &CSteeringBehaviors::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CArrive*)) &CSteeringBehaviors::AddBehavior)
				/*.def("add_behaviour", (void(CSteeringBehaviors::*)(CEvade*)) &CSteeringBehaviors::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CArrive*)) &CSteeringBehaviors::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CWander*)) &CSteeringBehaviors::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CCollisionAvoidance*)) &CSteeringBehaviors::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CObstacleWallAvoidance*)) &CSteeringBehaviors::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CSeparation*)) &CSteeringBehaviors::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CAlignment*)) &CSteeringBehaviors::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviors::*)(CCohesion*)) &CSteeringBehaviors::AddBehavior)*/
				.def("on", &CSteeringBehaviors::On)
				.def("seek_on", &CSteeringBehaviors::SeekOn)
				.def("seek_off", &CSteeringBehaviors::SeekOff)
				.def("flee_on", &CSteeringBehaviors::FleeOn)
				.def("flee_off", &CSteeringBehaviors::FleeOff)
				.def("pursuit_on", &CSteeringBehaviors::PursuitOn)
				.def("pursuit_off", &CSteeringBehaviors::PursuitOff)
				//.def("offset_pursuit_on", &CSteeringBehaviors::OffsetPursuitOn)
				.def("offset_pursuit_off", &CSteeringBehaviors::OffsetPursuitOff)
				.def("evade_on", &CSteeringBehaviors::EvadeOn)
				.def("evade_off", &CSteeringBehaviors::EvadeOff)
				.def("arrive_on", &CSteeringBehaviors::ArriveOn)
				.def("arrive_off", &CSteeringBehaviors::ArriveOff)
				.def("wander_on", &CSteeringBehaviors::WanderOn)
				.def("wander_off", &CSteeringBehaviors::WanderOff)
				.def("collision_avoidance_on", &CSteeringBehaviors::CollisionAvoidanceOn)
				.def("collision_avoidance_off", &CSteeringBehaviors::CollisionAvoidanceOff)
				.def("obstacle_wall_avoidance_on", &CSteeringBehaviors::ObstacleWallAvoidanceOn)
				.def("obstacle_wall_avoidance_off", &CSteeringBehaviors::ObstacleWallAvoidanceOff)
				.def("separation_on", &CSteeringBehaviors::SeparationOn)
				.def("separation_off", &CSteeringBehaviors::SeparationOff)
				.def("alignment_on", &CSteeringBehaviors::AlignmentOn)
				.def("alignment_off", &CSteeringBehaviors::AlignmentOff)
				.def("cohesion_on", &CSteeringBehaviors::CohesionOn)
				.def("cohesion_off", &CSteeringBehaviors::CohesionOff)
				.property("seek", &CSteeringBehaviors::GetSeek, &CSteeringBehaviors::SetSeek)
				/*.property("flee", &CSteeringBehaviors::GetFlee, &CSteeringBehaviors::SetFlee)*/
				.property("pursuit", &CSteeringBehaviors::GetPursuit, &CSteeringBehaviors::SetPursuit)
				/*.property("evade", &CSteeringBehaviors::GetEvade, &CSteeringBehaviors::SetEvade)*/
				.property("arrive", &CSteeringBehaviors::GetArrive, &CSteeringBehaviors::SetArrive)
				/*.property("wander", &CSteeringBehaviors::GetWander, &CSteeringBehaviors::SetWander)
				.property("collision_avoidance", &CSteeringBehaviors::GetCollisionAvoidance, &CSteeringBehaviors::SetCollisionAvoidance)
				.property("obstacle_wall_avoidance", &CSteeringBehaviors::GetObstacleWallAvoidance, &CSteeringBehaviors::SetObstacleWallAvoidance)*/
				/*.property("separation", &CSteeringBehaviors::GetSeparation, &CSteeringBehaviors::SetSeparation)
				.property("alignment", &CSteeringBehaviors::GetAlignment, &CSteeringBehaviors::SetAlignment)
				.property("cohesion", &CSteeringBehaviors::GetCohesion, &CSteeringBehaviors::SetCohesion)*/
		];
		
		module(_pLua) [
			class_<CSteeringBehaviorsSeetingsManager>("CSteeringBehaviorsSeetingsManager")
				.def(constructor<>())
				.property("wander_distance", &CSteeringBehaviorsSeetingsManager::GetWanderDistance, &CSteeringBehaviorsSeetingsManager::SetWanderDistance)
				.property("wander_radius", &CSteeringBehaviorsSeetingsManager::GetWanderRadius, &CSteeringBehaviorsSeetingsManager::SetWanderRadius)
				.property("wander_refresh_rate", &CSteeringBehaviorsSeetingsManager::GetWanderRefreshRate, &CSteeringBehaviorsSeetingsManager::SetWanderRefreshRate)
		];

		
		
	}

} //namespace ScriptAPI
} //namespace Logic