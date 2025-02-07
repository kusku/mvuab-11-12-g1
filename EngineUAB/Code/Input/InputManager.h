//----------------------------------------------------------------------------------
// CInputManager class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona los inputs de entrada de aplicacion; raton, teclado y joystick
// Internamente utiliza la librer�a DirectInput.
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_INPUT_MANAGER_H_
#define INC_INPUT_MANAGER_H_

#include <dinput.h>
#include "InputDefs.h"

//#include "Script/ScriptRegister.h"

//---Forward Declarations--
class CScriptManager;
class CMouse;
class CKeyboard;
class CGamePad;
//-------------------------

#include "Math\Vector2.h"
#include "Math\Vector3.h"

class CInputManager /*: public CScriptRegister*/
{
public:
	// Init and End protocols
	CInputManager(): m_bIsOk(false), m_pDI( NULL ), m_pKB( NULL ), m_pMouse( NULL ), m_pGamePad(NULL), m_bActiveMouse(true) {}
	virtual ~CInputManager() { Done(); }  

	bool		      	Init			(HWND, const Vect2i& screenRes, bool exclusiveModeinMouse);
	void		      	Done			();
	bool		      	IsOk			() const { return m_bIsOk; }

	// Poll input devices
	HRESULT		      Update			();		


	//--Query Input Data and States--------
	HRESULT	        GetPosition									(INPUT_DEVICE_TYPE, Vect2i&);		// will work for mouse and joystick only
	const Vect3i&   GetMouseDelta		      					();							                // get change in mouse position
	bool	        IsDown										(INPUT_DEVICE_TYPE, uint32);	  // will work for keyboard, mouse and joystick
	bool	        IsDownUp				      				(INPUT_DEVICE_TYPE, uint32);	  // will work for keyboard, mouse and joystick
	bool			IsUpDown				      				(INPUT_DEVICE_TYPE, uint32);	  // will work for keyboard, mouse and joystick
	bool	        HasGamePad									(INPUT_DEVICE_TYPE device = IDV_GAMEPAD1) const;	// GamePad available?
	int32			Scan2ascii									(uint32 scancode, uint16* result);
	
	//GamePad functions:
	bool			GetGamePadLeftThumbDeflection				(float *pfX, float *pfY, INPUT_DEVICE_TYPE device = IDV_GAMEPAD1);
	bool			GetGamePadRightThumbDeflection				(float *pfX, float *pfY, INPUT_DEVICE_TYPE device = IDV_GAMEPAD1);
	bool			GetGamePadDeltaTriggers						(float *pfLeft, float *pfRight, INPUT_DEVICE_TYPE device = IDV_GAMEPAD1);
	void			SetGamePadLeftMotorSpeed					(uint32 speed, INPUT_DEVICE_TYPE device = IDV_GAMEPAD1); //[0-65535]
	void			SetGamePadRightMotorSpeed					(uint32 speed, INPUT_DEVICE_TYPE device = IDV_GAMEPAD1); //[0-65535]

	CMouse*			GetMouse		() const	{ return m_pMouse; }

	void			SetActiveMouse				( bool active )			{ m_bActiveMouse = active; }
	bool			GetActiveMouse				() const				{ return m_bActiveMouse; }

	//----CScriptRegister interface-------------------
	//virtual void		RegisterFunctions								(CScriptManager* scriptManager);


private:
	void				Release			();

private:	
	bool		        m_bIsOk;	      // Initialization boolean control
	bool				m_bActiveMouse;
	LPDIRECTINPUT8		m_pDI;
	CKeyboard*			m_pKB;		      // Pointer to the Keyboard instance
	CMouse*				m_pMouse;	      // Pointer to the Mouse instance
	CGamePad*			m_pGamePad;		  // Pointer to the GamePad instance
	HWND				m_hWndMain;	    // application main window
};

#endif //INC_INPUT_MANAGER_H_

