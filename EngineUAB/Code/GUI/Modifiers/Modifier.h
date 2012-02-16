#pragma once

#ifndef _MODIFIER_H
#define _MODIFIER_H

class CXMLTreeNode;
class CFontManager;
class CEffectTechnique;

#include "Utils\Types.h"
#include "Utils\Named.h"
#include "Math\Color.h"
#include <map>
#include <string>

class CModifier : public CNamed
{
protected:
	class CParam : public CNamed
	{
	public:
		uint16				m_ParamId;
		float				m_StartRange;
		float				m_EndRange;
		float				m_Precision;

		CParam( uint16 Id, float StartRange, float EndRange, float Precision, const std::string &Name )
			: m_ParamId(Id)
			, m_StartRange(StartRange)
			, m_EndRange(EndRange)
			, m_Precision(Precision)
		{
			SetName(Name);
		}

		~CParam()
		{
		}
	};

	std::map<uint16, CParam*>			m_ParamsMap;
	uint16								m_NowIndexInMap;
	CEffectTechnique					*m_pEffectTechnique;	

public:
	CModifier( CXMLTreeNode &Node );
	~CModifier();

	void		Render					( CFontManager &FM, const std::string &Name, uint32 dx, uint32 Initial_dy, CColor Color = colWHITE );

	void		ResetToMove				()		{ m_NowIndexInMap = 0; }
	void		MoveToNextParam			();
	void		MoveToPreviousParam		();
	void		AddValue				();
	void		SubsValue				();
};

#endif
