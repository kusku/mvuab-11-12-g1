#pragma once

#ifndef _ITEMMANAGER_H
#define _ITEMMANAGER_H

class CItem;
#include <vector>
#include "Math\Vector2.h"

class CItemManager
{
protected:
	std::vector< CItem *> m_ItemVector;

public:
	CItemManager();
	virtual ~CItemManager();

	void	Release	();

	void	Init(int numItems, const Vect2i &screen);
	void	Update();
	void	Render();

	bool	Impact2Bonus	( const Vect2f &player );
	bool	Impact2Stone	( const Vect2f &player );
};

#endif
