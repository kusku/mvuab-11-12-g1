#pragma once

#ifndef _ITEM_H
#define _ITEM_H

#include "..\defines.h"
#include "Math\Vector2.h"

class CItem
{
protected:
	ITEM_TYPE m_Type;
	Vect2f m_Position;

public:
	CItem();
	~CItem();
	
	void	Render ();

	bool	Impact ( const Vect2f &player );

	void		SetPosition	( const Vect2f &position )	{ m_Position = position; }
	void		SetType		( ITEM_TYPE type )			{ m_Type = type; }

	Vect2f		GetPosition() const		{ return m_Position; }
	ITEM_TYPE	GetType() const			{ return m_Type; }
};

#endif
