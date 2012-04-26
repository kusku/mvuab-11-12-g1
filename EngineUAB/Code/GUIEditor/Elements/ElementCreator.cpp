#include "stdafx.h"
#include "ElementCreator.h"
#include "InputManager.h"
#include "Periphericals\Mouse.h"
#include "RenderManager.h"
#include "GUIWindow.h"
#include "Controls\GUIImage.h"
#include "Controls\GUIButton.h"
#include "Controls\GUICheckButton.h"
#include "Controls\GUIStaticText.h"
#include "Controls\GUIProgressBar.h"
#include "Controls\GUIEditableTextBox.h"
#include "Controls\GUISlider.h"
#include "Controls\GUIDialogBox.h"
#include "Controls\GUIAnimatedImage.h"
#include "Controls\GUIRadioBox.h"
#include "Textures\TextureManager.h"
#include "Textures\Texture.h"
#include "ElementProperties.h"
#include "Core.h"
#include "Base.h"
#include "defines.h"
#include "HWNDManager.h"
#include <sstream>

void CElementCreator::CreateElement(TElement type, CGUIWindow *window)
{
	CGuiElement *l_pElement = NULL;

	switch( type )
	{
	case ANIMATED_IMAGE:
		{
			l_pElement = CreateAnimatedImage(window);
			break;
		}
	case BUTTON:
		{
			l_pElement = CreateButton(window);
			break;
		}
	case CHECK_BUTTON:
		{
			l_pElement = CreateCheckButton(window);
			break;
		}
	case DIALOG_BOX:
		{
			l_pElement = CreateDialogBox(window);
			break;
		}
	case EDITABLE_TEXT_BOX:
		{
			l_pElement = CreateEditableTextBox(window);
			break;
		}
	case IMAGE:
		{
			l_pElement = CreateImage(window);
			break;
		}
	case PROGRESS_BAR:
		{
			l_pElement = CreateProgressBar(window);
			break;
		}
	case RADIO_BOX:
		{
			l_pElement = CreateRadioBox(window);
			break;
		}
	case SLIDER:
		{
			l_pElement = CreateSlider(window);
			break;
		}
	case STATIC_TEXT:
		{
			l_pElement = CreateStaticText(window);
			break;
		}
	}

	PostMessage( CHWNDManager::GetInstance()->GetHWNDFiles(), WM_ADD_ELEMENT_FILE, (WPARAM)l_pElement->GetName().c_str(), 0);
}

CGuiElement* CElementCreator::CreateAnimatedImage(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUIAnimatedImage *l_pAnimatedImage = new CGUIAnimatedImage(screen.y, screen.x, 20.f, 20.f, l_Position, "", 0, 0, true, true);
	std::string l_Name = "animated_image_" + GetSufixNumber(window);
	l_pAnimatedImage->SetName( l_Name );
	l_pAnimatedImage->SetID( l_Name );

	//TODO: Faltan las texturas
	CTexture *default_texture = CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/animatedimage.jpg");
	l_pAnimatedImage->SetDefaultTexture( default_texture );

	l_pAnimatedImage->SetOnLoadValueAction("");
	l_pAnimatedImage->SetOnSaveValueAction("");

	l_pAnimatedImage->SetRenderForGUIEditor(true);
	l_pAnimatedImage->SetIsSelected(true);

	window->AddGuiElement( l_pAnimatedImage );

	CElementProperties::ElementProperties(l_pAnimatedImage);

	return l_pAnimatedImage;
}

CGuiElement* CElementCreator::CreateButton(CGUIWindow *window)
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

	return l_pButton;
}

CGuiElement* CElementCreator::CreateCheckButton(CGUIWindow *window)
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

	return l_pCheckButton;
}

CGuiElement* CElementCreator::CreateDialogBox(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUIDialogBox *l_pDialog = new CGUIDialogBox(screen.y, screen.x, 30.f, 30.f, l_Position, 10.f, 10.f, "", 0, 0, true, true);
	std::string l_Name = "dialog_box_" + GetSufixNumber(window);
	l_pDialog->SetName( l_Name );
	l_pDialog->SetID( l_Name );
	
	CTexture *background = CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/dialogbox.jpg");
	l_pDialog->SetDialogTexture(background);

	CTexture *button = CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/button.jpg");
	l_pDialog->SetCloseButtonTextures(button, button, button, button);
	l_pDialog->SetMoveButtonTextures(button, button, button, button);

	l_pDialog->SetOnLoadValueAction("");
	l_pDialog->SetOnSaveValueAction("");

	l_pDialog->SetRenderForGUIEditor( true );
	l_pDialog->SetIsSelected( true );

	window->AddGuiElement( l_pDialog );

	CElementProperties::ElementProperties( l_pDialog );

	return l_pDialog;
}

