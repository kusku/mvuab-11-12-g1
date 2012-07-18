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
#include "Steering Behaviours\SteeringBehaviours.h"
#include "Steering Behaviours\Steering.h"
#include "Steering Behaviours\Seek.h"
#include "Steering Behaviours\Pursuit.h"
#include "Steering Behaviours\Arrive.h"

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
			class_<CSeek>("CSeek")
		];

		module(_pLua) [
			class_<CPursuit>("CPursuit")
		];

		module(_pLua) [
			class_<CArrive>("CArrive")
		];

		module(_pLua) [
			class_<CSteeringBehaviours>("CSteeringBehaviours")
				.def("has_behavior", &CSteeringBehaviours::HasBehavior)
				.def("update", &CSteeringBehaviours::Update)
				.def("add_behaviour", (void(CSteeringBehaviours::*)(CSeek*)) &CSteeringBehaviours::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviours::*)(CPursuit*)) &CSteeringBehaviours::AddBehavior)
				.def("add_behaviour", (void(CSteeringBehaviours::*)(CArrive*)) &CSteeringBehaviours::AddBehavior)
				.property("seek", &CSteeringBehaviours::GetSeek, &CSteeringBehaviours::SetSeek)
				.property("pursuit", &CSteeringBehaviours::GetPursuit, &CSteeringBehaviours::SetPursuit)
				.property("arrive", &CSteeringBehaviours::GetArrive, &CSteeringBehaviours::SetArrive)
		];
	}

} //namespace ScriptAPI
} //namespace Logic