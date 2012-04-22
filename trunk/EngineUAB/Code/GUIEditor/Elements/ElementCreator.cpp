#include "stdafx.h"
#include "ElementCreator.h"
#include "InputManager.h"
#include "Periphericals\Mouse.h"
#include "RenderManager.h"
#include "GUIWindow.h"
#include "Controls\GUIImage.h"
#include "Controls\GUIButton.h"
#include "Controls\GUICheckButton.h"
#include "Textures\TextureManager.h"
#include "Textures\Texture.h"
#include "ElementProperties.h"
#include "Core.h"
#include "Base.h"
#include <sstream>

void CElementCreator::CreateButton(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUIButton *l_pButton = new CGUIButton(screen.y, screen.x, 10.f, 20.f, l_Position, "", 0, 0, true, true);
	std::string l_Name = "button_" + GetSufixNumber(window);
	l_pButton->SetName(l_Name);
	l_pButton->SetID(l_Name);
	l_pButton->SetOnClickedAction(std::string(""));
	l_pButton->SetOnOverAction(std::string(""));

	CTexture *texture = CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/button.jpg");
	l_pButton->SetTextures(texture,texture,texture,texture);
	
	l_pButton->SetRenderForGUIEditor(true);
	l_pButton->SetIsSelected(true);

	window->AddGuiElement( l_pButton );

	CElementProperties::ElementProperties(l_pButton);
}

void CElementCreator::CreateCheckButton(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUICheckButton *l_pCheckButton = new CGUICheckButton(screen.y, screen.x, 10.f, 20.f, l_Position, true, "", 0, 0, true, true);
	std::string l_Name = "check_button_" + GetSufixNumber(window);
	l_pCheckButton->SetName(l_Name);
	l_pCheckButton->SetID(l_Name);
	l_pCheckButton->SetOnCheckOffAction("");
	l_pCheckButton->SetOnCheckOnAction("");
	l_pCheckButton->SetOnLoadValueAction("");
	l_pCheckButton->SetOnOverAction("");
	l_pCheckButton->SetOnSaveValueAction("");

	CTexture *texture = CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/checkbutton.jpg");
	l_pCheckButton->SetTextures(texture, texture, texture);

	l_pCheckButton->SetRenderForGUIEditor(true);
	l_pCheckButton->SetIsSelected(true);

	window->AddGuiElement( l_pCheckButton );

	CElementProperties::ElementProperties(l_pCheckButton);
}

void CElementCreator::CreateImage(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUIImage *l_pImage = new CGUIImage(screen.y, screen.x, 20.f, 20.f, l_Position, "", 0, 0, true, true);
	std::string l_Name = "image_" + GetSufixNumber(window);
	l_pImage->SetName(l_Name);
	l_pImage->SetID(l_Name);
	l_pImage->SetActiveTexture("img1");

	l_pImage->SetOnLoadValueAction("");
	l_pImage->SetOnSaveValueAction("");
	l_pImage->SetBackGround(false);
	l_pImage->SetFlip(NONE_FLIP);

	CTexture* texture_image	= CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/image.jpg");
	l_pImage->SetTexture(texture_image, "default_normal");

	l_pImage->SetRenderForGUIEditor(true);
	l_pImage->SetIsSelected(true);

	window->AddGuiElement( l_pImage );

	CElementProperties::ElementProperties(l_pImage);
}

Vect2f CElementCreator::GetPositionToAdd(Vect2i screen)
{
	Vect2i pos = CORE->GetInputManager()->GetMouse()->GetPosition();

	//Convert the position of the mouse to a % value
	float valueX = 100.f/(float)screen.x;
	float valueY = 100.f /(float)screen.y;

	return Vect2f( valueX * pos.x, valueY * pos.y);
}

std::string CElementCreator::GetSufixNumber(CGUIWindow *window)
{
	uint32 size = window->GetNumElements();

	std::stringstream out;
	out << size;
	 return out.str();
}

void CElementCreator::CleanSelections(CGUIWindow *window)
{
	//hace reset de todos los controles
	CGuiElement *element = NULL;

	uint32 count = window->GetNumElements();
	for( uint32 i = 0; i < count; ++i)
	{
		element = window->GetElementById(i);
		element->SetIsSelected(false);
	}
}