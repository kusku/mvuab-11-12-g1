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
					CState		( void ) {};
	virtual			~CState		( void ) {};

	// ------------- Mètodes Principals ---------------------
	
	// Se ejecuta en cada update del estado
	virtual void	Execute		( T* ) {};

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( T* ) {};

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( T* ) {};

	//// Pausa la ejecución del estado
	//virtual void	OnSuspend	( T* )						{};

	//// Permite la continuación de la ejecución del estado
	//virtual void	OnResume	( T* )						{};

	// Este se ejecuta si el agente recibe un mensaje de un dispensador de mensajes
	virtual bool	OnMessage	( T*, const Telegram& ) { return true; };

};

#endif __CLASS_STATE_H__