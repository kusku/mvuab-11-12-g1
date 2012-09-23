#include "InputRegisterScript.h"
#include "Scripting\ScriptManager.h"
#include"ActionToInput.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

namespace Input
{
namespace ScriptAPI
{
	void RegisterScript(lua_State *_pLua)
	{
		module(_pLua) [
			class_<CActionToInput>("CActionToInput")
				.def("do_action", (bool(CActionToInput::*)(const std::string&))&CActionToInput::DoAction)
				.def("do_action_mouse", &CActionToInput::DoActionMouse)
				.property("mouse_inverted", &CActionToInput::IsMouseInverted, &CActionToInput::SetMouseInverted)
		];
	}

} //namespace ScriptAPI

} //namespace Input
