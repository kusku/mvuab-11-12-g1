//----------------------------------------------------------------------------------
// CActionToInput class
// Author: Marc Cuscullola
//
// Description:
// Esta clase abstrae el control de los periféricos de entrada. Asocia una acción
// de un input con un nombre de acción.
// Internamente utiliza la clase CInputManager
//----------------------------------------------------------------------------------
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
public:
	// Init and End protocols
	CActionToInput();
	virtual ~CActionToInput();

	bool	Init		( HWND hWnd, const Vect2i& screenRes, bool exclusiveModeinMouse );
	void	Done		();
	bool	IsOk		( void ) const { return m_bIsOk; }

	// Read Data
	void	LoadXML		( const std::string &filename );
	void	Reload		();

	// Poll input devices
	void	Update		();
	
	bool	DoAction	( const std::string &action );
	bool	DoAction	( const std::string &action, float &delta_ );
	float	DoActionMouse( const std::string &action );

	//-----Get Methods-------
	void			GetActionInfo	( const std::string &action, std::string &keys_ );
	bool			GetGamepadState ( void ) const;
	bool			IsMouseInverted	() const	{ return m_bMouseInverted; }

	void			SetMouseInverted ( bool value )			{ m_bMouseInverted = value; }

	inline CInputManager*	GetInputManager( void ) const { return m_pInputManager; }
	

private:
	void	Release				();

	void	InitString2Input	();
	void	InitString2Name		();

private:
	bool	m_bIsOk;
	bool	m_bMouseInverted;

	CInputManager	*m_pInputManager;

	typedef std::map< std::string, std::vector< SInputInfo* > > TMapActions;
	TMapActions											m_ActionsMap;
	std::map<std::string, uint32>						m_String2Code;
	std::map<std::string, std::string>					m_String2Name;
	std::string											m_FilePath;
};

#endif
