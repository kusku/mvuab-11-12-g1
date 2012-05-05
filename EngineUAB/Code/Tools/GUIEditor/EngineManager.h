#pragma once

#ifndef _ENGINE_MANAGER_H
#define _ENGINE_MANAGER_H

#include "Base.h"
#include "Engine.h"

class CEngineManager
{
public:
	CEngineManager()
		: m_pEngine(NULL)
	{
	}

	~CEngineManager()
	{
	}

	static CEngineManager*	GetInstance()
	{
		if( m_pEngineManager == NULL )
		{
			m_pEngineManager = new CEngineManager();
		}

		return m_pEngineManager;
	}


	void	CleanUp()
	{
		CHECKED_DELETE(m_pEngine);
	}

	void		SetEngine		( CEngine *engine )		{ m_pEngine = engine; }
	CEngine*	GetEngine		() const				{ return m_pEngine; }

private:
	static CEngineManager *m_pEngineManager;

	CEngine		*m_pEngine;
};

#endif