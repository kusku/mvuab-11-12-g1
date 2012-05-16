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

enum TIPUS_ACTOR
{
	ENEMY = 0,
	PLAYER
};

enum ESTADOS_ITEM
{
	EXIST = 0,
	GOT
};

enum TYPES_CAMERA
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