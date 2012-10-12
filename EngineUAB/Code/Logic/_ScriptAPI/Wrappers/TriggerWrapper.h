#pragma once

#ifndef _TRIGGER_WRAPPER_H_
#define _TRIGGER_WRAPPER_H_

#include "Triggers/Types/BoxTrigger.h"
#include "Triggers\Types\SphereTrigger.h"
#include "Scripting\ScriptManager.h"
#include "PhysicUserData.h"

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

	virtual void OnEnter(CPhysicUserData* _pUserData)
	{
		call<void>("on_enter", _pUserData);
	}

	virtual void OnStay(CPhysicUserData* _pUserData)
	{
		call<void>("on_stay", _pUserData);
	}

	virtual void OnExit(CPhysicUserData* _pUserData)
	{
		call<void>("on_exit", _pUserData);
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

	virtual void OnEnter(CPhysicUserData* _pUserData)
	{
		call<void>("on_enter", _pUserData);
	}

	virtual void OnStay(CPhysicUserData* _pUserData)
	{
		call<void>("on_stay", _pUserData);
	}

	virtual void OnExit(CPhysicUserData* _pUserData)
	{
		call<void>("on_exit", _pUserData);
	}
};

#endif //_TRIGGER_WRAPPER_H_