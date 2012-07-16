#pragma once

#ifndef _PHYSIC_REGISTER_SCRIPT_H_
#define _PHYSIC_REGISTER_SCRIPT_H_

#include <luabind/luabind.hpp>

namespace Physic
{
	namespace ScriptAPI
	{
		void RegisterScript(lua_State *_pLua);

	} //namespace ScriptAPI

} //namespace Physic

#endif //_PHYSIC_REGISTER_SCRIPT_H_
