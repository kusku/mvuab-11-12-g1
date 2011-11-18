#pragma once

#ifndef _ACTIONTOINPUT_H
#define _ACTIONTOINPUT_H

class CInputManager;

#include "InputDefs.h"
#include "Math\Vector2.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <map>

class CActionToInput
{
protected:
	CInputManager	*m_pInputManager;

	std::map< std::string, std::vector< SInputInfo > > m_ActionsMap;

	INPUT_DEVICE_TYPE		string2Device	( const std::string &deviceType );
	INPUT_EVENT_TYPE		string2Event	( const std::string &eventType );
	INPUT_AXIS_TYPE			string2Axis		( const std::string &axisType );

public:
	CActionToInput();
	virtual ~CActionToInput();

	void	Init		( HWND hWnd, const Vect2i& screenRes, bool exclusiveModeinMouse );
	void	Release		();

	void	LoadXML		( const std::string &filename );
};

#endif
