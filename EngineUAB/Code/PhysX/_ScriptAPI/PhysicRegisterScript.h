#pragma once

#ifndef _PHYSIC_REGISTER_SCRIPT_H_
#define _PHYSIC_REGISTER_SCRIPT_H_

#include <luabind/luabind.hpp>

#define NOMINMAX
#include "NxJoint.h"
#include "NxFixedJointDesc.h"
#include "NxFixedJoint.h"
#include "NxSphericalJointDesc.h"
#include "NxSphericalJoint.h"
#include "NxRevoluteJointDesc.h"
#include "NxRevoluteJoint.h"


namespace Physic
{
	namespace ScriptAPI
	{
		void RegisterScript(lua_State *_pLua);

	} //namespace ScriptAPI

} //namespace Physic

#endif //_PHYSIC_REGISTER_SCRIPT_H_
