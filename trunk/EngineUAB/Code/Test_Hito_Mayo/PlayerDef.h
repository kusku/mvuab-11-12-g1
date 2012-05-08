#pragma once

#ifndef __CLASS_PLAYER_DEF__H__
#define __CLASS_PLAYER_DEF__H__

#define TEMPS_REINICI 4
#define RADI_XOC 10

#define QUANTITAT_MOVIMENT_LLUM 10.f
#define QUANTITAT_MOVIMENT_CAMERA 10.f
#define QUANTITAT_MOVIMENT_PLAYER 10.f

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


// ********* CAMERA MOVEMENT *********** 
#define ACTION_MOVE_CAMERA_FOWARD			"MoveCameraFoward"
#define ACTION_MOVE_CAMERA_BACK				"MoveCameraBack"
#define ACTION_MOVE_CAMERA_LEFT				"MoveCameraLeft"
#define ACTION_MOVE_CAMERA_RIGHT			"MoveCameraRight"
#define ACTION_MOVE_CAMERA_UP				"MoveCameraUp"
#define ACTION_MOVE_CAMERA_DOWN 			"MoveCameraDown"

// ********* CONTROLER / PLAYER MOVEMENT *********** 

#define ACTION_MOVE_PLAYER_FOWARD			"MoveControlerFoward"
#define ACTION_MOVE_PLAYER_BACK				"MoveControlerBack"
#define ACTION_MOVE_PLAYER_LEFT				"MoveControlerLeft"
#define ACTION_MOVE_PLAYER_RIGHT			"MoveControlerRight"

#define MOMENTUM							100.f		// Cantidad de movimiento del player
#define INCREMENT_FACTOR_VELOCITAT			50
#define TIEMPO_INCREMENTO_VELOCITDAD		30

#define ACTION_LOCK_FREE_CAMERA				"ToggleLockFreeCamera"
#define ACTION_MOVE_FREE_CAMERA_X 			"MoveXCam"
#define ACTION_MOVE_FREE_CAMERA_Y 			"MoveYCam"

#endif __CLASS_PLAYER_DEF__H__