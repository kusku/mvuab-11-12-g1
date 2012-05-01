#ifndef __MESSAGES_DEFS_H__
#define __MESSAGES_DEFS_H__

#include <string>

enum message_type
{
	Msg_Home,
	Msg_Ready,
	Msg_Sleep,
	Msg_Attack,
	Msg_RunAway,
	Msg_Patrol,
};

inline std::string MsgToStr( int _Msg )
{
	switch ( _Msg )
	{
		case Msg_Home:
			return "Home"; 

		case Msg_Ready:
			return "Ready";

		// TODO

		default:
			return "Not recognized!";
	}
}


#endif __MESSAGES_DEFS_H__