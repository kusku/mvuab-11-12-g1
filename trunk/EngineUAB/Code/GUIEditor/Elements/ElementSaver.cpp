#include "stdafx.h"
#include "ElementSaver.h"
#include "ElementProperties.h"
#include "GuiElement.h"
#include "GUIManager.h"
#include "Controls\GUIImage.h"
#include "Controls\GUIButton.h"
#include "Controls\GUICheckButton.h"
#include "Controls\GUIStaticText.h"
#include "Controls\GUIEditableTextBox.h"
#include "Controls\GUIProgressBar.h"
#include "Controls\GUISlider.h"
#include "Controls\GUIDialogBox.h"
#include "Controls\GUIAnimatedImage.h"
#include "GUIWindow.h"
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
	case CGuiElement::TypeGuiElement::ANIMATED_IMAGE:
		{
			AnimatedImageProperties( element, properties );
			break;
		}
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
	case CGuiElement::TypeGuiElement::DIALOG_BOX:
		{
			SaveDialogBoxProperties( element, properties );
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
	case CGuiElement::TypeGuiElement::PROGRESS_BAR:
		{
			SaveProgressBarProperties( element, properties );
			break;
		}
	case CGuiElement::TypeGuiElement::SLIDER:
		{
			SaveSliderProperties( element, properties );
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

void CElementSaver::AnimatedImageProperties(CGuiElement *element, CMFCPropertyGridCtrl *properties)
{
	CGUIAnimatedImage *animated_element = static_cast<CGUIAnimatedImage*>(element);

	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	animated_element->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	animated_element->SetVisible( (value.boolVal == VARIANT_TRUE) );

	Vect2f pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = static_cast<float>( value.intVal );
	animated_element->SetPositionPercent( pos );

	Vect2f size;
	value = properties->GetProperty(2)->GetSubItem(0)->GetValue();
	size.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(2)->GetSubItem(1)->GetValue();
	size.y = static_cast<float>( value.intVal );
	animated_element->SetWidthPercent( size.x );
	animated_element->SetHeightPercent( size.y );

	//-----------------------------------------
	//Propiedades de información
	//-----------------------------------------
	value = properties->GetProperty(3)->GetSubItem(2)->GetValue();
	animated_element->SetName( std::string( _bstr_t( value.bstrVal ) ) );

	value = properties->GetProperty(3)->GetSubItem(3)->GetValue();
	animated_element->SetLiteral( std::string( _bstr_t( value.bstrVal ) ) );

	//-----------------------------------------
	//Propiedades de texturas
	//-----------------------------------------
	int l_PrevNumTextures = animated_element->NumFrames();
	value = properties->GetProperty(4)->GetSubItem(0)->GetValue();
	int l_CurrentNumTextures = value.intVal;

	CTexture *texture = NULL;

	if( l_PrevNumTextures != l_CurrentNumTextures )
	{
		std::vector<CTexture*> l_Textures;
		for(uint16 i=0; i<l_PrevNumTextures; ++i)
		{
			l_Textures.push_back( animated_element->GetTexture(i) );
		}

		animated_element->DeleteTextures();
		for(uint16 i=0; i<(uint16)l_CurrentNumTextures; ++i)
		{
			if( i < l_PrevNumTextures )
			{
				animated_element->AddFrame( l_Textures[i] );
			}
			else
			{
				animated_element->AddFrame(NULL);
			}
		}

		l_Textures.clear();
	}
	else
	{
		CTextureManager *TM = CORE->GetTextureManager();

		animated_element->DeleteTextures();
		for(int i=0; i<l_CurrentNumTextures; ++i)
		{
			value = properties->GetProperty(4)->GetSubItem(i+1)->GetValue();
			std::string texture_path = std::string( _bstr_t( value.bstrVal ) );
			texture = TM->GetTexture( texture_path );

			animated_element->AddFrame( texture );
		}
	}

	//-----------------------------------------
	//Propiedades de scripts
	//-----------------------------------------
	//OnLoad
	value = properties->GetProperty(5)->GetSubItem(0)->GetValue();
	std::string script = std::string( _bstr_t( value.bstrVal ) );
	animated_element->SetOnLoadValueAction( script );

	//OnSave
	value = properties->GetProperty(5)->GetSubItem(1)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	animated_element->SetOnLoadValueAction( script );

	if( l_PrevNumTextures != l_CurrentNumTextures )
	{
		CElementProperties::ElementProperties(animated_element);
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

void CElementSaver::SaveCheckButtonProperties(CGuiElement *element, CMFCPropertyGridCtrl *properties)
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

void CElementSaver::SaveDialogBoxProperties(CGuiElement *element, CMFCPropertyGridCtrl *properties)
{
	CGUIDialogBox *dialogbox_element = static_cast<CGUIDialogBox*>(element);

	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	dialogbox_element->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	dialogbox_element->SetVisible( (value.boolVal == VARIANT_TRUE) );

	Vect2f pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = static_cast<float>(value.intVal);
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = static_cast<float>(value.intVal);
	dialogbox_element->SetPositionPercent( pos );

	Vect2f size;
	value = properties->GetProperty(2)->GetSubItem(0)->GetValue();
	size.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(2)->GetSubItem(1)->GetValue();
	size.y = static_cast<float>( value.intVal );
	dialogbox_element->SetWidthPercent( size.x );
	dialogbox_element->SetHeightPercent( size.y );

	//-----------------------------------------
	//Propiedades de información
	//-----------------------------------------
	value = properties->GetProperty(3)->GetSubItem(2)->GetValue();
	dialogbox_element->SetName( std::string( _bstr_t( value.bstrVal ) ) );

	value = properties->GetProperty(3)->GetSubItem(3)->GetValue();
	dialogbox_element->SetLiteral( std::string( _bstr_t( value.bstrVal ) ) );

	//-----------------------------------------
	//Propiedades de texturas del botón de move
	//-----------------------------------------
	CTexture *l_pNormalTexture = NULL;
	CTexture *l_pOverTexture = NULL;
	CTexture *l_pClickedTexture = NULL;
	CTexture *l_pDeactivatedTexture = NULL;
	CTexture *l_pBackgroundTexture = NULL;
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	value = properties->GetProperty(4)->GetSubItem(0)->GetValue();
	std::string texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pNormalTexture = l_pTextureManager->GetTexture( texture_path );

	value = properties->GetProperty(4)->GetSubItem(1)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pOverTexture = l_pTextureManager->GetTexture( texture_path );

	value = properties->GetProperty(4)->GetSubItem(2)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pClickedTexture = l_pTextureManager->GetTexture( texture_path );

	value = properties->GetProperty(4)->GetSubItem(3)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pDeactivatedTexture = l_pTextureManager->GetTexture( texture_path );
	
	dialogbox_element->SetMoveButtonTextures( l_pNormalTexture, l_pOverTexture, l_pClickedTexture, l_pDeactivatedTexture );

	//-----------------------------------------
	//Propiedades de texturas del botón de close
	//-----------------------------------------
	value = properties->GetProperty(5)->GetSubItem(0)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pNormalTexture = l_pTextureManager->GetTexture( texture_path );

	value = properties->GetProperty(5)->GetSubItem(1)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pOverTexture = l_pTextureManager->GetTexture( texture_path );

	value = properties->GetProperty(5)->GetSubItem(2)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pClickedTexture = l_pTextureManager->GetTexture( texture_path );

	value = properties->GetProperty(5)->GetSubItem(3)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pDeactivatedTexture = l_pTextureManager->GetTexture( texture_path );
	
	dialogbox_element->SetCloseButtonTextures( l_pNormalTexture, l_pOverTexture, l_pClickedTexture, l_pDeactivatedTexture );

	//-----------------------------------------
	//Propiedades de texturas de background
	//-----------------------------------------
	value = properties->GetProperty(6)->GetSubItem(0)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pBackgroundTexture = l_pTextureManager->GetTexture( texture_path );

	dialogbox_element->SetDialogTexture( l_pBackgroundTexture );

	//-----------------------------------------
	//Propiedades de scripts
	//-----------------------------------------
	//OnLoad
	value = properties->GetProperty(7)->GetSubItem(0)->GetValue();
	std::string script = std::string( _bstr_t( value.bstrVal ) );
	dialogbox_element->SetOnLoadValueAction( script );

	//OnSave
	value = properties->GetProperty(7)->GetSubItem(1)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	dialogbox_element->SetOnSaveValueAction( script );
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
	
	value = properties->GetProperty(4)->GetSubItem(1)->GetValue();
	CColor backgroundColor = ConvertColor(value);
	
	value = properties->GetProperty(4)->GetSubItem(2)->GetValue();
	CColor textColor = ConvertColor(value);

	textbox_element->SetFont( textColor, l_iFont);
	textbox_element->SetBackGroundColor( backgroundColor );

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

void CElementSaver::SaveProgressBarProperties(CGuiElement *element, CMFCPropertyGridCtrl *properties)
{
	CGUIProgressBar *progressbar_element = static_cast<CGUIProgressBar*>(element);

	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	progressbar_element->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	progressbar_element->SetVisible( (value.boolVal == VARIANT_TRUE) );

	Vect2f pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = static_cast<float>( value.intVal );
	progressbar_element->SetPositionPercent( pos );

	Vect2f size;
	value = properties->GetProperty(2)->GetSubItem(0)->GetValue();
	size.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(2)->GetSubItem(1)->GetValue();
	size.y = static_cast<float>( value.intVal );
	progressbar_element->SetWidthPercent( size.x );
	progressbar_element->SetHeightPercent( size.y );

	//-----------------------------------------
	//Propiedades de información
	//-----------------------------------------
	value = properties->GetProperty(3)->GetSubItem(2)->GetValue();
	progressbar_element->SetName( std::string( _bstr_t( value.bstrVal ) ) );

	value = properties->GetProperty(3)->GetSubItem(3)->GetValue();
	progressbar_element->SetLiteral( std::string( _bstr_t( value.bstrVal ) ) );

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

	value = properties->GetProperty(4)->GetSubItem(1)->GetValue();
	CColor backgroundColor = ConvertColor( value );

	value = properties->GetProperty(4)->GetSubItem(2)->GetValue();
	CColor progressColor = ConvertColor( value );

	value = properties->GetProperty(4)->GetSubItem(3)->GetValue();
	CColor textColor = ConvertColor( value );

	progressbar_element->SetColors( backgroundColor, progressColor);
	progressbar_element->SetFont( l_iFont, textColor );

	//-----------------------------------------
	//Propiedades de texturas
	//-----------------------------------------
	CTexture *l_pBackgroundTexture = NULL;
	CTexture *l_pProgressTexture = NULL;
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	//Textura On
	value = properties->GetProperty(5)->GetSubItem(0)->GetValue();
	std::string texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pBackgroundTexture = l_pTextureManager->GetTexture( texture_path );
	if( l_pBackgroundTexture == l_pTextureManager->GetNoTexture() )
	{
		l_pBackgroundTexture = NULL;
	}

	value = properties->GetProperty(5)->GetSubItem(1)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pProgressTexture = l_pTextureManager->GetTexture( texture_path );
	if( l_pProgressTexture == l_pTextureManager->GetNoTexture() )
	{
		l_pProgressTexture = NULL;
	}
	
	progressbar_element->SetTextures( l_pBackgroundTexture, l_pProgressTexture );

	//-----------------------------------------
	//Propiedades de scripts
	//-----------------------------------------
	value = properties->GetProperty(6)->GetSubItem(0)->GetValue();
	std::string script = std::string( _bstr_t( value.bstrVal ) );
	progressbar_element->SetOnLoadValueAction( script );

	value = properties->GetProperty(6)->GetSubItem(1)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	progressbar_element->SetOnSaveValueAction( script );

	value = properties->GetProperty(6)->GetSubItem(2)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	progressbar_element->SetOnComplete( script );
}

void CElementSaver::SaveSliderProperties(CGuiElement *element, CMFCPropertyGridCtrl *properties)
{
	CGUISlider *slider_element = static_cast<CGUISlider*>(element);

	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	slider_element->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	slider_element->SetVisible( (value.boolVal == VARIANT_TRUE) );

	Vect2f pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = static_cast<float>( value.intVal );
	slider_element->SetPositionPercent( pos );

	Vect2f size;
	value = properties->GetProperty(2)->GetSubItem(0)->GetValue();
	size.x = static_cast<float>( value.intVal );
	value = properties->GetProperty(2)->GetSubItem(1)->GetValue();
	size.y = static_cast<float>( value.intVal );
	slider_element->SetWidthPercent( size.x );
	slider_element->SetHeightPercent( size.y );

	//-----------------------------------------
	//Propiedades de información
	//-----------------------------------------
	value = properties->GetProperty(3)->GetSubItem(2)->GetValue();
	slider_element->SetName( std::string( _bstr_t( value.bstrVal ) ) );

	value = properties->GetProperty(3)->GetSubItem(3)->GetValue();
	slider_element->SetLiteral( std::string( _bstr_t( value.bstrVal ) ) );

	//-----------------------------------------
	//Propiedades del botón
	//-----------------------------------------
	Vect2i button_size;
	value = properties->GetProperty(5)->GetSubItem(0)->GetValue();
	button_size.x = value.intVal;
	value = properties->GetProperty(5)->GetSubItem(1)->GetValue();
	button_size.y = value.intVal;

	slider_element->SetWidthButton( static_cast<float>(button_size.x) );
	slider_element->SetHeightButton( static_cast<float>(button_size.y) );

	//-----------------------------------------
	//Propiedades de texturas
	//-----------------------------------------
	CTexture *l_pNormalTexture = NULL;
	CTexture *l_pOverTexture = NULL;
	CTexture *l_pClickedTexture = NULL;
	CTexture *l_pDeactivatedTexture = NULL;
	CTexture *l_pSliderTexture = NULL;
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	//Textura On
	value = properties->GetProperty(6)->GetSubItem(0)->GetValue();
	std::string texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pNormalTexture = l_pTextureManager->GetTexture( texture_path );

	value = properties->GetProperty(6)->GetSubItem(1)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pOverTexture = l_pTextureManager->GetTexture( texture_path );
	
	value = properties->GetProperty(6)->GetSubItem(2)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pClickedTexture = l_pTextureManager->GetTexture( texture_path );

	value = properties->GetProperty(6)->GetSubItem(3)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pDeactivatedTexture = l_pTextureManager->GetTexture( texture_path );

	value = properties->GetProperty(6)->GetSubItem(4)->GetValue();
	texture_path = std::string( _bstr_t( value.bstrVal ) );
	l_pSliderTexture = l_pTextureManager->GetTexture( texture_path );

	slider_element->SetButtonTextures( l_pNormalTexture, l_pOverTexture, l_pClickedTexture, l_pDeactivatedTexture );
	slider_element->SetBackGroundTexture( l_pSliderTexture );

	//-----------------------------------------
	//Propiedades de scripts
	//-----------------------------------------
	value = properties->GetProperty(7)->GetSubItem(0)->GetValue();
	std::string script = std::string( _bstr_t( value.bstrVal ) );
	slider_element->SetOnLoadValueAction( script );

	value = properties->GetProperty(7)->GetSubItem(1)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	slider_element->SetOnSaveValueAction( script );

	value = properties->GetProperty(7)->GetSubItem(2)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	slider_element->SetOnChangeValueAction( script );

	value = properties->GetProperty(7)->GetSubItem(3)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	slider_element->SetOnOverAction( script );

	value = properties->GetProperty(7)->GetSubItem(4)->GetValue();
	script = std::string( _bstr_t( value.bstrVal ) );
	slider_element->SetOnClickedAction( script );
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

CColor CElementSaver::ConvertColor(COleVariant variant)
{
	//TODO: Convertir el color
	return colBLACK;
}

void CElementSaver::SaveWindowProperties(CGUIWindow *window)
{
	CMFCPropertyGridCtrl *properties = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();
	
	std::string current_window = CORE->GetGUIManager()->GetCurrentWindow();

	//Information
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	std::string window_name = std::string(_bstr_t(value.bstrVal));

	if( CORE->GetGUIManager()->ChangeWindowName(current_window, window_name) )
	{
		window->SetName( window_name );
		CORE->GetGUIManager()->ActiveWindows( window_name );

		CString* current_string_window = new CString( current_window.c_str() );
		CString* new_string_window = new CString( window_name.c_str()  );
		PostMessage( CHWNDManager::GetInstance()->GetHWNDFiles(), WM_CHANGE_WINDOW_NAME, (WPARAM)current_string_window, (LPARAM)new_string_window);
	}

	//Scripts
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	window->SetOnLoadWindows( std::string(_bstr_t(value.bstrVal)) );

	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	window->SetOnSaveWindows( std::string(_bstr_t(value.bstrVal)) );

	value = properties->GetProperty(1)->GetSubItem(2)->GetValue();
	window->SetOnUpdateWindows( std::string(_bstr_t(value.bstrVal)) );
}

#pragma warning(pop)