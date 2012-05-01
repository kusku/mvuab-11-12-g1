#pragma once

#ifndef __CLASS_STATE_H__
#define __CLASS_STATE_H__

#include "MessageDefs.h"

//--- Foward Declarations ---//
//---------------------------//

struct Telegram;

template <class T>
class CState
{
public:
	// ------------- Constructors i Destructors ----------------
					//CState		( void ) {};
					~CState		( void ) {}

	// ------------- Mètodes Principals ---------------------
	
	// Se ejecuta en cada update del estado
	virtual void	Execute		( T* )	= 0;

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( T* )						= 0;

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( T* )						= 0;

	// Pausa la ejecución del estado
	virtual void	OnSuspend	( T* )						= 0;

	// Permite la continuación de la ejecución del estado
	virtual void	OnResume	( T* )						= 0;

	// Este se ejecuta si el agente recibe un mensaje de un dispensador de mensajes
	virtual bool	OnMessage	( T*, const Telegram& )		= 0;

};

#endif __CLASS_STATE_H__