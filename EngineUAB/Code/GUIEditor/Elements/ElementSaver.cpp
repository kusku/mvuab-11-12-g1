#include "stdafx.h"
#include "ElementSaver.h"
#include "ElementProperties.h"
#include "GuiElement.h"
#include "Controls\GUIImage.h"
#include "Controls\GUIButton.h"
#include "Controls\GUICheckButton.h"
#include "Controls\GUIStaticText.h"
#include "Controls\GUIEditableTextBox.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "HWNDManager.h"
#include "Fonts\FontManager.h"
#include "Core.h"
#include "Base.h"
#include "defines.h"
#include <comdef.h>

#pragma warning(push)
#pragma warning(disable : 4482)

void CElementSaver::SaveProperties(CGuiElement *element)
{
	CGuiElement::TypeGuiElement type = element->GetType();
	CMFCPropertyGridCtrl *properties = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	std::string l_Name = element->GetName();

	switch( type )
	{
	case CGuiElement::TypeGuiElement::BUTTON:
		{
			SaveButtonProperties( element, properties );
			break;
		}
	case CGuiElement::TypeGuiElement::CHECKBUTTON:
		{
			SaveCheckButtonProperties( element, properties );
			break;
		}
	case CGuiElement::TypeGuiElement::EDITABLE_TEXT_BOX:
		{
			SaveEditableTextBoxProperties( element, properties );
			break;
		}
	case CGuiElement::TypeGuiElement::IMAGE:
		{
			SaveImageProperties( element, properties );
			break;
		}
	case CGuiElement::TypeGuiElement::STATIC_TEXT:
		{
			SaveStaticTextProperties( element, properties );
			break;
		}
	}

	std::string l_NewName = element->GetName();
	if( l_Name != l_NewName )
	{
		//Actualizamos la lista de ficheros
		CString *original = new CString(element->GetID().c_str());
		CString *modified = new CString(l_NewName.c_str());
		PostMessage( CHWNDManager::GetInstance()->GetHWNDFiles(), WM_UPDATE_FILE_DATA, (WPARAM)original,(LPARAM)modified);
	}
}

void CElementSaver::SaveButtonProperties(CGuiElement *element, CMFCPropertyGridCtrl *properties)
{
	CGUIButton *button_element = static_cast<CGUIButton*>(element);

	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	button_element->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	button_element->SetVisible( (value.boolVal == VARIANT_TRUE) );

	Vect2f pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = static_cast<float>( value.intVal );
	button_element->SetPositionPercent( pos );

	Vect2f size;
	value = properties->GetProperty(2)->GetSubItem(0)->GetValue();
	size.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(2)->GetSubItem(1)->GetValue();
	size.y = static_cast<float>( value.intVal );
	button_element->SetWidthPercent( size.x );
	button_element->SetHeightPercent( size.y );

	//-----------------------------------------
	//Propiedades de información
	//-----------------------------------------
	value = properties->GetProperty(3)->GetSubItem(2)->GetValue();
	button_element->SetName( std::string( _bstr_t( value.bstrVal ) ) );

	value = properties->GetProperty(3)->GetSubItem(3)->GetValue();
	button_element->SetLiteral( std::string( _bstr_t( value.bstrVal ) ) );

	//-----------------------------------------
	//Propiedades de texturas
	//-----------------------------------------
	CTexture *l_pTextureNormal = NULL;
	CTexture *l_pTextureOver = NULL;
	CTexture *l_pTextureClicked = NULL;
	CTexture *l_pTextureDeactivated = NULL;
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	//Textura Normal
	value = properties->GetProperty(4)->GetSubItem(0)->GetValue();
	std::string texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pTextureNormal = l_pTextureManager->GetTexture( texture_path );

	//Textura Over
	value = properties->GetProperty(4)->GetSubItem(1)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pTextureOver = l_pTextureManager->GetTexture( texture_path );

	//Textura Clicked
	value = properties->GetProperty(4)->GetSubItem(2)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pTextureClicked = l_pTextureManager->GetTexture( texture_path );

	//Textura Deactivated
	value = properties->GetProperty(4)->GetSubItem(3)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pTextureDeactivated = l_pTextureManager->GetTexture( texture_path );

	button_element->SetTextures( l_pTextureNormal, l_pTextureOver, l_pTextureClicked, l_pTextureDeactivated );

	//-----------------------------------------
	//Propiedades de scripts
	//-----------------------------------------
	//OnLoad
	value = properties->GetProperty(5)->GetSubItem(0)->GetValue();
	std::string script = std::string( _bstr_t( value.bstrVal ) );
	button_element->SetOnLoadValueAction( script );
	
	//OnSave
	value = properties->GetProperty(5)->GetSubItem(1)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	button_element->SetOnSaveValueAction( script );

	//OnClicked
	value = properties->GetProperty(5)->GetSubItem(2)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	button_element->SetOnClickedAction( script );

	//OnOver
	value = properties->GetProperty(5)->GetSubItem(3)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	button_element->SetOnOverAction( script );
}

