#pragma once

#ifndef _ACTIONTOINPUT_H
#define _ACTIONTOINPUT_H

class CInputManager;

#include "InputDefs.h"
#include "Base.h"
#include "Math\Vector2.h"
#include <Windows.h>
#include <string>

class CActionToInput
{
protected:

public:
	CActionToInput();
	virtual ~CActionToInput();

	void	Init		( HWND hWnd, const Vect2i& screenRes, bool exclusiveModeinMouse );
	void	Release		();

	void	LoadXML		( const std::string &filename );
};

#endif
