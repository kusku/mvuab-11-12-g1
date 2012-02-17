#pragma once

#ifndef _MODIFIER_MANAGER_H
#define _MODIFIER_MANAGER_H

class CFontManager;
class CRenderManager;

#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\Active.h"
#include "Math\Color.h"
#include "Math\Vector2.h"
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

	void		Render		( CRenderManager &RM, CFontManager &FM, CColor Color = colWHITE );

	void		MoveToNextModifier		();
	void		MoveToPreviousModifier	();	

	void		AddValueToModifier		();
	void		SubsValueToModifier		();

	void		GoToModifier			();
	void		GoToRoot				();

	void		SetVisible				( bool visible );
	bool		GetVisible				() const			{ return GetActive(); }

private:
	std::string				m_FileName;
	uint16					m_NowIndexInVector;
	bool					m_IsChild;

	Vect2i					m_SizeRectangle;

	bool		LoadFile			();
	void		RenderQuad			( CRenderManager &RM, const uint16 dx, const uint16 dy );
	void		AnalizeSizeInfo		();
};

#endif

