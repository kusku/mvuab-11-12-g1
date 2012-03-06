#include "DebugOptions.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "RenderManager.h"
#include "Fonts\FontManager.h"
#include "Math\Vector2.h"
#include "ScriptManager.h"
#include "Base.h"
#include "Core.h"
#include <sstream>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDebugOptions::CDebugOptions()
	: m_FileName("")
	, m_uNowPage(1)
	, m_bBoolValue(false)
	, m_iIntValue(0)
	, m_fFloatValue(0.f)
{
	m_Active = false;
}


CDebugOptions::~CDebugOptions()
{
	CleanUp();
}

void CDebugOptions::CleanUp()
{
	for(uint16 i=0; i<m_PagesVector.size(); ++i)
	{
		m_PagesVector[i].ElementsVector.clear();
	}

	m_PagesVector.clear();
}

bool CDebugOptions::Load(const std::string &_FileName)
{
	m_FileName = _FileName;
	return LoadFile();
}

bool CDebugOptions::Reload()
{
	CleanUp();
	return LoadFile();
}

bool CDebugOptions::LoadFile()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CDebugOptions::LoadFile->Error al intentar leer el archivo de opciones: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_Node = newFile["debug_options"];
	if( l_Node.Exists() )
	{
		//Reading the pages
		uint16 l_Count = l_Node.GetNumChildren();
		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_Node(i).GetName();
			if( l_Type == "page" )
			{
				TETypePage l_TypeOfPage = String2TypePage( l_Node(i).GetPszProperty("type", "") );
				if( l_TypeOfPage != NONE )
				{
					//Reading the options of a page
					SPage l_Page;
					l_Page.Type = l_TypeOfPage;

					uint16 l_PageCount = l_Node(i).GetNumChildren();
					for(uint16 j=0; j<l_PageCount; ++j)
					{
						std::string l_PageType = l_Node(i)(j).GetName();
						if( l_PageType == "option" )
						{
							SElementOnPage l_Element;
							l_Element.Name = l_Node(i)(j).GetPszProperty("name", "");
							l_Element.Init_Script = l_Node(i)(j).GetPszProperty("init_script", "");
							l_Element.Script = l_Node(i)(j).GetPszProperty("script", "");
							l_Element.Type = String2TypeValue( l_Node(i)(j).GetPszProperty("type_value", "") );
							
							l_Page.ElementsVector.push_back(l_Element);
						}
					}

					m_PagesVector.push_back(l_Page);
				}
				else
				{
					LOGGER->AddNewLog(ELL_WARNING, "CDebugOptions::LoadFile->No se reconoce el tipo de página." );
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

TETypePage CDebugOptions::String2TypePage(const std::string &Type)
{
	if( Type == "Options" ) return OPTIONS;
	else if( Type == "Stadistics" ) return STADISTICS;
	else return NONE;
}

TETypeOfValues CDebugOptions::String2TypeValue(const std::string &Type)
{
	if( Type == "bool" ) return BOOL_TYPE;
	else if( Type == "int" ) return INT_TYPE;
	else if( Type == "float" ) return FLOAT_TYPE;
	else return NONE_TYPE;
}

void CDebugOptions::Render(CRenderManager &RM, CFontManager &FM, CColor color)
{
	if( GetActive() )
	{
		Vect2i l_ScreenSize = RM.GetScreenSize();
		uint16 dx = 50;
		uint16 dy = 50;

		RenderQuad(RM, Vect2i(dx, dy), Vect2i(l_ScreenSize.x - 100, l_ScreenSize.y - 100) );
	
		dx += 10;
		dy += 10;

		for(uint16 i=0; i<m_PagesVector[m_uNowPage].ElementsVector.size(); ++i)
		{
			if( m_PagesVector[m_uNowPage].ElementsVector[i].Type != NONE_TYPE )
			{
				FM.DrawDefaultText(dx, dy, color, m_PagesVector[m_uNowPage].ElementsVector[i].Name.c_str() );

				CORE->GetScriptManager()->RunCode( m_PagesVector[m_uNowPage].ElementsVector[i].Script );
				std::stringstream out;

				if( m_PagesVector[m_uNowPage].ElementsVector[i].Type == BOOL_TYPE )
					out << m_bBoolValue;
				else if( m_PagesVector[m_uNowPage].ElementsVector[i].Type == INT_TYPE )
					out << m_iIntValue;
				else if( m_PagesVector[m_uNowPage].ElementsVector[i].Type == FLOAT_TYPE )
					out << m_fFloatValue;
				
				std::string l_Value = out.str();
				dy += FM.DrawDefaultText(l_ScreenSize.x - 200, dy, color, l_Value.c_str() );
			}
		}
	}
}

void CDebugOptions::RenderQuad(CRenderManager &RM, const Vect2i &Init, const Vect2i &End)
{
	CColor backgroundColor = CColor(0.35f, 0.35f, 0.35f);
	backgroundColor.SetAlpha(0.8f);

	CColor edgesColor = colBLACK;
	edgesColor.SetAlpha(0.7f);
		
	Vect2i screen = RM.GetScreenSize();
	RM.DrawRectangle2D(Init, End.x, End.y, backgroundColor, 2, 2, edgesColor);
}