#include "_ScriptAPI/GUIRegisterScript.h"
#include "Scripting/ScriptManager.h"
#include "GUIManager.h"
#include "Controls/GUIAnimatedImage.h"
#include "DebugGUIManager.h"
#include "Console/Console.h"
#include "DebugOptions/DebugOptions.h"
#include "DebugInfo/DebugRender.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

namespace GUI
{
namespace ScriptAPI
{
	void RegisterScript(lua_State *_pLua)
	{
		module(_pLua) 
			[
				class_<CGUIManager>("CGUIManager")
					.def("active_windows", &CGUIManager::ActiveWindows)							// Activa la ventana pasada
					.def("active_windows_with_effect", &CGUIManager::ActiveWindowsWithEffect)	// Activa la ventana pasada con effecto
					.def("get_state_slider", &CGUIManager::GetStateSlider)						// Obtiene el valor del Slider indicado
					.def("set_state_slider", &CGUIManager::SetStateSlider)						// Coloca el valor del Slider indicado
					.def("get_state_check_button", &CGUIManager::GetStateCheckButton)			// Obtiene el valor del Check Button indicado
					.def("set_state_check_button", &CGUIManager::SetStateCheckButton)			// Coloca el valor del Check Button indicado
					.def("push_windows", &CGUIManager::PushWindows)								// Almacena la ventana padre i activa la pasada
					.def("pop_windows", &CGUIManager::PopWindows)								// Retorna a la ventana anterior padre
					.def("play_image", &CGUIManager::PlayImage)									// Executa animacions d'imatges
					.def("set_visible_pointer_mouse", &CGUIManager::SetVisiblePointerMouse)
			];

		module(_pLua)
			[
				class_<CGUIAnimatedImage>("CGUIAnimatedImage")
					.def("play_animation", &CGUIAnimatedImage::PlayAnimation)						// Executa animacions d'imatges
			];

		module(_pLua) 
			[
				class_<CDebugGUIManager>("CDebugGUIManager")
					.property("console", &CDebugGUIManager::GetConsole)
					.property("debug_render",&CDebugGUIManager::GetDebugRender)
					.property("debug_options", &CDebugGUIManager::GetDebugOptions)
			];

		module(_pLua) 
			[
				class_<CConsole>("CConsole")
					.def("toggle", &CConsole::Toggle)
					.def("set_active", &CConsole::SetActive)
					.def("is_active", &CConsole::IsActive)
			];

		module(_pLua) 
			[
				class_<CDebugOptions>("CDebugOptions")
					.def("set_bool", &CDebugOptions::SetBool)
					.def("set_int", &CDebugOptions::SetInt)
					.def("set_float", &CDebugOptions::SetFloat)
					.def("reload", &CDebugOptions::Reload)
			];

		module(_pLua)
			[
				class_<CDebugRender>("CDebugRender")
				.def("is_fps_visible", &CDebugRender::GetFPSVisible)
				.def("is_delta_time_visible", &CDebugRender::GetDeltaTimeVisible)
				.def("is_gamepad_visible", &CDebugRender::GetGamePadVisible)
				.def("is_camera_visible", &CDebugRender::GetCameraVisible)
				.def("is_state_visible", &CDebugRender::GetStateVisible)
				.def("toggle_fps", &CDebugRender::ToggleFPS)
				.def("toggle_delta_time", &CDebugRender::ToggleDeltaTime)
				.def("toggle_gamepad", &CDebugRender::ToggleGamePad)
				.def("toggle_camera", &CDebugRender::ToggleCamera)
				.def("toggle_state", &CDebugRender::ToggleState)
				.def("set_state_name", &CDebugRender::SetStateName)
			];
	}

} //namespace ScriptAPI
} //namespace GUI