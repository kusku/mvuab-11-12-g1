#pragma once

#ifndef _ACTIVE_H
#define _ACTIVE_H

#include <string>

class CActive
{
protected:
	bool		m_Active;

public:
	CActive()
		: m_Active(true)
	{
	}

	virtual ~CActive()
	{
	}

	void SetActive ( bool IsActive )
	{
		m_Active = IsActive;
	}

	bool GetActive () const
	{
		return m_Active;
	}

};

#endif