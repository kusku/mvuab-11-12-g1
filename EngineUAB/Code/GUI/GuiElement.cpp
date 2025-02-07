#include "GuiElement.h"
#include "Core.h"
#include "Base.h"
#include "Scripting\ScriptManager.h"
#include "Location\LanguageManager.h"
#include "RenderManager.h"
#include "Fonts\FontManager.h"

CGuiElement::CGuiElement( uint32 windowsHeight, uint32 windowsWidth, float height_percent, float width_percent, 
							Vect2f position_percent, TypeGuiElement type, std::string lit, uint32 textHeightOffset,
							uint32 textWidthOffset, bool isVisible, bool isActive)
	: m_uWindowsHeight( windowsHeight )
	, m_uWindowsWidth(windowsWidth )
	, m_fWidthPercent( width_percent )
	, m_fHeightPercent( height_percent )
	, m_PositionPercent( position_percent )
	, m_bIsVisible( isVisible )
	, m_bIsActive( isActive )
	, m_bIsOver( false )
	, m_bIsInside( false )
	, m_bFocus( false )
	, m_bRenderSelected(false)
	, m_bRenderForGUIEditor(false)
	, m_sLuaCode_OnLoadValue("")
	, m_sLuaCode_OnSaveValue("")
	, m_sName("DefaultGuiElement")
	, m_sId("")
	, m_pParent(NULL)
	, m_eEditMode(DISABLE)
	, m_eType(type)
	, m_z(0)
	, m_sLiteral(lit)
	, m_uTextHeightOffset(textHeightOffset)
	, m_uTextWidthOffset(textWidthOffset)
{
	m_uWidth	= (uint32)(m_fWidthPercent * 0.01f * m_uWindowsWidth);
	m_uHeight	= (uint32)(m_fHeightPercent * 0.01f * m_uWindowsHeight);
	m_Position.x	= (uint32) (m_uWindowsWidth	* 0.01f * m_PositionPercent.x );
	m_Position.y	= (uint32) (m_uWindowsHeight * 0.01f * m_PositionPercent.y);
}


void CGuiElement::SetWindowsWidth (uint32	w)
{
	m_uWindowsWidth = w;
	m_uWidth	= (uint32)(m_fWidthPercent * 0.01f * m_uWindowsWidth);
	m_Position.x	= (uint32) (m_uWindowsWidth	* 0.01f * m_PositionPercent.x );
}

void CGuiElement::SetWindowsHeight (uint32	h)
{
	m_uWindowsHeight = h;
	m_uHeight	= (uint32)(m_fHeightPercent * 0.01f * m_uWindowsHeight);
	m_Position.y	= (uint32) (m_uWindowsHeight * 0.01f * m_PositionPercent.y);
}

void CGuiElement::SetHeightPercent (float height_percent)
{
	m_fHeightPercent = height_percent;
	m_uHeight	= (uint32)(m_fHeightPercent * 0.01f * m_uWindowsHeight);
}

void CGuiElement::SetWidthPercent (float width_percent)
{
	m_fWidthPercent = width_percent;
	m_uWidth	= (uint32)(m_fWidthPercent * 0.01f * m_uWindowsWidth);
}

void CGuiElement::SetPositionPercent (const Vect2f& pos)
{
	m_PositionPercent = pos;
	m_Position.x	= (uint32) (m_uWindowsWidth	* 0.01f * m_PositionPercent.x );
	m_Position.y	= (uint32) (m_uWindowsHeight * 0.01f * m_PositionPercent.y);
}
void	CGuiElement::SetPosition (const Vect2i& pos)
{
	m_Position = pos;
	m_PositionPercent.x = (float) (pos.x/m_uWindowsWidth)*100;
	m_PositionPercent.y = (float) (pos.y/m_uWindowsHeight)*100;
}

