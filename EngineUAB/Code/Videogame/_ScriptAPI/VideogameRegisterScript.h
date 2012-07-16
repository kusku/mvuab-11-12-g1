#pragma once

#ifndef _VIDEOGAME_REGISTER_SCRIPT_H_
#define _VIDEOGAME_REGISTER_SCRIPT_H_

#include <luabind/luabind.hpp>

namespace Videogame
{
	namespace ScriptAPI
	{
		void RegisterScript(lua_State *_pLua);

	} //namespace ScriptAPI

} //namespace Videogame

#endif //_VIDEOGAME_REGISTER_SCRIPT_H_