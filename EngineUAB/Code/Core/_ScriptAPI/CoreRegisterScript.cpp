#include "CoreRegisterScript.h"
#include "Scripting\ScriptManager.h"
#include "_ScriptAPI\GraphicsRegisterScript.h"
#include "_ScriptAPI\LogicRegisterScript.h"
#include "_ScriptAPI\SoundRegisterScript.h"
#include "_ScriptAPI\GUIRegisterScript.h"
#include "_ScriptAPI\InputRegisterScript.h"
#include "_ScriptAPI\PhysicRegisterScript.h"
#include "EngineProcess.h"
#include "utils/Named.h"
#include "Base.h"
#include "Core.h"
#include "StatesMachine\MessageDispatcher.h"
#include "DebugGUIManager.h"
#include "Stadistics\Stadistics.h"
#include "GUIManager.h"
#include "SoundManager.h"
#include "Movement\WayPointManager.h"
#include "ActionToInput.h"
#include "PhysicsManager.h"
#include "StatesMachine\EntityManager.h"
#include "Stadistics\Stadistics.h"
#include "Rails\RailManager.h"

#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Math\Matrix33.h"
#include "Math\Matrix44.h"
#include "Math\Matrix34.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

namespace Core
{
namespace ScriptAPI
{
	void RegisterScript()
	{
		lua_State *l_pState = CORE->GetScriptManager()->GetLuaState();

		module(l_pState) 
		[
			class_<CEngineProcess>("CEngineProcess")
		];

		module(l_pState) [
			class_<CSingleton<CCore>>("CSingleton_CCore")
				.scope 
				[
					def("get_singleton", &CSingleton<CCore>::GetSingletonPtr)
				]
		];

		module(l_pState) 
			[
				class_<CCore>("CCore")
					.def("reload_all", &CCore::Reload)
					.def("reload_fonts", &CCore::ReloadTTFs)
					.def("reload_languages", &CCore::ReloadLanguages)
					.def("reload_inputs", &CCore::ReloadInputs)
					.def("reload_render_commands", &CCore::ReloadSceneRendererCommandManager)
					.def("reload_renderable_objects_layers", &CCore::ReloadRenderableObjectsLayersManager)
					.def("reload_effects", &CCore::ReloadEffects)
					.def("reload_meshes", &CCore::ReloadMeshes)
					.def("reload_pools", &CCore::ReloadPools)
					.def("reload_scripts", &CCore::ReloadScripts)
					.def("reload_lights", &CCore::ReloadLights)
					.def("reload_physics", &CCore::ReloadPhysics)
					.def("reload_billboards", &CCore::ReloadBillboards)
					.def("reload_particles", &CCore::ReloadParticles)
					.def("reload_triggers", &CCore::ReloadTriggers)
					.def("reload_gui", &CCore::ReloadGUI)
					.def("reload_sounds", &CCore::ReloadSounds)
					.def("reload_waypoints", &CCore::ReloadWayPoints)
					.def("reload_steering_behaviors_settings", &CCore::ReloadSteeringBehaviorSettings)
					.def("reload_rails", &CCore::ReloadRails)
					.def("reload_physxobjs", &CCore::ReloadPhysXObjs)
					.def("get_debug_gui_manager", &CCore::GetDebugGUIManager)
					.def("get_stadistics", &CCore::GetStadistics)
					.def("get_gui_manager", &CCore::GetGUIManager)
					.def("get_sound_manager", &CCore::GetSoundManager)
					.def("get_waypoints_manager", &CCore::GetWayPointManager)
					.def("get_action_to_input", &CCore::GetActionToInput)
					.def("get_physics_manager", &CCore::GetPhysicsManager)
					.def("get_process", &CCore::GetProcess)
					.def("get_entity_manager", &CCore::GetEntityManager)
					.def("get_message_dispatchers", &CCore::GetMessageDispatcher)
					.def("get_rail_manager", &CCore::GetRailManager)
					.def("load_fonts", &CCore::LoadFonts)
					.def("load_languages", &CCore::LoadLanguages)
					.def("load_inputs", &CCore::LoadInputs)
					.def("load_effects", &CCore::LoadEffects)
					.def("load_renderable_objects_techniques", &CCore::LoadROTechniques)
					.def("load_static_meshes", &CCore::LoadStaticMeshes)
					.def("load_animated_models", &CCore::LoadAnimatedModels)
					.def("load_renderable_objects_layers", &CCore::LoadROLayers)
					.def("load_lights", &CCore::LoadLights)
					.def("load_render_commands", &CCore::LoadRenderCommands)
					.def("load_physics", &CCore::LoadPhysics)
					.def("load_billboards", &CCore::LoadBillboards)
					.def("load_particles", &CCore::LoadParticles)
					.def("load_gui", &CCore::LoadGUI)
					.def("load_triggers", &CCore::LoadTriggers)
					.def("load_sounds", &CCore::LoadSounds)
					.def("load_debug_gui", &CCore::LoadDebugGUI)
					.def("load_waypoints", &CCore::LoadWaypoints)
					.def("load_animal_movement", &CCore::LoadAnimalMovements)
					.def("load_steering_behaviors_settings", &CCore::LoadSteeringBehaviorSettings)
					.def("load_rails", &CCore::LoadRails)
					.def("load_physxobjs", &CCore::LoadPhysXObjs)
					.def("is_game_mode", &CCore::IsGameMode)
					.def("is_debug_mode", &CCore::IsDebugMode)
					.def("is_release_mode", &CCore::IsReleaseMode)
					.def("unload_static_meshes", &CCore::UnloadStaticMeshes)
					.def("unload_animated_models", &CCore::UnloadAnimatedModels)
					.def("unload_renderable_objects_layers", &CCore::UnloadRenderableObjectsLayers)
					.def("unload_lights", &CCore::UnloadLights)
					.def("unload_billboards", &CCore::UnloadBillboards)
					.def("unload_particles", &CCore::UnloadParticles)
					.def("unload_triggers", &CCore::UnloadTriggers)
					.def("unload_animal_movement", &CCore::UnloadAnimalMovements)
					.def("Unload_waypoints", &CCore::UnloadWayPoints)
					.def("unload_physx_objs", &CCore::UnloadPhysXObjs)
					.def("set_game_mode", &CCore::SetGameMode)
			];

		module(l_pState)
			[
				class_<CNamed>("CNamed")
					.property("name", &CNamed::GetName, &CNamed::SetName)
			];


		module(l_pState)
			[
				class_<CStadistics>("CStadistics")
					.def("get_draw_calls", &CStadistics::GetNumOfDrawCalls)
					.def("get_draw_debug_lines", &CStadistics::GetNumOfDebugLines)
					.def("get_vertices_in_frustum", &CStadistics::GetNumOfVerticesInFrustum)
					.def("get_triangles_in_frustum", &CStadistics::GetNumOfTrianglesInFrustum)
			];

		
		Graphics::ScriptAPI::RegisterScript(l_pState);
		Input::ScriptAPI::RegisterScript(l_pState);
		Logic::ScriptAPI::RegisterScript(l_pState);
		GUI::ScriptAPI::RegisterScript(l_pState);
		Physic::ScriptAPI::RegisterScript(l_pState);
		Sound::ScriptAPI::RegisterScript(l_pState);
		Core::ScriptAPI::RegisterMathScript(l_pState);
	}

