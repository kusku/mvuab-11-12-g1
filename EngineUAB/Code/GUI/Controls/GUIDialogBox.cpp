#include "GUIDialogBox.h"
#include "InputManager.h"
#include "Textures/Texture.h"
#include "RenderManager.h"
#include "Core.h"
#include "Base.h"

//---Constructor
CGUIDialogBox::CGUIDialogBox(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
											 const Vect2f position_percent,  float buttonWidthPercent, float buttonHeightPercent,
											 std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, DIALOG_BOX, lit, textHeightOffset, textWidthOffset, isVisible, isActive)

, m_bDialogClicked(false)
, m_ButtonClose(windowsHeight, windowsWidth, buttonHeightPercent, buttonWidthPercent,  
								position_percent + Vect2f(witdh_percent-buttonWidthPercent,0.f), "", 0, 0, isVisible, isActive )
, m_ButtonMove(windowsHeight, windowsWidth, buttonHeightPercent, witdh_percent-buttonWidthPercent,  position_percent, "", 0, 0, isVisible, isActive )
, m_pBackGroundTexture(NULL)
, m_BackGroundColor(colWHITE)
, m_bStart_to_Move(false)
, m_PreviousPosMouse(v2iZERO)
, m_fButtonHeight( buttonHeightPercent )
, m_fButtonWidth( buttonWidthPercent )
{
	m_ButtonClose.SetParent(this);
	m_ButtonClose.SetName("buttonClose");
}

//---------------Interfaz de GuiElement----------------------
void CGUIDialogBox::Render	(CRenderManager *renderManager, CFontManager* fm)
{
	if (CGuiElement::m_bIsVisible)
	{
		if (m_pBackGroundTexture)
		{
			renderManager->DrawTexturedQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pBackGroundTexture);
		}
		else
		{
			renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_BackGroundColor);
		}

		m_ButtonMove.Render(renderManager, fm);
		m_ButtonClose.Render(renderManager, fm);
	}
	
	//Finalmente renderizamos el texto:
	CGuiElement::RenderText(renderManager, fm);	

	CGuiElement::RenderGUIEditor(renderManager);
}

void CGUIDialogBox::Update (CInputManager* intputManager, float elapsedTime)
{
	if (CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive)
	{
		Vect2i posMouse;
		intputManager->GetPosition(IDV_MOUSE, posMouse);
		if( m_bDialogClicked )
		{
			if (!intputManager->IsDown(IDV_MOUSE,MOUSE_BUTTON_LEFT))
			{
				m_bDialogClicked = false;
				m_bStart_to_Move = false;
			}
			else
			{
				if( !m_bStart_to_Move )
				{
					m_bStart_to_Move = true;
					m_PreviousPosMouse = posMouse;
				}
				else
				{
					Vect2i incPos = posMouse - m_PreviousPosMouse;
					m_PreviousPosMouse = posMouse;
					m_ButtonMove.SetPosition( m_ButtonMove.GetPosition() + incPos );
					m_ButtonClose.SetPosition( m_ButtonClose.GetPosition() + incPos );
					SetPosition( GetPosition() + incPos);
				}
			}
		}

		CGuiElement::CalculatePosMouse(posMouse);
		if (CGuiElement::IsInside())
		{
			if (intputManager->IsDown(IDV_KEYBOARD,0))
			{
				CGuiElement::m_bFocus = true;
			}
		}

		m_ButtonClose.Update( intputManager, elapsedTime );
		m_ButtonMove.Update( intputManager, elapsedTime );

		CGuiElement::CalculatePosMouse(posMouse);
		if( m_ButtonMove.IsClicking() )
			m_bDialogClicked = true;

	}// End if( GuiElement::mIsVisible && GuiElement::mIsActive )
}

void CGUIDialogBox::OnClickedChild( const std::string& name )
{
	if( name.compare("buttonClose") == 0 )
	{
		CGuiElement::m_bIsActive = false;
		CGuiElement::m_bIsVisible = false;
	}
}

void CGUIDialogBox::SetPosition(const Vect2i& pos)
{
	m_ButtonMove.SetPosition( pos );
	m_ButtonClose.SetPosition( pos + Vect2i((uint32)(CGuiElement::m_uWidth - m_ButtonClose.GetWidth()),0) );
	CGuiElement::SetPosition( pos );
}

void CGUIDialogBox::SetPositionPercent(const Vect2f& pos)
{
	m_ButtonMove.SetPositionPercent(pos);
	m_ButtonClose.SetPositionPercent( pos + Vect2f(CGuiElement::m_fWidthPercent - m_ButtonClose.GetWidthPercent(), 0.f) );
	CGuiElement::SetPositionPercent(pos);
}



//---------------Interfaz de Button---------------------------
void CGUIDialogBox::SetCloseButtonTextures	(CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
{
	m_ButtonClose.SetTextures(normal, over, clicked, deactivated);
}

void CGUIDialogBox::SetMoveButtonTextures (CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
{
	m_ButtonMove.SetTextures(normal, over, clicked, deactivated);
}

void CGUIDialogBox::SetDialogTexture (CTexture* background)
{
	assert(background);
	m_pBackGroundTexture = background;
}

void CGUIDialogBox::SetCloseButtonColors (	const CColor& normal, const CColor& over, const CColor& clicked,
																				const CColor& deactivated)
{
	m_ButtonClose.SetColors(normal, over, clicked, deactivated);
}
void CGUIDialogBox::SetMoveButtonColors (const CColor& normal, const CColor& over, const CColor& clicked, 
																			const CColor& deactivated)
{
	m_ButtonMove.SetColors(normal, over, clicked, deactivated);
}

void CGUIDialogBox::SetDialogColors (const CColor& background)
{
	m_BackGroundColor = background;
}