void CGuiElement::CalculatePosMouse( const Vect2i& mousePosition )
{
	if( m_bIsActive && m_bIsVisible )
	{
		//  (0,0) ________    
		//		 |					|   
		//		 |GuiElement|   
		//		 |					|
		//		  ----------(m_fWidth, m_fHeight)  

		if(	(mousePosition.x > m_Position.x) && (mousePosition.x < m_Position.x+(int32)m_uWidth) &&
				(mousePosition.y > m_Position.y) && (mousePosition.y < m_Position.y+(int32)m_uHeight) )
		{
			//Esta dentro de la zona que representa el GuiElement

			//Antes de actulizar mIsInside a true, observamos si anteriormente el puntero del 
			//mouse estaba fuera de la zona (pas� de fuera a dentro-> mIsOver=true).
			if( m_bIsInside == true )
			{
				//Si el puntero de mouse estaba ya dentro ==> no ha pasado de fuera a dentro
				m_bIsOver = false;
			}
			else 
			{	//Si el puntero de mouse estaba fuera ==> ha pasado de fuera a dentro
				m_bIsOver = true;
			}

			//Finalmente actualizamos el valor de mIsInside a true
			m_bIsInside = true;
		}
		else
		{
			m_bIsInside = false;
			m_bIsOver = false;
		}
	}
}

bool CGuiElement::AddChild (CGuiElement* child)
{
	assert(child);
	//Primero comprobamos que no lo hayan registrado ya como hijo
	std::vector<CGuiElement*>::iterator it = m_Children.begin();	
	std::vector<CGuiElement*>::iterator itEnd = m_Children.end();	
	while(it!=itEnd)
	{
		CGuiElement* guiElement = *it;
		if (guiElement == child )
		{
			return false;
		}
		it++;
	}

	m_Children.push_back(child);
	child->SetParent(this);
	return true;
}

bool CGuiElement::RemoveChild (CGuiElement* child)
{
	assert(child);
	bool ok = false;
	std::vector<CGuiElement*>::iterator it = m_Children.begin();	
	std::vector<CGuiElement*>::iterator itEnd = m_Children.end();	
	while(it!=itEnd)
	{
		CGuiElement* guiElement = *it;
		if (guiElement == child )
		{
			m_Children.erase(it);
			ok = true;
			break;
		}
		it++;
	}

	return ok;
}

void CGuiElement::OnLoadValue( void )
{
	if( m_sLuaCode_OnLoadValue.compare("") != 0)
	{
		//Ejecutar el c�digo Lua:
		CScriptManager * scriptManager = CORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnLoadValue);
		//scriptManager->RunScript(m_sLuaCode_OnLoadValue);
	}
}

void CGuiElement::OnSaveValue( void )
{
	if( m_sLuaCode_OnSaveValue.compare("") != 0)
	{
		//Ejecutar el c�digo Lua:
		CScriptManager * scriptManager = CORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnSaveValue);
		//scriptManager->RunScript(m_sLuaCode_OnSaveValue);
	}
}


void CGuiElement::Render (CRenderManager* render, CFontManager* fm)
{
	if (m_bIsVisible)
	{
		//Renderizamos todos los hijos que cuelgan de este CGuiElement:
		std::vector<CGuiElement*>::iterator it = m_Children.begin();	
		std::vector<CGuiElement*>::iterator itEnd = m_Children.end();	
		while(it!=itEnd)
		{
			(*it)->Render(render, fm);
			it++;
		}
	}//if (m_sLiteral != "")
}

void CGuiElement::RenderText (CRenderManager* render, CFontManager* fm)
{
	if (m_bIsVisible)
	{
		//Render text:
		if (m_sLiteral != "")
		{
			uint32 posX = CGuiElement::m_Position.x + m_uTextWidthOffset;
			uint32 posY = CGuiElement::m_Position.y + m_uTextHeightOffset;

			fm->DrawLiteral(posX, posY, m_sLiteral);
		}//if (m_sLiteral != "")

	}//if (m_bIsVisible)
}

void CGuiElement::Update (CInputManager* input, float elapsedTime)
{
	if (m_bIsVisible && m_bIsActive)
	{
		//Renderizamos todos los hijos que cuelgan de este CGuiElement:
		std::vector<CGuiElement*>::iterator it = m_Children.begin();	
		std::vector<CGuiElement*>::iterator itEnd = m_Children.end();	
		while(it!=itEnd)
		{
			CGuiElement* guiElement = *it;
			guiElement->Update(input, elapsedTime);
			it++;
		}
	}
}

void CGuiElement::RenderGUIEditor(CRenderManager *render)
{
	if( m_bRenderForGUIEditor )
	{
		if( m_bRenderSelected )
		{
			render->DrawQuad2D( m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, UPPER_LEFT, CColor(0.2f, 0.2f, 0.2f, 0.8f) );
		}
		else if( m_bIsInside )
		{
			render->DrawQuad2D( m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, UPPER_LEFT, CColor(1.0f, 1.0f, 0.0f, 0.6f) );
		}
	}
}