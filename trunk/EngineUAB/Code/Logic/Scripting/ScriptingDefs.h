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

using namespace luabind;

#include "Core.h"
#include "Utils\Singleton.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Math\Matrix33.h"
#include "Math\Matrix44.h"
#include "Math\Matrix34.h"
#include "Console\Console.h"
#include "Stadistics\Stadistics.h"
#include "DebugOptions\DebugOptions.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "SoundManager.h"
#include "Triggers\TriggersManager.h"
#include "StatesMachine\State.h"
#include "StatesMachine\StateMachine.h"
#include "StatesMachine\BaseGameEntity.h"
#include "GUIManager.h"
#include "Controls\GUIAnimatedImage.h"
#include "EngineProcess.h"

#endif __SCRIPTING_DEFS_H__