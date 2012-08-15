//----------------------------------------------------------------------------------
// CRailManager class
// Author: Marc Cuscullola
//
// Description:
// This class manages all the rails in the game.
//----------------------------------------------------------------------------------

#pragma once

#ifndef _RAIL_MANAGER_H_
#define _RAIL_MANAGER_H_

#include "Rail.h"
#include "Utils\TemplatedVectorMapManager.h"
#include <string>

//--Forward declarations------
class CXMLTreeNode;
class CRenderManager;
//----------------------------

class CRailManager : public CTemplatedVectorMapManager< CRail >
{
public:
	//--Constructor & Destructor-----------
	CRailManager();
	~CRailManager();

	//--Main Methods-----------------------
	bool	Load		( const std::string &_Filename );
	bool	Reload		();
	void	CleanUp		();

	void	Update		( float _fElapsedTime );

	//--Control Methods--------------------
	void	StartCurrentRail		();
	void	PauseCurrentRail		();
	void	StopCurrentRail			();

	//--Set & Get Methods------------------
	void			SetCurrentRail	( const std::string &_Rail );
	CRail*			GetRail			( const std::string &_Rail )	{ return GetResource(_Rail); }
	inline CRail*	GetCurrentRail	() const						{ return m_pCurrentRail; }

	//--Debug Methods----------------------
	void	Render		( CRenderManager &_RM );

private:
	bool	LoadXML		();

private:
	std::string		m_Filename;

	CRail			*m_pCurrentRail;
};

#endif //_RAIL_MANAGER_H_