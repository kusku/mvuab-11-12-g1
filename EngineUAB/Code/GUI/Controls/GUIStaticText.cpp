#include "GUIStaticText.h"
#include "RenderManager.h"
#include "Logger/Logger.h"
#include "Core.h"
#include "Base.h"

//---Constructor
CGUIStaticText::CGUIStaticText( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
													const Vect2f position_percent, std::string lit, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, STATIC_TEXT, lit, 0, 0, isVisible,isActive)
{}


//---------------Interfaz de GuiElement----------------------
void	CGUIStaticText::Render (CRenderManager *renderManager, CFontManager* fm)
{	
	if( CGuiElement::m_bIsVisible)
	{
		//Primero renderizamos todos los hijos que pudiera tener el Static Text:
		CGuiElement::Render(renderManager, fm);

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText(renderManager, fm);	

		CGuiElement::RenderGUIEditor(renderManager);
	}
}

void CGUIStaticText::Update(CInputManager* intputManager, float elapsedTime)
{
	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		//Primero actualizamos todos los hijos que pudiera tener el checkButton:
		CGuiElement::Update(intputManager, elapsedTime);
	}
}

void CGUIStaticText::SetLiteral( const std::string& lit)
{
	CGuiElement::m_sLiteral = lit;
}

