#pragma once

#ifndef _LOGIC_REGISTER_SCRIPT_H_
#define _LOGIC_REGISTER_SCRIPT_H_

#include <luabind/luabind.hpp>

namespace Logic
{
	namespace ScriptAPI
	{
		void RegisterScript(lua_State *_pLua);

	} // namespace ScriptAPI

} //namespace Logic

#endif //_LOGIC_REGISTER_SCRIPT_H_