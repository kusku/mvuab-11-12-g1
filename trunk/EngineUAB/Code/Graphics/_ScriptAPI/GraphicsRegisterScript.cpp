#include "GraphicsRegisterScript.h"
#include "Scripting/ScriptManager.h"
#include "Cameras/ThPSCamera.h"
#include "RenderableObjects/AnimatedModel/AnimatedInstanceModel.h"
#include "Object3D.h"
#include "Utils/Named.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

namespace Graphics
{
namespace ScriptAPI
{
	void RegisterScript(lua_State *_pLua)
	{
		module(_pLua) [
			class_<CObject3D>("CObject3D")
				.property("position",&CObject3D::GetPosition, &CObject3D::SetPosition)
				.property("yaw", &CObject3D::GetYaw, &CObject3D::SetYaw)
				.property("pitch", &CObject3D::GetPitch, &CObject3D::SetPitch)
				.property("roll", &CObject3D::GetRoll, &CObject3D::SetRoll)
				.property("scale", &CObject3D::GetScale, &CObject3D::SetScale)
		];

		module(_pLua) [
			class_<CThPSCamera, CNamed>("CThPSCamera")
				.def("get_direction", &CThPSCamera::GetDirection)
		];

		module(_pLua) [
			class_<CAnimatedInstanceModel, bases<CObject3D, CNamed>>("CAnimatedInstanceModel")
				.def(constructor<>())
				.def("blend_cycle", &CAnimatedInstanceModel::BlendCycle)
				.def("clear_cycle", &CAnimatedInstanceModel::ClearCycle)
				.def("execute_action", &CAnimatedInstanceModel::ExecuteAction)
				.def("get_current_animation_duration", &CAnimatedInstanceModel::GetCurrentAnimationDuration)
		];
	}

} //namespace ScriptAPI
} //namespace Graphics