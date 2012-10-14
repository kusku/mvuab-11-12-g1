
#pragma once


#ifndef _LIMIT_MANAGER_H
#define _LIMIT_MANAGER_H

#include <string>
#include <vector>
#include "Utils\MapManager.h"
#include "Limits.h"

class CLimitManager
{
	typedef std::vector<CLimit*> LimitVect;
	
	typedef std::map<std::string, LimitVect> LimitsMap;
	typedef LimitsMap::iterator LimitsMapIt;

private:

	LimitsMap	m_LimitsMap;

	std::string		m_Filename;

	bool	AddToGroup(const std::string& group, CLimit* lim);

public:
	CLimitManager();
	virtual ~CLimitManager();

	bool			Load					(const std::string& filename);
	bool			Reload					();
	void			CleanUp					();
	void			Update					();

	bool	ActivateGroup(const std::string& group, bool activate);

	void DeActivateAll();
	void ActivateAll();
};

#endif