void CElementSaver::SaveCheckButtonProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties )
{
	CGUICheckButton *checkbutton_element = static_cast<CGUICheckButton*>(element);

	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	checkbutton_element->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	checkbutton_element->SetVisible( (value.boolVal == VARIANT_TRUE) );

	Vect2f pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = static_cast<float>( value.intVal );
	checkbutton_element->SetPositionPercent( pos );

	Vect2f size;
	value = properties->GetProperty(2)->GetSubItem(0)->GetValue();
	size.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(2)->GetSubItem(1)->GetValue();
	size.y = static_cast<float>( value.intVal );
	checkbutton_element->SetWidthPercent( size.x );
	checkbutton_element->SetHeightPercent( size.y );

	//-----------------------------------------
	//Propiedades de información
	//-----------------------------------------
	value = properties->GetProperty(3)->GetSubItem(2)->GetValue();
	checkbutton_element->SetName( std::string( _bstr_t( value.bstrVal ) ) );

	value = properties->GetProperty(3)->GetSubItem(3)->GetValue();
	checkbutton_element->SetLiteral( std::string( _bstr_t( value.bstrVal ) ) );

	//-----------------------------------------
	//Propiedades de texturas
	//-----------------------------------------
	CTexture *l_pTextureOn = NULL;
	CTexture *l_pTextureOff = NULL;
	CTexture *l_pTextureDeactivated = NULL;
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	//Textura On
	value = properties->GetProperty(4)->GetSubItem(0)->GetValue();
	std::string texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pTextureOn = l_pTextureManager->GetTexture( texture_path );

	//Textura Off
	value = properties->GetProperty(4)->GetSubItem(1)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pTextureOff = l_pTextureManager->GetTexture( texture_path );

	//Textura Deactivated
	value = properties->GetProperty(4)->GetSubItem(1)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pTextureDeactivated = l_pTextureManager->GetTexture( texture_path );

	checkbutton_element->SetTextures( l_pTextureOn, l_pTextureOff, l_pTextureDeactivated );

	//-----------------------------------------
	//Propiedades de scripts
	//-----------------------------------------
	//OnLoad
	value = properties->GetProperty(5)->GetSubItem(0)->GetValue();
	std::string script = std::string( _bstr_t( value.bstrVal ) );
	checkbutton_element->SetOnLoadValueAction( script );

	//OnSave
	value = properties->GetProperty(5)->GetSubItem(1)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	checkbutton_element->SetOnSaveValueAction( script );

	//OnCheckOn
	value = properties->GetProperty(5)->GetSubItem(2)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	checkbutton_element->SetOnCheckOnAction( script );

	//OnCheckOff
	value = properties->GetProperty(5)->GetSubItem(3)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	checkbutton_element->SetOnCheckOffAction( script );

	//OnOvrer
	value = properties->GetProperty(5)->GetSubItem(4)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	checkbutton_element->SetOnOverAction( script );
}

void CElementSaver::SaveEditableTextBoxProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties )
{
	CGUIEditableTextBox *textbox_element = static_cast<CGUIEditableTextBox*>(element);

	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	textbox_element->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	textbox_element->SetVisible( (value.boolVal == VARIANT_TRUE) );

	Vect2f pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = static_cast<float>( value.intVal );
	textbox_element->SetPositionPercent( pos );

	Vect2f size;
	value = properties->GetProperty(2)->GetSubItem(0)->GetValue();
	size.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(2)->GetSubItem(1)->GetValue();
	size.y = static_cast<float>( value.intVal );
	textbox_element->SetWidthPercent( size.x );
	textbox_element->SetHeightPercent( size.y );

	//-----------------------------------------
	//Propiedades de información
	//-----------------------------------------
	value = properties->GetProperty(3)->GetSubItem(2)->GetValue();
	textbox_element->SetName( std::string( _bstr_t( value.bstrVal ) ) );

	value = properties->GetProperty(3)->GetSubItem(3)->GetValue();
	textbox_element->SetLiteral( std::string( _bstr_t( value.bstrVal ) ) );

	//-----------------------------------------
	//Propiedades de estilo
	//-----------------------------------------
	value = properties->GetProperty(4)->GetSubItem(0)->GetValue();

	int l_iFont = 0;
	CFontManager *FM = CORE->GetFontManager();
	for(uint32 i=0; i<FM->GetNumFonts(); ++i)
	{
		std::string font = FM->GetTTFName(i);
		int found = font.rfind("\\");
		if( found == -1 ) found = font.rfind("/");
		font = font.substr(found+1, font.size());

		std::string currentfont = _bstr_t(value.bstrVal );
		if( currentfont == font )
		{
			l_iFont = i;
			break;
		}
	}
	//TODO: Guardar los colores
	value = properties->GetProperty(4)->GetSubItem(1)->GetValue();
	//int blue = value.intVal >> 16;
	//int green = value.intVal >> 8;
	
	value = properties->GetProperty(4)->GetSubItem(2)->GetValue();

	textbox_element->SetFont( colBLACK, l_iFont);

	//-----------------------------------------
	//Propiedades de texturas
	//-----------------------------------------
	CTexture *l_pBackgroundTexture = NULL;
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	//Textura On
	value = properties->GetProperty(5)->GetSubItem(0)->GetValue();
	std::string texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pBackgroundTexture = l_pTextureManager->GetTexture( texture_path );
	if( l_pBackgroundTexture == l_pTextureManager->GetNoTexture() )
	{
		l_pBackgroundTexture = NULL;
	}
	textbox_element->SetBackGroundTexture( l_pBackgroundTexture );

	//-----------------------------------------
	//Propiedades de scripts
	//-----------------------------------------
	//OnLoad
	value = properties->GetProperty(6)->GetSubItem(0)->GetValue();
	std::string script = std::string( _bstr_t( value.bstrVal ) );
	textbox_element->SetOnLoadValueAction( script );

	//OnSave
	value = properties->GetProperty(6)->GetSubItem(1)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	textbox_element->SetOnSaveValueAction( script );
}

