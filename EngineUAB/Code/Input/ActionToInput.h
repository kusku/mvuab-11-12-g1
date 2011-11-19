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

	std::map< std::string, std::vector< SInputInfo* > > m_ActionsMap;
	std::map<std::string, uint32>						m_String2Code;
	std::map<std::string, std::string>					m_String2Name;

	void InitString2Input	();
	void InitString2Name	();

public:
	CActionToInput();
	virtual ~CActionToInput();

	void	Init		( HWND hWnd, const Vect2i& screenRes, bool exclusiveModeinMouse );
	void	Update		();
	void	Release		();
	
	bool	DoAction	( const std::string &action );
	bool	DoAction	( const std::string &action, float &delta_ );

	void		GetActionKeys	( const std::string &action, std::vector<std::string> &names_ );

	void	LoadXML		( const std::string &filename );
	CInputManager* GetInputManager() const { return m_pInputManager; }
};

#endif
