#ifndef __MESSAGES_DEFS_H__
#define __MESSAGES_DEFS_H__

#include <string>

enum message_type
{
	Msg_Idle,		// Repòs
	Msg_Ready,		// Llest
	Msg_Sleep,		// Dormir
	Msg_Attack,		// Atacar
	Msg_RunAway,	// Fugir
	Msg_MoveAway,	// Separarse
	Msg_Patrol,		// Patrullar
	Msg_Pusuit,		// Cercar
	Msg_Roam,		// Vagar
	Msg_Evade,		// Evadir
	Msg_Chase,		// Persecució
	Msg_Push,		// Empenya
};

inline std::string MsgToStr( int _Msg )
{
	switch ( _Msg )
	{
		case Msg_Idle:
			return "Idle"; 

		case Msg_Ready:
			return "Ready";
	
		case Msg_Sleep:
			return "Sleep";

		case Msg_Attack:
			return "Attack";
		
		case Msg_RunAway:
			return "RunAway";

		case Msg_MoveAway:
			return "MoveAway";

		case Msg_Patrol:
			return "Patrol";
		
		case Msg_Pusuit:
			return "Pursuit";

		case Msg_Roam:
			return "Roam";
		
		case Msg_Evade:
			return "Evade";
		
		case Msg_Chase:
			return "Chase";

		case Msg_Push:
			return "Push";
		// TODO

		default:
			return "Not recognized!";
	}
}


#endif __MESSAGES_DEFS_H__