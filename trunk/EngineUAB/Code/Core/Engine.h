#pragma once

#ifndef _ENGINE_H
#define _ENGINE_H

#include <Windows.h>
#include "EngineProcess.h"
#include "Math\Color.h"
#include "Utils\Timer.h"
#include "CoreDefs.h"

//---FOWARD DECLARATIONS --- //
class CLogger;
class CCore;
//---------------------------//

class CEngine
{
public:
	//--- Init and End protocols------------------------------------------
					CEngine				( void );
	virtual			~CEngine			( void );

	bool			Init			( HWND _HWnd );
	void			Done			( void );
	bool			IsOk			( void ) const { return m_bIsOk; }

	//----Main Methods -----------------------------------------------------
	void			Update				( void );
	void			Render				( void );
	void			RenderScene			( CRenderManager* _RM );

private:
	void			Release				( void );

	//---- Methods ---------------------------------------------------------
public:
	void			LoadConfigXML		( const std::string &_ConfigFile );
	void			Reload				( void );
	
	//----Properties ( get & Set )---------------------------------------
	void			SetProcess			( CEngineProcess *_Process );
	CEngineProcess*	GetProcess			( void ) const						{ return m_pProcess; }

	Vect2i			GetWindowResolution	( void ) const						{ return m_Config.windowResolution; }
	Vect2i			GetResolution		( void ) const						{ return m_Config.resolution; }
	Vect2i			GetPosition			( void ) const						{ return m_Config.position; }

	const SConfig&	GetConfig			( void ) const						{ return m_Config; }

	void			SetIncreaseTimer	( float _Increase ) 				{ m_TimerIncreasing = _Increase; }

	//----Members -------------------------------------------------------
protected:
	bool			m_bIsOk;			// em diu si tot és correcte o no en certes operacions

	const CColor	string2Color		( const std::string &color );
	void			UpdateDebugInputs	( void );	


	CCore			*m_pCore;
	CEngineProcess	*m_pProcess;
	CLogger			*m_pLogger;

	CTimer			m_Timer;

	SConfig			m_Config;

	float			m_TimerIncreasing;		// Valor el qual permitirá multiplicar por el timer para acelerar/desacelerar el juego
};

#endif