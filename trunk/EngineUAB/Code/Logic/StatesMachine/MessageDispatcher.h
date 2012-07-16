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
		
	// Envia cualquier mensaje con retardo. M�todo llamado cada vez desde el loop principal del juego
	void		DispatchDelayedMessages ( void );
	
	// ------------- M�todes -----------------------------------
private:
	// M�todo usado por DispatchMessage o DispatchDelayedMessages.
	// Este m�todo llama la funci� que gestiona el mensaje de la entidad que recibe con el telegrama m�s nuevo
	void		Discharge ( CBaseGameEntity* pReceiver, const STelegram& msg );

	// ------------- Operators -------------------------------
	CMessageDispatcher ( const CMessageDispatcher& );
	CMessageDispatcher& operator = ( const CMessageDispatcher& );


	// ------------- Membres -----------------------------------
private:
	// Se usa un std::set para contener los mensajes con retraso obteniendo una ordenaci�n autom�tica y evitar replicados. 
	// Los mensajes se ordenan por el tiempo de env�o
	std::set<STelegram> PriorityQSet;
};

// Definimos el Singlenton
//#define Dispatch MessageDispatcher::Instance()

