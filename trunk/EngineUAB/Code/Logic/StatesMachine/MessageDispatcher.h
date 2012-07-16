#pragma once

//#define MESSAGEDISPATHER CMessageDispatcher::GetSingletonPtr()

#include <set>
#include "MessageDefs.h"
#include "Telegram.h"

#include "Utils\Singleton.h"

const double	SEND_MSG_IMMEDIATELY	= 0.0f;
const int		NO_ADDITIONAL_INFO		= 0;

#include "Scripting\ScriptManager.h"
#include "Base.h"

//--- Foward Declarations ---//
class CBaseGameEntity;
//---------------------------//

class CMessageDispatcher : public CSingleton<CMessageDispatcher>
{
	// ------------- Constructors i Destructors ----------------
public:
				CMessageDispatcher	( void )		{}
				~CMessageDispatcher	( void )		{}

public:
	//	Singleton
	static		CMessageDispatcher* Instance();

	// ------------- Funcions Principals ---------------------
	// Envia un mensaje a otro agente
	void		DispatchStateMessage ( double _Delay, int _Sender, int _Receiver, int _Msg, void* _pExtraInfo );
		
	// Envia cualquier mensaje con retardo. Método llamado cada vez desde el loop principal del juego
	void		DispatchDelayedMessages ( void );
	
	// ------------- Mètodes -----------------------------------
private:
	// Método usado por DispatchMessage o DispatchDelayedMessages.
	// Este método llama la funció que gestiona el mensaje de la entidad que recibe con el telegrama más nuevo
	void		Discharge ( CBaseGameEntity* pReceiver, const STelegram& msg );

	// ------------- Operators -------------------------------
	CMessageDispatcher ( const CMessageDispatcher& );
	CMessageDispatcher& operator = ( const CMessageDispatcher& );


	// ------------- Membres -----------------------------------
private:
	// Se usa un std::set para contener los mensajes con retraso obteniendo una ordenación automática y evitar replicados. 
	// Los mensajes se ordenan por el tiempo de envío
	std::set<STelegram> PriorityQSet;
};

// Definimos el Singlenton
//#define Dispatch MessageDispatcher::Instance()

