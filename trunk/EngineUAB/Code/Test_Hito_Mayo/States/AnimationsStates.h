#pragma once
#ifndef __CLASS_ANIMATIONS_STATES_H__
#define __CLASS_ANIMATIONS_STATES_H__

#include <string>
#include <vector>
#include <map>
#include "Object3D.h"
#include "Utils\Named.h"
#include "Math\Vector3.h"
#include "Utils\MapManager.h"

class CAnimationsStates : public CMapManager<std::vector<uint16>> // std::map<std::string, std::vector<int>>
{
public:
	//--- Init and End protocols ----------------------------------------
					CAnimationsStates	( std::string _Name );
					CAnimationsStates	( void );
					~CAnimationsStates	( void );

	//---- Main Methods --------------------------------------------------
	//---- Methods -------------------------------------------------------
	//---- Properties ( get & Set )---------------------------------------

	inline void			SetCore			( const std::string &_CoreName )		{ m_Core = _CoreName; }
	inline std::string	GetCore			( void ) const							{ return m_Core; }

	//---- Members -------------------------------------------------------
private:
	std::string		m_Core;
};

#endif __CLASS_PROPERTIES_H__