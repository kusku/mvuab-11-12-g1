#pragma once

#ifndef __CLASS_ENTITY_H__
#define __CLASS_ENTITY_H__

#include <cassert>

#include "Object3D.h"
#include "MessageDefs.h"
#include "Telegram.h"

class CBaseGameEntity : public CObject3D
{
public:
	// ------------- Constructors i Destructors --------------
					CBaseGameEntity		( int _Id )			{ SetID(_Id); }
					~CBaseGameEntity	( void )			{}


	//----Main Functions --------------------------------------
	virtual void	Update				( float _ElapsedTime )		= 0;
	
	//all subclasses can communicate using messages.
	virtual bool	HandleMessage		( const Telegram& _Msg)		= 0;

	//----Properties ( get & Set )-----------------------------
	void			SetID				( int _Val );
	
	// Llamado por el constructor para asignar un ID correcto.
	int				GetID				( void ) const		{ return m_ID; }

	//----Members )--------------------------------------------
private:
	// Cada entidad tiene un identificador unico
	int				m_ID;

	// Siguiente ID válido. Cada vez que se instancie un CBaseGameEntity se actualiza este valor
	static int		m_iNextValidID;

};

#endif __CLASS_ENTITY_H__