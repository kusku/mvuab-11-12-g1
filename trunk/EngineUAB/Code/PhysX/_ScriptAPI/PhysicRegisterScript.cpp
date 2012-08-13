#include "PhysicRegisterScript.h"
#include "Scripting\ScriptManager.h"
#include "Object3D.h"

#include "PhysicsManager.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicFixedJoint.h"
#include "PhysicSphericalJoint.h"
#include "PhysicRevoluteJoint.h"
#include "PhysicActor.h"


#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

namespace Physic
{
namespace ScriptAPI
{
	void RegisterScript(lua_State *_pLua)
	{
		module(_pLua) [
			class_<CPhysicsManager>("CPhysicsManager")
				.def("raycast_closest_actor",&CPhysicsManager::RaycastClosestActor)
				.property("debug_render_mode",  &CPhysicsManager::GetDebugRenderMode, &CPhysicsManager::SetDebugRenderMode)
				.def("add_physic_actor", &CPhysicsManager::AddPhysicActor)
				.def("add_physic_controller", &CPhysicsManager::AddPhysicController)
				.def("add_physic_fixed_joint", &CPhysicsManager::AddPhysicFixedJoint)
				.def("add_physic_revolute_joint", &CPhysicsManager::AddPhysicRevoluteJoint)
				.def("add_physic_spherical_joint", &CPhysicsManager::AddPhysicSphericalJoint)
		];
		
		module(_pLua) [
			class_<CPhysicController, CObject3D>("CPhysicController")
				.def("move", &CPhysicController::Move)
				.def("jump", &CPhysicController::Jump)
				.property("height", &CPhysicController::GetHeight)
				.property("user_data", &CPhysicController::GetUserData)
		];

		module(_pLua) [
			class_<CPhysicActor>("CPhysicActor")
		];

		module(_pLua) [
			class_<CPhysicUserData>("CPhysicUserData")
				.property("actor", &CPhysicUserData::GetActor, &CPhysicUserData::SetActor)
				.property("entity", &CPhysicUserData::GetEntity, &CPhysicUserData::SetEntity)
		];

		// registramos la clase CPhysicsManager
		module(_pLua) [
			class_<CPhysicSphericalJoint>("CPhysicSphericalJoint")
				.def(constructor<>())
				.def("create_joint",  &CPhysicSphericalJoint::CreateJoint)
				.def("set_info_complete", &CPhysicSphericalJoint::SetInfoComplete)
				.def("set_info", &CPhysicSphericalJoint::SetInfo)
				.def("get_joint", &CPhysicSphericalJoint::GetPhXJoint)
				.def("get_desc_joint", &CPhysicSphericalJoint::GetPhXDescJoint)
		];
		// registramos la clase CPhysicsManager
		module(_pLua) [
			class_<CPhysicFixedJoint>("CPhysicFixedJoint")
				.def(constructor<>())
				.def("create_joint",  &CPhysicFixedJoint::CreateJoint)
				.def("set_info", &CPhysicFixedJoint::SetInfo)
				.def("get_joint", &CPhysicFixedJoint::GetPhXJoint)
				.def("get_desc_joint", &CPhysicFixedJoint::GetPhXDescJoint)
		];

		// registramos la clase CPhysicsManager
		module(_pLua) [
			class_<CPhysicRevoluteJoint>("CPhysicRevoluteJoint")
				.def(constructor<>())
				.def("create_joint",  &CPhysicRevoluteJoint::CreateJoint)
				.def("set_info", &CPhysicRevoluteJoint::SetInfo)
				.def("get_joint", &CPhysicRevoluteJoint::GetPhXJoint)
				.def("get_desc_joint", &CPhysicRevoluteJoint::GetPhXDescJoint)
				.def("active_motor", &CPhysicRevoluteJoint::ActiveMotor)
				.def("set_motor", &CPhysicRevoluteJoint::SetMotor)
		];

	}

} //namespace ScriptAPI
} //namespace Physic