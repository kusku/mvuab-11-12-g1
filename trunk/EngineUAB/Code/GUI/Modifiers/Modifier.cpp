#include "Modifier.h"
#include "Utils\Types.h"
#include "XML\XMLTreeNode.h"
#include "Fonts\FontManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Base.h"
#include "Core.h"
#include <sstream>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CModifier::CModifier(CXMLTreeNode &Node)
{
	//General Params
	std::string l_Name = Node.GetPszProperty("name", "");
	std::string l_Technique = Node.GetPszProperty("technique", "");

	m_pEffectTechnique = CORE->GetEffectManager()->GetEffectTechnique(l_Technique);
	SetName(l_Name);

	//Specific Params
	uint16 l_Count = Node.GetNumChildren();
	for(uint16 i=0; i<l_Count; ++i)
	{
		std::string l_Type = Node(i).GetName();
		if( l_Type == "param" )
		{
			uint16 l_Id = Node(i).GetIntProperty("id", 0);
			std::string l_Name = Node(i).GetPszProperty("name", "");
			float l_StartRange = Node(i).GetFloatProperty("start_range", 0.0f);
			float l_EndRange = Node(i).GetFloatProperty("end_range", 0.0f);
			float l_Precision = Node(i).GetFloatProperty("precision", 0.1f);

			CParam *l_Param = new CParam(l_Id, l_StartRange, l_EndRange, l_Precision, l_Name);
			m_ParamsMap[l_Id] = l_Param;
		}
	}
}


CModifier::~CModifier()
{
	std::map<uint16, CParam*>::iterator l_It = m_ParamsMap.begin();
	std::map<uint16, CParam*>::iterator l_End = m_ParamsMap.end();
	for(; l_It != l_End; ++l_It)
	{
		CHECKED_DELETE( l_It->second );
	}

	m_ParamsMap.clear();
	m_pEffectTechnique = NULL;
}

void CModifier::Render(CFontManager &FM, const std::string &Name, uint32 dx, uint32 Initial_dy, CColor Color)
{
	uint32 dy = Initial_dy;
	uint16 l_Count = m_ParamsMap.size();
	dy += FM.DrawDefaultText( dx, dy, Color, Name.c_str() );
	dy += FM.DrawDefaultText( dx, dy, Color, "-------------------------------"  );

	for(uint16 i=0; i<l_Count; ++i)
	{
		std::stringstream out;
		out << m_pEffectTechnique->GetValueFromParam(i);
		std::string l_Text = m_ParamsMap[i]->GetName() + " : " + out.str();

		if( m_NowIndexInMap != i )
		{
			dy += FM.DrawDefaultText(dx, dy, Color, l_Text.c_str() );
		}
		else
		{
			l_Text = "> " + l_Text;
			dy += FM.DrawDefaultText(dx, dy, Color, l_Text.c_str() );
		}
	}
}

void CModifier::MoveToNextParam()
{
	uint16 l_Position = m_NowIndexInMap + 1;
	if( l_Position < m_ParamsMap.size() )
	{
		m_NowIndexInMap = l_Position;
	}
}

void CModifier::MoveToPreviousParam()
{
	if( m_NowIndexInMap > 0 )
	{
		--m_NowIndexInMap;
	}
}

void CModifier::AddValue()
{
	float l_Value = m_pEffectTechnique->GetValueFromParam(m_NowIndexInMap);
	l_Value +=  m_ParamsMap[m_NowIndexInMap]->m_Precision;
	if( l_Value > m_ParamsMap[m_NowIndexInMap]->m_EndRange )
	{
		l_Value = m_ParamsMap[m_NowIndexInMap]->m_EndRange;
	}
	
	m_pEffectTechnique->SetValueFromParam(m_NowIndexInMap, l_Value);
}

void CModifier::SubsValue()
{
	float l_Value = m_pEffectTechnique->GetValueFromParam(m_NowIndexInMap);
	l_Value -=  m_ParamsMap[m_NowIndexInMap]->m_Precision;
	if( l_Value < m_ParamsMap[m_NowIndexInMap]->m_StartRange )
	{
		l_Value = m_ParamsMap[m_NowIndexInMap]->m_StartRange;
	}
	m_pEffectTechnique->SetValueFromParam(m_NowIndexInMap, l_Value);
}