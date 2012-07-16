#pragma once

#ifndef _GRAPHICS_REGISTER_SCRIPT_H_
#define _GRAPHICS_REGISTER_SCRIPT_H_

#include <luabind/luabind.hpp>

namespace Graphics
{
	namespace ScriptAPI
	{
		void RegisterScript(lua_State *_pLua);

	} //namespace ScriptAPI

} //namespace Gaphics

#endif //_GRAPHICS_REGISTER_SCRIPT_H_