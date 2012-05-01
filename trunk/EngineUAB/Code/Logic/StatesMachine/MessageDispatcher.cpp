#include "MessageDispatcher.h"
#include "BaseGameEntity.h"
#include "EntityManager.h"
#include "MessageDefs.h"

#include <iostream>
using std::cout;

#include "Utils\Timer.h"
#include "Core.h"
#include "Base.h"

#ifdef TEXTOUTPUT
#include <fstream>
	extern std::ofstream os;
#define cout os
#endif

//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CMessageDispatcher::DispatchStateMessage ( double _Delay, int _Sender, int _Receiver, int _Msg, void* _pExtraInfo )
{
	// obtengo el puntero del emisor y receptor
	CBaseGameEntity* l_pSender   = ENTITYMANAGER->GetEntityFromID(_Sender);
	CBaseGameEntity* l_pReceiver = ENTITYMANAGER->GetEntityFromID(_Receiver);

	if ( l_pReceiver == NULL )
	{
		cout << "\nWarning! No Receiver with ID of " << _Receiver << " found";
		return;
	}

	//create the telegram
	Telegram l_Telegram ( 0, _Sender, _Receiver, _Msg, _pExtraInfo );
	
	//if there is no delay, route the telegram immediately
	if ( _Delay <= 0.0 )
	{
		/*cout << "\nInstant telegram dispatched at time: " << CORE->GetTimer()->GetTotalTime()
			 << " by " << GetNameOfEntity(pSender->ID()) << " for " << GetNameOfEntity(l_pReceiver->GetID()) 
			 << ". Msg is "<< MsgToStr(msg);*/

		//send the telegram to the recipient
		Discharge( l_pReceiver, l_Telegram );
	}
	//else calculate the time when the telegram should be dispatched
	else
	{
		//double CurrentTime = CORE->GetTimer()->GetCurrentTime();
		double CurrentTime = CORE->GetTimer()->GetTotalTime();
		l_Telegram.DispatchTime	= CurrentTime + _Delay;
		//and put it in the queue
		PriorityQSet.insert( l_Telegram );

		/*cout << "\nDelayed telegram from " << GetNameOfEntity(pSender->ID()) << " recorded at time " 
			<< Clock->GetCurrentTime() << " for " << GetNameOfEntity(pReceiver->ID())
			<< ". Msg is "<< MsgToStr(msg);*/
	}
}

void CMessageDispatcher::DispatchDelayedMessages ( void )
{
	//first get current time
	//double CurrentTime = Clock->GetCurrentTime();
	double l_CurrentTime = CORE->GetTimer()->GetTotalTime();

	//now peek at the queue to see if any telegrams need dispatching.
	//remove all telegrams from the front of the queue that have gone
	//past their sell-by date
	while( ( PriorityQSet.begin()->DispatchTime < l_CurrentTime ) && ( PriorityQSet.begin()->DispatchTime > 0 ) )
	{
		//read the telegram from the front of the queue
		Telegram l_Telegram = *PriorityQSet.begin();

		//find the recipient
		CBaseGameEntity* l_pReceiver = ENTITYMANAGER->GetEntityFromID( l_Telegram.Receiver);
		
		//send the telegram to the recipient
		Discharge( l_pReceiver, l_Telegram );
		
		//and remove it from the queue
		PriorityQSet.erase(PriorityQSet.begin());
	}
}