	void RegisterMathScript(lua_State *_pLua)
	{
		module(_pLua) [
			class_<Vect2f>("Vect2f")
				.def(constructor<float, float>())
				.def(const_self + const_self)
				.def(const_self - const_self)
				.def(const_self * const_self)
				.def(const_self + float())
				.def(const_self - float())
				.def(const_self * float())
				.def(const_self / float())
				.def(const_self == const_self)
				.def("perpendicular", &Vect2f::GetPerpendicular)
				.def("normalize", &Vect2f::Normalize)
				.def("length", &Vect2f::Length)
				.def("distance", &Vect2f::Distance)
				.def("sq_distance", &Vect2f::SqDistance)
				.def("zero", &Vect2f::SetZero)
				.def_readwrite("x", &Vect2f::x)
				.def_readwrite("y", &Vect2f::y)
		];

		module(_pLua) [
			class_<Vect3f>("Vect3f")
				.def(constructor<float, float, float>())
				.def(const_self + const_self)
				.def(const_self - const_self)
				.def(const_self * const_self)
				.def(const_self + float())
				.def(const_self - float())
				.def(const_self * float())
				.def(const_self / float())
				.def(const_self == const_self)
				.def("rotate_y", &Vect3f::RotateY)
				.def("perpendicular", &Vect3f::GetPerpendicular)
				.def("normalize", &Vect3f::Normalize)
				.def("length", &Vect3f::Length)
				.def("squared_length", &Vect3f::SquaredLength)
				.def("distance", &Vect3f::Distance)
				.def("zero", &Vect3f::SetZero)
				.def("is_zero", &Vect3f::IsZero)
				.def("truncate", &Vect3f::Truncate)
				.def("dot", &Vect3f::Dot)
				.def("cross", &Vect3f::Cross)
				.def("angle_with_vector", &Vect3f::AngleWithVector)
				.def_readwrite("x", &Vect3f::x)
				.def_readwrite("y", &Vect3f::y)
				.def_readwrite("z", &Vect3f::z)
		];

		module(_pLua) [
			class_<Vect4f>("Vect4f")
				.def(constructor<float, float, float, float>())
				.def(const_self + const_self)
				.def(const_self - const_self)
				.def(const_self * const_self)
				.def(const_self + float())
				.def(const_self - float())
				.def(const_self * float())
				.def(const_self / float())
				.def(const_self == const_self)
				.def("normalize", &Vect4f::Normalize)
				.def("length", &Vect4f::Length)
				.def("zero", &Vect4f::SetZero)
				.def_readwrite("x", &Vect4f::x)
				.def_readwrite("y", &Vect4f::y)
				.def_readwrite("z", &Vect4f::z)
				.def_readwrite("w", &Vect4f::w)
		];

		module(_pLua) [
			class_<Vect2i>("Vect2i")
				.def(constructor<int, int>())
				.def(const_self + const_self)
				.def(const_self - const_self)
				.def(const_self * const_self)
				.def(const_self + int())
				.def(const_self - int())
				.def(const_self * int())
				.def(const_self / int())
				.def(const_self == const_self)
				.def("zero", &Vect2i::SetZero)
				.def_readwrite("x", &Vect2i::x)
				.def_readwrite("y", &Vect2i::y)
		];

		module(_pLua) [
			class_<Vect3i>("Vect3i")
				.def(constructor<int, int, int>())
				.def(const_self + const_self)
				.def(const_self - const_self)
				.def(const_self * const_self)
				.def(const_self + int())
				.def(const_self - int())
				.def(const_self * int())
				.def(const_self / int())
				.def(const_self == const_self)
				.def("zero", &Vect3i::SetZero)
				.def_readwrite("x", &Vect3i::x)
				.def_readwrite("y", &Vect3i::y)
				.def_readwrite("z", &Vect3i::z)
		];

		module(_pLua) [
			class_<Vect4i>("Vect4i")
				.def(constructor<int, int, int, int>())
				.def(const_self + const_self)
				.def(const_self - const_self)
				.def(const_self * const_self)
				.def(const_self + int())
				.def(const_self - int())
				.def(const_self * int())
				.def(const_self / int())
				.def(const_self == const_self)
				.def("zero", &Vect4i::SetZero)
				.def_readwrite("x", &Vect4i::x)
				.def_readwrite("y", &Vect4i::y)
				.def_readwrite("z", &Vect4i::z)
				.def_readwrite("w", &Vect4i::w)
		];

		module(_pLua) [
			class_<Mat33f>("Mat33f")
				.def(constructor<float, float, float, float, float, float, float, float, float>())
				.def(const_self + const_self)
				.def(const_self - const_self)
				.def(const_self * const_self)
				.def(const_self * float())
				.def(const_self / float())
				.def(const_self == const_self)
				.def("identity", &Mat33f::SetIdentity)
				.def("zero", &Mat33f::SetZero)
				.def("reset_rotation", &Mat33f::ResetRotation)
				.def("reset_scale", &Mat33f::ResetScale)
				.def("set_from_angle_x", &Mat33f::SetFromAngleX)
				.def("set_from_angle_y", &Mat33f::SetFromAngleY)
				.def("set_from_angle_z", &Mat33f::SetFromAngleZ)
				.def("set_from_angles_xz", &Mat33f::SetFromAnglesXZ)
				.def("set_from_angles_yxz", &Mat33f::SetFromAnglesYXZ)
				.def("set_rot_by_angle_x", &Mat33f::SetRotByAngleX)
				.def("set_rot_by_angle_y", &Mat33f::SetRotByAngleY)
				.def("set_rot_by_angle_z", &Mat33f::SetRotByAngleZ)
				.def("set_rot_by_angles_xz", &Mat33f::SetRotByAnglesXZ)
				.def("set_rot_by_angles_yxz", &Mat33f::SetRotByAnglesYXZ)
				.def("get_scale", &Mat33f::GetScale)
				.def("get_scale_x", &Mat33f::GetScaleX)
				.def("get_scale_y", &Mat33f::GetScaleY)
				.def("get_scale_z", &Mat33f::GetScaleZ)
				.def("get_angle_x", &Mat33f::GetAngleX)
				.def("get_angle_y", &Mat33f::GetAngleY)
				.def("get_angle_z", &Mat33f::GetAngleZ)
				.def("get_yaw", &Mat33f::GetYaw)
				.def("get_pitch", &Mat33f::GetPitch)
				.def("get_roll", &Mat33f::GetRoll)
				.def("get_pitch_roll_yaw", &Mat33f::GetPitchRollYaw)
				.def("set_pitch_roll_yaw", &Mat33f::SetPitchRollYaw)
				.def("invert", &Mat33f::Invert)
				.def("transpose", &Mat33f::Transpose)
				.def_readwrite("m00", &Mat33f::m00)
				.def_readwrite("m01", &Mat33f::m01)
				.def_readwrite("m02", &Mat33f::m02)
				.def_readwrite("m10", &Mat33f::m10)
				.def_readwrite("m11", &Mat33f::m11)
				.def_readwrite("m12", &Mat33f::m12)
				.def_readwrite("m20", &Mat33f::m20)
				.def_readwrite("m21", &Mat33f::m21)
				.def_readwrite("m22", &Mat33f::m22)	
		];
	}

} //namespace ScriptAPI
} //namespace Core