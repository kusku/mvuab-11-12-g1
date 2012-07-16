#pragma once

#ifndef __SCRIPTING_DEFS_H__
#define __SCRIPTING_DEFS_H__

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include <luabind/return_reference_to_policy.hpp>
#include <luabind/out_value_policy.hpp>
#include <luabind/exception_handler.hpp>

using namespace luabind;

#endif __SCRIPTING_DEFS_H__