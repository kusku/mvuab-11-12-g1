//----------------------------------------------------------------------------------
// CItemManager class
// Author: Marc Cuscullola
//
// Description:
// Clase dedicada a gestionar los ítems del juego
//----------------------------------------------------------------------------------
#pragma once

#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

#include <string>
#include <vector>

class CRenderManager;
class CItem;

class CItemManager
{
public:
	CItemManager();
	~CItemManager();

	bool		Load		( const std::string &_Filename );
	bool		Reload		();
	void		CleanUp		();

	void		Update		( float _ElapsedTime );
	void		Render		( CRenderManager &_RM );

private:
	bool		LoadXML		();

private:
	std::string				m_Filename;		
	std::vector<CItem*>		m_ItemsVector;
};

#endif