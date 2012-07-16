#pragma once

#ifndef _INPUT_REGISTER_SCRIPT_H_
#define _INPUT_REGISTER_SCRIPT_H_

#include <luabind/luabind.hpp>

namespace Input
{
	namespace ScriptAPI
	{
		void RegisterScript(lua_State *_pLua);

	} //namespace ScriptAPI

} //namespace Input

#endif //_INPUT_REGISTER_SCRIPT_H_