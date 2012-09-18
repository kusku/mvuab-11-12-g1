#pragma once

#ifndef __CLASS_STATE_H__
#define __CLASS_STATE_H__

#include "MessageDefs.h"
#include "Particles\ParticleEmitterManager.h"

//--- Foward Declarations ---//
class CParticleEmitter;
//---------------------------//

struct STelegram;

#include "Utils\Named.h"
#include <string>

template <class T>
class CState : public CNamed
{
public:
	// ------------- Constructors i Destructors ----------------
					CState		( T* )							{ return; }
					CState		( T* , const std::string &_Name )	{ SetName(_Name); }
	virtual			~CState		( void ) {};

	// ------------- Mètodes Principals ------------------------
	
	// Se ejecuta en cada update del estado
	virtual void	Execute		( T*, float _ElapsedTime ) {
											return;			
									   };

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( T* ) {
											return;			
									   };

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( T* ) {
											return;			
									   };

	//// Pausa la ejecución del estado
	//virtual void	OnSuspend	( T* )						{};

	//// Permite la continuación de la ejecución del estado
	//virtual void	OnResume	( T* )						{};

	// Este se ejecuta si el agente recibe un mensaje de un dispensador de mensajes
	virtual bool	OnMessage	( T*, const STelegram& ) { 
															return true; 
														};
	
	//----Properties ------------------------------------------
	CParticleEmitter*	GetParticleEmitter( const std::string & _EmitterName ) const { return CORE->GetParticleEmitterManager()->GetResource(_EmitterName); }


	// ------------- Members ---------------------------------
private:

};

#endif __CLASS_STATE_H__