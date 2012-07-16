#pragma once

#ifndef _CORE_REGISTER_SCRIPT_H_
#define _CORE_REGISTER_SCRIPT_H_

#include <luabind\luabind.hpp>

namespace Core
{
	namespace ScriptAPI
	{
		void RegisterScript();
		void RegisterMathScript(lua_State *_pLua);

	} //namespace ScriptAPI

} //namespace Core

#endif //_CORE_REGISTER_SCRIPT_H_