#include "SteeringEntity.h"
#include "Scripting\ScriptManager.h"
#include "Base.h"
#include "Core.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

CSteeringEntity::CSteeringEntity(void)
{
}


CSteeringEntity::~CSteeringEntity(void)
{
}



// -----------------------------------------
//			     REGISTER METHOD
// -----------------------------------------
void CSteeringEntity::RegisterLUAMethods( void )
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		class_<CSteeringEntity>("CSteeringEntity")
			.def("get_front", &CSteeringEntity::GetFront)
			.def("get_angle", &CSteeringEntity::GetAngle)
			.def("get_perpendicular", &CSteeringEntity::GetPerpendicular)
			.def("get_side", &CSteeringEntity::GetSide)
			.property("velocity", &CSteeringEntity::GetVelocity, &CSteeringEntity::SetVelocity)
			.property("bounding_radius", &CSteeringEntity::GetBoundingRadius, &CSteeringEntity::SetBoundingRadius)
			.property("max_speed", &CSteeringEntity::GetMaxSpeed, &CSteeringEntity::SetMaxSpeed)
			.property("position", &CSteeringEntity::GetPosition, &CSteeringEntity::SetPosition)
			.property("previous_position", &CSteeringEntity::GetPreviousPosition, &CSteeringEntity::SetPreviousPosition)
	];
}