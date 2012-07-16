#pragma once

#ifndef _GUI_REGISTER_SCRIPT_H_
#define _GUI_REGISTER_SCRIPT_H_

#include <luabind/luabind.hpp>

namespace GUI
{
	namespace ScriptAPI
	{
		void RegisterScript(lua_State *_pLua);

	} //namespace ScriptAPI

} //namespace GUI

#endif //_GUI_REGISTER_SCRIPT_H_