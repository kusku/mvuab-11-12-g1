#pragma once

#ifndef __CLASS_STATE_MACHINE_H__
#define __CLASS_STATE_MACHINE_H__

#include <assert.h>
#include <string>

#include "Telegram.h"
#include "State.h"

//--- Foward Declarations ---//
//class CState;
//---------------------------//

template <class T>
class CStateMachine
{
public:
	// ------------- Constructors i Destructors --------------
			CStateMachine ( T * _pOwner)
				: m_pOwner			( _pOwner )
				, m_pCurrentState	( NULL )
				, m_pPreviousState	( NULL )
				, m_pGlobalState	( NULL )
			{ return; }

			~CStateMachine	( void )	{ return; }

	// ------------- Funcions Principals ---------------------
	// Cambiamos el actual estado a uno nuevo
	void ChangeState( CState<T>* _pNewState )
	{
		assert( _pNewState && "StateMachine::ChangeState-> Trying to change to a null state");

		if( m_pCurrentState != _pNewState )
		{
			// guardo registro del anterior estado
			m_pPreviousState = m_pCurrentState;

			// llamamos al metodo exit del estado que salimos
			m_pCurrentState->OnExit ( m_pOwner );

			// Ponemos el nuevo estado
			m_pCurrentState = _pNewState;

			// Llamamos al metodo de entrada del siguiente estado
			m_pCurrentState->OnEnter( m_pOwner );
		}
	}

	// Volvemos al anterior estado
	void RevertToPreviousState( void )
	{
		ChangeState( m_pPreviousState );
	}

	// Ejecutamos el estado
	void Update( void ) 
	{
		// Si existe un estado global llamamos a su método execute
		if( m_pGlobalState )
		{
			m_pGlobalState->Execute( m_pOwner );
		}

		// Lo mimo para el estado actual
		if( m_pCurrentState ) 
		{
			m_pCurrentState->Execute( m_pOwner );
		}

	}

	void SuspedState( CState<T>* _pNewState )
	{
		assert( _pNewState && "StateMachine::SuspedState-> Trying to change to a null state");

		// guardo registro del anterior estado
		m_pPreviousState = m_pCurrentState;

		// llamamos al metodo exit del estado que salimos
		m_pCurrentState->Exit( m_pOwner );

		// Ponemos el nuevo estado
		m_pCurrentState = pNewState;

		// Llamamos al metodo de entrada del siguiente estado
		m_pCurrentState->Enter( m_pOwner );
	}

	// Gestiona los mensajes que llegan
	bool HandleMessage ( const Telegram& _Msg ) const
	{
		// Mira si el estado actual es valido y si puede gestionar el mensaje
		if ( m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, _Msg) )
		{
			return true;
		}

		// Si no, y si el estado global ha sido implementado, envia el mensaje al estado global
		if ( m_pGlobalState && m_pGlobalState->OnMessage( m_pOwner, _Msg ) )
		{
			return true;
		}

		// No puede gestionarlo
		return false;
	}

	// ------------- Propietats ( Get / Set ) ----------------
	
	// Métodos para inicializar el FSM
	void		SetCurrentState			( CState<T>* _pState )		{ m_pCurrentState	= _pState; }
	void		SetGlobalState			( CState<T>* _pState )		{ m_pGlobalState	= _pState; }
	void		SetPreviousState		( CState<T>* _pState )		{ m_pPreviousState	= _pState; }

	// Métodos para obtener los estaos de FSM
	CState<T>*	GetCurrentState			( void ) const				{ return m_pCurrentState; }
	CState<T>*	GetGlobalState			( void ) const				{ return m_pGlobalState; }
	CState<T>*	GetPreviousState		( void ) const				{ return m_pPreviousState; }

	// Devuelve True if el estado actual coincide con el estado pasado por parametro
	bool isInState ( const CState<T>& _St ) const
	{
		if ( typeid ( *m_pCurrentState ) == typeid( _St ) ) 
			return true;
		
		return false;
	}

	std::string	GetNameOfCurrentState ( void ) const
	{
		std::string s ( typeid ( *m_pCurrentState ).name() );

		// Elimina la parte 'class ' del inicio de la cadena
		if ( s.size() > 5 )
		{
			s.erase(0, 6);
		}

		return s;
	}

	// ------------- Membres -----------------------------------
private:
	// Puntero al propietario de esta instancia
	T*				m_pOwner;

	// Su estado actual
	CState<T>*		m_pCurrentState;

	//// Su previo estado para cuando ejecutemos un estado global. 
	CState<T>*		m_pPreviousState;

	//// Este estado se llama cada vez que la máquina se actualize. Este estado es un estado global que reune funcionalidad común. Cuando finaliza retorna al estado previo
	CState<T>*		m_pGlobalState;;
};

#endif __CLASS_STATE_MACHINE_H__
