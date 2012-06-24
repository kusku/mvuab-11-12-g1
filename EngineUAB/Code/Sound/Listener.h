//----------------------------------------------------------------------------------
// CListener class
// Author: Marc Cuscullola
//
// Description:
// Clase que representa un micrófono en el mundo
//----------------------------------------------------------------------------------
#pragma once

#ifndef _LISTENER_H_
#define _LISTENER_H_

#include <AK/SoundEngine/Common/AkTypes.h>

//--- Foward Declarations ---
class CCamera;
//---------------------------

class CListener
{
public:
	//--- Constructor & Destructor -------------
	CListener();
	CListener( CCamera *camera );
	~CListener();

	//--- Common Methods ----------------------
	void	Update	();

	//--- Properties ---------------------------
	inline void				SetListenerPosition		( const AkListenerPosition &position )		{ m_ListenerPosition = position; }
	inline void				SetCamera				( CCamera *camera )							{ m_pCamera = camera; }

	inline const AkListenerPosition&	GetListenerPosition		() const						{ return m_ListenerPosition; }
	inline CCamera*						GetCCamera				() const						{ return m_pCamera; }

private:
	AkListenerPosition	m_ListenerPosition;
	CCamera				*m_pCamera;
};

#endif