#include "GUIRadioBox.h"
#include "InputManager.h"
#include "Textures/Texture.h"
#include "RenderManager.h"
#include "Logger/Logger.h"
#include "Scripting\ScriptManager.h"
#include "GraphicsDefs.h"
#include "Core.h"
#include "Base.h"

//---Constructor
CGUIRadioBox::CGUIRadioBox(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
										 const Vect2f position_percent,  uint32 columns, uint32 rows, const std::string& defaultButtonCheck,
										  std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, bool isVisible, bool isActive)

: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, RADIO_BOX, lit, textHeightOffset, textWidthOffset, isVisible, isActive)
, m_pBackGroundTexture(NULL)
, m_sDefaultButtonCheck(defaultButtonCheck)
, m_uColumns(columns)
, m_uRows(rows)
, m_uLastButtonColumn(0)
, m_uLastButtonRow(0)
, m_uBlockCheckButton(0)
{
	m_CheckButtons.reserve(columns*rows);
}


//---------------CGuiElement Interface----------------------
void CGUIRadioBox::Render (CRenderManager *renderManager, CFontManager* fm)
{
	if( CGuiElement::m_bIsVisible)
	{
		if (m_pBackGroundTexture)
		{
			renderManager->DrawTexturedQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pBackGroundTexture);
		}

		uint32 cont_init = m_uBlockCheckButton * (m_uColumns*m_uRows);
		uint32 cont_end = m_uColumns*m_uRows;
		for( uint32 cont = cont_init; cont < cont_init + cont_end; cont++ )
		{
			m_CheckButtons[cont].Render(renderManager, fm);
		}

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText(renderManager, fm);	
	}
}
void CGUIRadioBox::Update (CInputManager* intputManager, float elapsedTime)
{
	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		uint32 cont_init = m_uBlockCheckButton * (m_uColumns*m_uRows);
		uint32 cont_end = m_uColumns * m_uRows;

		for( uint32 cont = cont_init; cont < cont_init + cont_end; cont++ )
		{
			m_CheckButtons[cont].Update(intputManager, elapsedTime);
		}
	}
}

void CGUIRadioBox::OnClickedChild( const std::string& inName )
{
	CGuiElement::m_bFocus = true;

	std::vector<CGUICheckButton>::iterator it(m_CheckButtons.begin());
	std::vector<CGUICheckButton>::iterator itEnd(m_CheckButtons.end());
	//[sp] Tenemos que recorrer todos los CheckButtons para ponerlos a todos en off menos el inName
	while (it!=itEnd)
	{
		if( it->GetName().compare(inName) != 0 )
		{
			it->SetOn( false );
		}
		else
		{
			it->SetOn( true );
		}
		it++;
	}

	m_sDefaultButtonCheck = inName;
}

//---------------CCheckButton Interface----------------------
void CGUIRadioBox::SetCheckButton (const std::string& name, CTexture* on, CTexture* off, CTexture* deactivated )
{
	float heightButton	= m_fHeightPercent/m_uRows;
	float widthButton		= m_fWidthPercent/m_uColumns;

	Vect2f pos;
	pos.y = m_PositionPercent.y + m_uLastButtonRow*heightButton;
	pos.x = m_PositionPercent.x + m_uLastButtonColumn*widthButton;

	CGUICheckButton newCheckButton(CGuiElement::m_uWindowsHeight, CGuiElement::m_uWindowsWidth, heightButton, widthButton, pos, false, "", 0, 0, true, true);
	if( name.compare(m_sDefaultButtonCheck) == 0)
	{
		newCheckButton.SetOn(true);
	}
	newCheckButton.SetTextures(on,off,deactivated);
	newCheckButton.SetParent(this);
	newCheckButton.SetName(name);
	m_CheckButtons.push_back(newCheckButton);

	m_uLastButtonColumn++;
	if( m_uLastButtonColumn == m_uColumns )
	{
		m_uLastButtonColumn = 0;
		m_uLastButtonRow++;
		if( m_uLastButtonRow == m_uRows )
		{
			m_uLastButtonRow = 0;
		}
	}
}

void CGUIRadioBox::NextBlock ()
{
	m_uBlockCheckButton++;
	if( m_uBlockCheckButton >= m_CheckButtons.size()/(m_uColumns*m_uRows) )
	{
		m_uBlockCheckButton--;
	}
}

void CGUIRadioBox::PrevBlock ()
{
	if( m_uBlockCheckButton != 0 )
	{
		m_uBlockCheckButton--;
	}
}

void CGUIRadioBox::SetOnCheckButton (const std::string& buttonCheck)
{
	if( buttonCheck.compare(m_sDefaultButtonCheck) != 0 )
	{
		CGuiElement::m_bFocus = true;

		m_sDefaultButtonCheck = buttonCheck;

		std::vector<CGUICheckButton>::iterator it(m_CheckButtons.begin());
		std::vector<CGUICheckButton>::iterator itEnd(m_CheckButtons.end());
		while (it!=itEnd)
		{
			//[sp] Tenemos que recorrer todos los CheckButtons para ponerlos a todos en off menos el inName
			if( it->GetName().compare(buttonCheck) != 0 )
			{
				it->SetOn( false );
			}
			else
			{
				it->SetOn( true );
			}
			it++;
		}

	}//END if( buttonCheck.compare(m_sDefaultButtonCheck) != 0 )
}

void CGUIRadioBox::SetCheckButtonActions (const std::string&  onCheckOn, const std::string&  onCheckOff, 
																			 const std::string&  onOverButton)
{
	std::vector<CGUICheckButton>::iterator it(m_CheckButtons.begin());
	std::vector<CGUICheckButton>::iterator itEnd(m_CheckButtons.end());
	while (it != itEnd)
	{
		it->SetOnCheckOffAction(onCheckOff);
		it->SetOnCheckOnAction(onCheckOn);
		it->SetOnOverAction(onOverButton);
		it++;
	}
}
