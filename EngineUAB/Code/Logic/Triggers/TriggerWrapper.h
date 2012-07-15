#pragma once

#ifndef _TRIGGER_WRAPPER_H_
#define _TRIGGER_WRAPPER_H_

#include "Triggers/Types/BoxTrigger.h"
#include "Triggers\Types\SphereTrigger.h"
#include "Scripting\ScriptManager.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// ----------------------------------------
//		WRAPPER PARA LUA
// ----------------------------------------
struct Box_Trigger_Wrapper: CBoxTrigger, luabind::wrap_base
{
	Box_Trigger_Wrapper()
		: CBoxTrigger()
	{
	}

	virtual void Update(float _fElapsedTime)
	{
		call<void>("update", _fElapsedTime);
	}

	virtual void OnEnter()
	{
		call<void>("on_enter");
	}

	virtual void OnStay()
	{
		call<void>("on_stay");
	}

	virtual void OnExit()
	{
		call<void>("on_exit");
	}
};

struct Sphere_Trigger_Wrapper: CSphereTrigger, luabind::wrap_base
{
	Sphere_Trigger_Wrapper()
		: CSphereTrigger()
	{
	}

	virtual void Update(float _fElapsedTime)
	{
		call<void>("update", _fElapsedTime);
	}

	virtual void OnEnter()
	{
		call<void>("on_enter");
	}

	virtual void OnStay()
	{
		call<void>("on_stay");
	}

	virtual void OnExit()
	{
		call<void>("on_exit");
	}
};

#endif //_TRIGGER_WRAPPER_H_