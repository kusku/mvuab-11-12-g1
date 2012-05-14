#ifndef __TEST_DEF_H__
#define __TEST_DEF_H__

enum TECLES 
{
	UP		= 0,
	DOWN	= 1,
	LEFT	= 2,
	RIGHT	= 3,
	NO_KEY	= 4
};

// ********* VIEW CAMERA *********** 
#define ACTION_CAMERA_SWITCH				"CommutacioCamera"
#define ACTION_PLAYER_SWITCH				"CommutacioPlayer"

enum ESTADOS
{
	DEFAULT = 0,
	DISPARA,				// Només player
	PARADO,
	PERSEGUIM,
	VOLVIENDO,
	BUSCANDO,
	RUTEANDO_ALEATORI,
	RUTEANDO_SEQUENCIAL,
	PILLADO,				// Només player
	BUSCANDO_CURA,
	MOVIENDO_HACIA_CURA
	
};

enum ESTADOS_DISPARO
{
	DISPARO = 0,
	ENRUTA,
	IMPACTO,
	DESAPARICION
};

enum TIPO_ACTOR
{
	ENEMY = 0,
	PLAYER
};

enum TIPO_ENEMIGO
{
	LOBO = 0,
	CONEJO
};

enum ESTADOS_ITEM
{
	EXIST = 0,
	GOT
};

enum TIPO_CAMERA
{
	FIX = 0,
	RAIL,
	RAIL_AND_BACK,
	INTELLIGENCE,
	INTELLIGENCE_AND_BACK
};

// ---------- ITEMS
#define MAX_NUM_ITEMS	 4
#define VIDA_INICIAL_PLAYER		10
#define VIDA_INICIAL_ENEMY		30
#define VIDA_INICIAL_ITEM		1
#endif __TEST_DEF_H__