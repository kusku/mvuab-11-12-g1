#pragma once

#ifndef __CLASS_PLAYER_DEF__H__
#define __CLASS_PLAYER_DEF__H__

#define TEMPS_REINICI 4
#define RADI_XOC 10

#define QUANTITAT_MOVIMENT_LLUM 10.f

#define QUANTITAT_MOVIMENT 25
#define QUANTITAT_ROTACIO 5

#define FAR_RADIOUS 10

enum DIRECTION
{
	Up = 0,
	UpRight,
	Right,
	BackRight,
	Back,
	BackLeft,
	Left,
	UpLeft
};


// ********* PLAYER MOVEMENT *********** 
#define ACTION_MOVE_PLAYER_FOWARD			"MovePlayerFoward"
#define ACTION_MOVE_PLAYER_BACK				"MovePlayerBack"
#define ACTION_MOVE_PLAYER_LEFT				"MovePlayerLeft"
#define ACTION_MOVE_PLAYER_RIGHT			"MovePlayerRight"
#define ACTION_MOVE_PLAYER_UP				"MovePlayerUp"
#define ACTION_MOVE_PLAYER_DOWN 			"MovePlayerDown"


#define MOMENTUM							100.f		// Cantidad de movimiento del player
#define INCREMENT_FACTOR_VELOCITAT			50
#define TIEMPO_INCREMENTO_VELOCITDAD		30

#define ACTION_LOCK_FREE_CAMERA				"ToggleLockFreeCamera"
#define ACTION_MOVE_FREE_CAMERA_X 			"MoveXCam"
#define ACTION_MOVE_FREE_CAMERA_Y 			"MoveYCam"

#endif __CLASS_PLAYER_DEF__H__