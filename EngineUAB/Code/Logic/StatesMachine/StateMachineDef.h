#ifndef __STATE_MACHINE_DEFS_H__
#define __STATE_MACHINE_DEFS_H__

enum StateType
{
	RunAway, 
	Patrol, 
	Attack,
	roam,			// Vagar
	chase,			// Persecució
	evade,			// Evadir
	pursuit			// Cercar
};






#endif __STATE_MACHINE_DEFS_H__