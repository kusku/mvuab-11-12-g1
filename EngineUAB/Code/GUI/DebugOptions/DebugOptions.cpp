#include "DebugOptions.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "RenderManager.h"
#include "Fonts\FontManager.h"
#include "Math\Vector2.h"
#include "Scripting\ScriptManager.h"
#include "ActionToInput.h"
#include "Utils/BaseUtils.h"
#include "Base.h"
#include "Core.h"
#include <sstream>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDebugOptions::CDebugOptions()
	: m_FileName("")
	, m_uNowPage(0)
	, m_bBoolValue(false)
	, m_iIntValue(0)
	, m_fFloatValue(0.f)
	, m_uNowLine(0)
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

	m_uNowPage = 0;
	m_uNowLine = 0;

	m_bBoolValue = false;
	m_iIntValue = 0;
	m_fFloatValue = 0.f;
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
				if( l_TypeOfPage != NONE_PAGE )
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
							l_Element.Show_Script = l_Node(i)(j).GetPszProperty("show_script", "");
							l_Element.Action_Script = l_Node(i)(j).GetPszProperty("action_script", "");
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
	else return NONE_PAGE;
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
		uint16 dx = 30;
		uint16 dy = 30;
		CColor l_ColorToRender = color;

		RenderQuad(RM, Vect2i(dx, dy), Vect2i(l_ScreenSize.x - 70, l_ScreenSize.y - 70) );
	
		dx += 10;
		dy += 10;

		//Make Header
		std::string l_sNextPage;
		std::string l_sPrevPage;
		std::string l_DoAction;
		CORE->GetActionToInput()->GetActionInfo("NextPage", l_sNextPage);
		CORE->GetActionToInput()->GetActionInfo("PrevPage", l_sPrevPage);
		CORE->GetActionToInput()->GetActionInfo("DoAction", l_DoAction);

		std::string l_sArrows;
		if( m_uNowPage == 0 && m_PagesVector.size() > 0 )
		{
			l_sArrows = ">";
		}
		else if( m_uNowPage > 0 && m_uNowPage < m_PagesVector.size() -1 )
		{
			l_sArrows = "<  >";
		}
		else if( m_uNowPage == m_PagesVector.size() - 1)
		{
			l_sArrows = "<";
		}

		if( m_PagesVector[m_uNowPage].Type == OPTIONS )
		{
			
			dy += FM.DrawDefaultText(dx, dy, l_ColorToRender, "OPTIONS    | Previous Page: %s  | Next Page: %s  | Change Value: %s  |   %s",
										l_sNextPage.c_str(), l_sPrevPage.c_str(), l_DoAction.c_str(), l_sArrows.c_str() );
				
		}
		else if( m_PagesVector[m_uNowPage].Type = STADISTICS )
		{
			dy += FM.DrawDefaultText(dx, dy, l_ColorToRender, "STADISTICS    | Previous Page: %s  | Next Page: %s  | Change Value: %s  |   %s",
										l_sNextPage.c_str(), l_sPrevPage.c_str(), l_DoAction.c_str(), l_sArrows.c_str() );
		}

		dy += FM.DrawDefaultText(dx, dy, l_ColorToRender, "__________________________________________________________________________________________\n\n");


		//Print the info of the page
		for(uint16 i=0; i<m_PagesVector[m_uNowPage].ElementsVector.size(); ++i)
		{
			if( m_PagesVector[m_uNowPage].ElementsVector[i].Type != NONE_TYPE )
			{
				//Assign color to the line
				if( m_uNowLine == i )
				{
					l_ColorToRender = colYELLOW;
				}
				else
				{
					l_ColorToRender = color;
				}

				//Run the script
				CORE->GetScriptManager()->RunCode( m_PagesVector[m_uNowPage].ElementsVector[i].Show_Script );

				//Get the value from script
				std::stringstream out;
				std::string l_Value;

				if( m_PagesVector[m_uNowPage].ElementsVector[i].Type == BOOL_TYPE )
				{
					l_Value = m_bBoolValue ? "true" : "false";
				}
				else if( m_PagesVector[m_uNowPage].ElementsVector[i].Type == INT_TYPE )
				{
					out << m_iIntValue;
					l_Value = out.str();
				}
				else if( m_PagesVector[m_uNowPage].ElementsVector[i].Type == FLOAT_TYPE )
				{
					out << m_fFloatValue;
					l_Value = out.str();
				}
				
				//Render Text
				FM.DrawDefaultText(dx, dy, l_ColorToRender, m_PagesVector[m_uNowPage].ElementsVector[i].Name.c_str() );
				dy += FM.DrawDefaultText(l_ScreenSize.x - 100, dy, l_ColorToRender, l_Value.c_str() );
			}
		}
	}
	else
	{
		//Renderiza la información para abrir las opciones
		CColor backgroundColor = CColor(0.35f, 0.35f, 0.35f);
		backgroundColor.SetAlpha(0.8f);

		CColor edgesColor = colBLACK;
		edgesColor.SetAlpha(0.7f);

		Vect2i l_SizeRectangle;
		Vect2i l_Screen = RM.GetScreenSize();

		std::string l_sInfo, l_Action;
		CORE->GetActionToInput()->GetActionInfo("DebugOptions", l_Action);
		baseUtils::FormatSrting (l_sInfo, "Press %s to view the Stadistics", l_Action.c_str() );
		l_SizeRectangle.x = FM.SizeX(l_sInfo.c_str());
		l_SizeRectangle.y = FM.SizeY(l_sInfo.c_str());

		uint32 dx = l_Screen.x - l_SizeRectangle.x - 10;
		RM.DrawRectangle2D(Vect2i(dx, l_Screen.y-15), l_SizeRectangle.x, l_SizeRectangle.y, backgroundColor, 2, 2, edgesColor);

		FM.DrawDefaultText(dx, l_Screen.y-15, colWHITE, "Press %s to view the Stadistics", l_Action.c_str());
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

void CDebugOptions::MoveToNextPage()
{
	if( m_uNowPage < (m_PagesVector.size() - 1) )
	{
		++m_uNowPage;
		m_uNowLine = 0;
	}
}

void CDebugOptions::MoveToPrevPage()
{
	if( m_uNowPage > 0)
	{
		--m_uNowPage;
		m_uNowLine = 0;
	}
}

void CDebugOptions::MoveToPrevLine()
{
	if( m_uNowLine > 0 )
	{
		--m_uNowLine;
	}
}

void CDebugOptions::MoveToNextLine()
{
	if( m_uNowLine <  (m_PagesVector[m_uNowPage].ElementsVector.size() - 1))
	{
		++m_uNowLine;
	}
}

void CDebugOptions::DoAction()
{
	CORE->GetScriptManager()->RunCode( m_PagesVector[m_uNowPage].ElementsVector[m_uNowLine].Action_Script );
}