CGuiElement* CElementCreator::CreateEditableTextBox(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUIEditableTextBox *l_pTextBox = new CGUIEditableTextBox(screen.y, screen.x, 10.f, 20.f, l_Position, colBLACK, 0, "", 0, 0, true, true);
	std::string l_Name = "editable_text_box_" + GetSufixNumber(window);
	l_pTextBox->SetName( l_Name );
	l_pTextBox->SetID( l_Name );
	l_pTextBox->SetBuffer("hola món");
	l_pTextBox->SetFont( colBLACK, 0 );
	l_pTextBox->SetBackGroundColor( colWHITE );
	l_pTextBox->SetOnLoadValueAction("");
	l_pTextBox->SetOnSaveValueAction("");
	l_pTextBox->SetBackGroundTexture( NULL );

	l_pTextBox->SetRenderForGUIEditor( true );
	l_pTextBox->SetIsSelected( true );

	window->AddGuiElement( l_pTextBox );

	CElementProperties::ElementProperties( l_pTextBox );

	return l_pTextBox;
}

CGuiElement* CElementCreator::CreateImage(CGUIWindow *window)
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

	return l_pImage;
}

CGuiElement* CElementCreator::CreateProgressBar(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUIProgressBar *l_pProgressBar = new CGUIProgressBar(screen.y, screen.x, 10.f, 30.f, l_Position, "", 0, 0, true, true);
	std::string l_Name = "progress_bar_" + GetSufixNumber(window);
	l_pProgressBar->SetName( l_Name );
	l_pProgressBar->SetID( l_Name );

	CTexture *texture = CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/progressbar.jpg");
	l_pProgressBar->SetTextures( texture, texture );
	l_pProgressBar->SetColors(colWHITE, colBLUE);
	l_pProgressBar->SetFont(0, colBLACK);
	l_pProgressBar->SetOnLoadValueAction("");
	l_pProgressBar->SetOnSaveValueAction("");
	l_pProgressBar->SetOnComplete("");

	l_pProgressBar->SetRenderForGUIEditor( true );
	l_pProgressBar->SetIsSelected( true );

	window->AddGuiElement( l_pProgressBar );

	CElementProperties::ElementProperties( l_pProgressBar );

	return l_pProgressBar;
}

CGuiElement* CElementCreator::CreateRadioBox(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUIRadioBox *l_pRadioBox = new CGUIRadioBox(screen.y, screen.x, 30.f, 30.f, l_Position, 1, 1, "", "", 0, 0, true, true);
	std::string l_Name = "radio_box_" + GetSufixNumber(window);
	l_pRadioBox->SetName(l_Name);
	l_pRadioBox->SetID(l_Name);

	CTexture *texture = CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/radiobox.jpg");
	l_pRadioBox->SetTextureBack(texture);

	l_pRadioBox->SetCheckButtonActions("", "", "");
	l_pRadioBox->SetOnCheckButton("");

	l_pRadioBox->SetRenderForGUIEditor( true );
	l_pRadioBox->SetIsSelected( true );

	window->AddGuiElement( l_pRadioBox );

	CElementProperties::ElementProperties( l_pRadioBox); 

	return l_pRadioBox;
}

CGuiElement* CElementCreator::CreateSlider(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUISlider *l_pSlider = new CGUISlider( screen.y, screen.x, 10.f, 30.f, l_Position, 9.f, 9.f, 0, "", 0, 0, true, true );
	std::string l_Name = "slider_" + GetSufixNumber(window);
	l_pSlider->SetName( l_Name );
	l_pSlider->SetID( l_Name );

	l_pSlider->SetOnLoadValueAction("");
	l_pSlider->SetOnSaveValueAction("");
	l_pSlider->SetOnClickedAction(std::string(""));
	l_pSlider->SetOnOverAction(std::string(""));
	l_pSlider->SetOnChangeValueAction(std::string(""));

	CTexture *button_texture = CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/button.jpg");
	l_pSlider->SetButtonTextures( button_texture, button_texture, button_texture, button_texture);

	CTexture *slider_texture = CORE->GetTextureManager()->GetTexture("./Data/General/Textures/GUI/slider.jpg");
	l_pSlider->SetBackGroundTexture( slider_texture );

	l_pSlider->SetButtonColors( colBLACK, colBLACK, colBLACK, colBLACK) ;
	l_pSlider->SetBackGroundColor( colBLACK );

	l_pSlider->SetRenderForGUIEditor( true );
	l_pSlider->SetIsSelected( true );

	window->AddGuiElement( l_pSlider );

	CElementProperties::ElementProperties( l_pSlider); 

	return l_pSlider;
}

CGuiElement* CElementCreator::CreateStaticText(CGUIWindow *window)
{
	CleanSelections(window);

	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	Vect2f l_Position = GetPositionToAdd(screen);

	CGUIStaticText *l_pStaticText = new CGUIStaticText( screen.y, screen.x, 20.f, 30.f, l_Position, "STATIC TEXT", true, true);
	std::string l_Name = "static_text_" + GetSufixNumber(window);
	l_pStaticText->SetName( l_Name );
	l_pStaticText->SetID( l_Name );

	l_pStaticText->SetRenderForGUIEditor( true );
	l_pStaticText->SetIsSelected( true );

	window->AddGuiElement( l_pStaticText );

	CElementProperties::ElementProperties( l_pStaticText );

	return l_pStaticText;
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