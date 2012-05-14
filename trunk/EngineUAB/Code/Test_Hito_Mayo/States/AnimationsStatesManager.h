#pragma once

#ifndef __CLASS_ANIMATIONS_STATES_MANAGER_H__
#define __CLASS_ANIMATIONS_STATES_MANAGER_H__

#include "Utils\MapManager.h"
#include "AnimationsStates.h"

//--- Foward Declarations ---
class CXMLTreeNode;
class CAnimationsStates;
//---------------------------

class CAnimationsStatesManager : public CMapManager<CAnimationsStates>
{

public:
	//--- Init and End protocols ----------------------------------------
						CAnimationsStatesManager	( void );
						~CAnimationsStatesManager	( void );

	//---- Main Methods --------------------------------------------------
	//bool				LoadXML						( const CXMLTreeNode &_Node  );
	//bool				Reload						( void );
	void				CleanUp						( void );

	//---- Methods -------------------------------------------------------
	bool				LoadDefaultAnimationsStates	( const CXMLTreeNode &_Node );		// Carga un node por defecto de estados de animación. Esta info se almacena en el mismo manager de propiedades
	CAnimationsStates*	LoadPlayerAnimationsStates	( const CXMLTreeNode &_Node );		// Carga un node de estados de animación del player y los devuelve
	CAnimationsStates*	LoadEnemyAnimationsStates	( const CXMLTreeNode &_Node );		// Carga un node de estados de animación de un enemigo y los devuelve

private:
	// Carga un node de estados de animación y lo devuelve
	CAnimationsStates*	LoadAnimationsStates		( const CXMLTreeNode &_Node, const std::string &_Name = "", const std::string &_CoreName = "" );	

	//---- Properties ( get & Set )---------------------------------------
	//---- Members -------------------------------------------------------
};

#endif __CLASS_ANIMATIONS_STATES_MANAGER_H__