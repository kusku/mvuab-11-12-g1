#pragma once

#ifndef _SOUND_REGISTER_SCRIPT_H_
#define _SOUND_REGISTER_SCRIPT_H_

#include <luabind/luabind.hpp>

namespace Sound
{
	namespace ScriptAPI
	{
		void RegisterScript(lua_State *_pLua);

	} //namescape ScriptAPI

} //namespace Sound

#endif //_SOUND_REGISTER_SCRIPT_H_