void CElementSaver::SaveImageProperties(CGuiElement *element, CMFCPropertyGridCtrl *properties)
{
	CGUIImage *image_element = static_cast<CGUIImage*>(element);

	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	image_element->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	image_element->SetVisible( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(2)->GetValue();
	BSTR name = value.bstrVal;
	image_element->SetFlip( String2Flip( std::string(_bstr_t(name)) ) );

	Vect2f pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = static_cast<float>( value.intVal );
	image_element->SetPositionPercent( pos );

	Vect2f size;
	value = properties->GetProperty(2)->GetSubItem(0)->GetValue();
	size.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(2)->GetSubItem(1)->GetValue();
	size.y = static_cast<float>( value.intVal );
	image_element->SetWidthPercent( size.x );
	image_element->SetHeightPercent( size.y );

	//-----------------------------------------
	//Propiedades de información
	//-----------------------------------------
	value = properties->GetProperty(3)->GetSubItem(2)->GetValue();
	image_element->SetName( std::string( _bstr_t( value.bstrVal ) ) );

	value = properties->GetProperty(3)->GetSubItem(3)->GetValue();
	image_element->SetLiteral( std::string( _bstr_t( value.bstrVal ) ) );

	//-----------------------------------------
	//Propiedades de texturas
	//-----------------------------------------
	value = properties->GetProperty(4)->GetSubItem(0)->GetValue();
	std::string texture_path = std::string( _bstr_t( value.bstrVal ) );
	std::string texture_name = texture_path != "" ? "normal" : "default_normal";

	if( texture_name == "normal" )
	{
		CTexture *texture = CORE->GetTextureManager()->GetTexture( texture_path );
		image_element->SetTexture( texture, texture_name );
	}

	image_element->SetActiveTexture( texture_name );

	//-----------------------------------------
	//Propiedades de scripts
	//-----------------------------------------
	value = properties->GetProperty(5)->GetSubItem(0)->GetValue();
	std::string script = std::string( _bstr_t( value.bstrVal ) );
	image_element->SetOnLoadValueAction( script );

	value = properties->GetProperty(5)->GetSubItem(1)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	image_element->SetOnSaveValueAction( script );
}

void CElementSaver::SaveStaticTextProperties(CGuiElement *element, CMFCPropertyGridCtrl *properties)
{
	CGUIStaticText *l_pStaticText = static_cast<CGUIStaticText*>(element);

	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	l_pStaticText->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	l_pStaticText->SetVisible( (value.boolVal == VARIANT_TRUE) );

	Vect2f pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = static_cast<float>( value.intVal );
	l_pStaticText->SetPositionPercent( pos );

	Vect2f size;
	value = properties->GetProperty(2)->GetSubItem(0)->GetValue();
	size.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(2)->GetSubItem(1)->GetValue();
	size.y = static_cast<float>( value.intVal );
	l_pStaticText->SetWidthPercent( size.x );
	l_pStaticText->SetHeightPercent( size.y );

	//-----------------------------------------
	//Propiedades de información
	//-----------------------------------------
	value = properties->GetProperty(3)->GetSubItem(2)->GetValue();
	l_pStaticText->SetName( std::string( _bstr_t( value.bstrVal ) ) );

	value = properties->GetProperty(3)->GetSubItem(3)->GetValue();
	l_pStaticText->SetLiteral( std::string( _bstr_t( value.bstrVal ) ) );
}

ETypeFlip CElementSaver::String2Flip(const std::string &type)
{
	if( type == "None" )
	{
		return NONE_FLIP;
	}
	else if( type == "Flip X" )
	{
		return FLIP_X;
	}
	else if( type == "Flip Y" )
	{
		return FLIP_Y;
	}
	
	return NONE_FLIP;
}

#pragma warning(pop)