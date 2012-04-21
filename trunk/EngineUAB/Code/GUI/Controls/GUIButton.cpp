#include "GUIButton.h"
#include "Core.h"
#include "Base.h"
#include "Textures\Texture.h"
#include "RenderManager.h"
#include "Logger\Logger.h"
#include "ScriptManager.h"
#include "InputManager.h"


//---Constructor
CGUIButton::CGUIButton (	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
										const Vect2f position_percent, std::string lit, uint32 textHeightOffset,
										uint32 textWidthOffset, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent,position_percent, BUTTON, lit, textHeightOffset, textWidthOffset, isVisible,isActive)
, m_eState(BS_NORMAL)
, m_sLuaCode_OnClicked("")
, m_sLuaCode_OnOver("")
, m_pNormalTexture(NULL)
, m_pOverTexture(NULL)
, m_pClickedTexture(NULL)
, m_pDeactivatedTexture(NULL)
, m_NormalColor(colBLUE)
, m_OverColor(colGREEN)
, m_ClickedColor(colRED)
, m_DeactivatedColor(colYELLOW)
{}


//---------------Interfaz de GuiElement----------------------
void CGUIButton::Render	(CRenderManager *renderManager, CFontManager* fm)
{
	if( CGuiElement::m_bIsVisible)
	{
		//Primero renderizamos todos los hijos que pudiera tener el Button:
		CGuiElement::Render(renderManager, fm);

		switch(m_eState)
		{
		case BS_NORMAL:
			if (m_pNormalTexture)
			{
				renderManager->DrawTexturedQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pNormalTexture);
			}
			else
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_NormalColor);
			}
			break;
		case BS_OVER:
			if (m_pOverTexture)
			{
				renderManager->DrawTexturedQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pOverTexture);
			}
			else
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_OverColor);
			}
			break;
		case BS_CLICKED:
			if (m_pClickedTexture)
			{
				renderManager->DrawTexturedQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pClickedTexture);
			}
			else
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_ClickedColor);
			}
			break;
		default:
			LOGGER->AddNewLog(ELL_ERROR,"CGUIButton::Render Tipo de estado desconocido en el boton %s",CGuiElement::m_sName.c_str());
			break;
		}

		if( !CGuiElement::m_bIsActive )
		{
			if (m_pDeactivatedTexture)
			{
				renderManager->DrawTexturedQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pDeactivatedTexture);
			}
			else
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_DeactivatedColor);
			}
		}

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText(renderManager, fm);	

		CGuiElement::RenderGUIEditor(renderManager);
	}//END if( CGuiElement::m_bIsVisible 
}

void CGUIButton::Update(CInputManager* intputManager, float elapsedTime)
{
	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		//Primero actualizamos todos los hijos que pudiera tener el button:
		CGuiElement::Update(intputManager, elapsedTime);

		Vect2i mousePosition;
		intputManager->GetPosition(IDV_MOUSE, mousePosition);
		CGuiElement::CalculatePosMouse(mousePosition);
		if( CGuiElement::IsInside() )
		{
			if(m_eState != BS_CLICKED)
			{
				m_eState = BS_OVER;
			}
			if( CGuiElement::IsOver() )
			{
				OnOverButton();
			}
			
			if (intputManager->IsUpDown(IDV_MOUSE,MOUSE_BUTTON_LEFT) ||
				 (intputManager->IsDown(IDV_MOUSE,MOUSE_BUTTON_LEFT) && m_eState == BS_CLICKED ) )
			{
				m_eState = BS_CLICKED;
			}

			if (intputManager->IsDownUp(IDV_MOUSE,MOUSE_BUTTON_LEFT) && m_eState == BS_CLICKED )
			{
				m_eState = BS_OVER;
				//Si hemos pasado de apretar el botón a no apretarlo entonces lanzar evento
				OnClickedButton();
			}
		}
		else
		{
			m_eState = BS_NORMAL;
		}
	}
}

//---------------Interfaz de Button---------------------------
void CGUIButton::SetTextures (CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
{
	m_pNormalTexture			= normal;
	m_pOverTexture				= over;
	m_pClickedTexture			= clicked;
	m_pDeactivatedTexture	= deactivated;
}

void CGUIButton::SetColors (const CColor& normal, const CColor& over, const CColor& clicked, const CColor& deactivated, float alpha)
{
	m_NormalColor				= normal;
	m_OverColor					= over;
	m_ClickedColor			= clicked;
	m_DeactivatedColor	= deactivated;

	m_NormalColor.SetAlpha(alpha);
	m_OverColor.SetAlpha(alpha);
	m_ClickedColor.SetAlpha(alpha);
	m_DeactivatedColor.SetAlpha(alpha);
}

void CGUIButton::SetOnClickedAction( std::string & inAction )
{
	m_sLuaCode_OnClicked = inAction;
}

void CGUIButton::SetOnOverAction( std::string & inAction )
{
	m_sLuaCode_OnOver = inAction;
}

void CGUIButton::OnClickedButton( void )
{
	if( CGuiElement::m_pParent != NULL )
		m_pParent->OnClickedChild(CGuiElement::m_sName);

	if (m_sLuaCode_OnClicked.compare(""))
	{
		//Lanzar acción en Lua:
		CScriptManager* scriptManager = CORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnClicked);
		//scriptManager->RunScript(m_sLuaCode_OnClicked);
	}
}

void CGUIButton::OnOverButton( void )
{
	if (m_sLuaCode_OnOver.compare(""))
	{
		//Lanzar acción en Lua:
		CScriptManager* scriptManager = CORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnOver);
		//scriptManager->RunScript(m_sLuaCode_OnOver);
	}
}


