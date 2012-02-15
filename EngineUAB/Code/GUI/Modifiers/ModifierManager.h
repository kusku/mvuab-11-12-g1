#pragma once

#ifndef _MODIFIER_MANAGER_H
#define _MODIFIER_MANAGER_H

class CFontManager;

#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\Active.h"
#include "Math\Color.h"
#include "Modifier.h"
#include <string>

class CModifierManager : public CTemplatedVectorMapManager<CModifier>, public CActive
{
public:
	CModifierManager();
	~CModifierManager();

	bool		Load		( const std::string &FileName );
	bool		Reload		();
	void		CleanUp		();

	void		Render		( CFontManager &FM, CColor Color = colWHITE );

	void		MoveToNextModifier		();
	void		MoveToPreviousModifier	();	

	void		AddValueToModifier		();
	void		SubsValueToModifier		();

	void		GoToModifier			();
	void		GoToRoot				();

private:
	std::string				m_FileName;
	uint16					m_NowIndexInVector;
	bool					m_IsChild;

	bool		LoadFile	();
};

#endif

