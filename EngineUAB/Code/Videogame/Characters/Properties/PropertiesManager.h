#pragma once

#ifndef __CLASS_PROPERTIES2_MANAGER_H__
#define __CLASS_PROPERTIES2_MANAGER_H__

#include "Utils\MapManager.h"
#include "Properties.h"

//--- Foward Declarations ---
class CXMLTreeNode;
//class CProperties;
//---------------------------

class CPropertiesManager : public CMapManager<CProperties>
{

public:
	//--- Init and End protocols ----------------------------------------
					CPropertiesManager			( void );
					~CPropertiesManager			( void );

	//---- Main Methods --------------------------------------------------
	//bool			LoadXML						( const CXMLTreeNode &_Node  );
	//bool			Reload						( void );
	void			CleanUp						( void );

	//---- Methods -------------------------------------------------------
	bool			LoadDefaultProperties		( const CXMLTreeNode &_Node );					// Carga un node por defecto de propiedades. Esta info se almacena en el mismo manager de propiedades
	CProperties*	LoadPlayerProperties		( const CXMLTreeNode &_Node );					// Carga un node de propiedades del player y lo devuelve
	CProperties*	LoadEnemyProperties			( const CXMLTreeNode &_Node );					// Carga un node de propiedades de un enemigo y lo devuelve

	static void		RegisterMethods				();

private:
	// Carga un nodo de propiedades y lo devuelve
	CProperties*	LoadProperties				( const CXMLTreeNode &_Node, const std::string &_Name = "", const std::string &_CoreName = "", const std::string &_AnimatedInstance = "" );		

	//---- Properties ( get & Set )---------------------------------------
	//---- Members -------------------------------------------------------
};

#endif __CLASS_PROPERTIES2_MANAGER_H__