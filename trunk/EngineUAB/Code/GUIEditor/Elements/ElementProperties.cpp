#include "stdafx.h"
#include "ElementProperties.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "GuiElement.h"
#include "Controls\GUIImage.h"
#include "Controls\GUIButton.h"
#include "Controls\GUICheckButton.h"
#include "Controls\GUIEditableTextBox.h"
#include "Controls\GUIProgressBar.h"
#include "Controls\GUISlider.h"
#include "Textures\Texture.h"
#include "RenderManager.h"
#include "Fonts\FontManager.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#pragma warning(push)
#pragma warning(disable : 4482)

CElementProperties::CElementProperties()
{
}

CElementProperties::~CElementProperties()
{
}

void CElementProperties::CleanUp()
{
	CHECKED_DELETE(m_pElementProperties);
}

CElementProperties* CElementProperties::m_pElementProperties = NULL;

CElementProperties* CElementProperties::GetInstance()
{
	if( m_pElementProperties == NULL )
	{
		m_pElementProperties  = new CElementProperties();
	}

	return m_pElementProperties ;
}


#pragma warning(push)
#pragma warning(disable : 4482)

void CElementProperties::ElementProperties(CGuiElement *element)
{
	CGuiElement::TypeGuiElement type = element->GetType();

	switch( type )
	{
	case CGuiElement::TypeGuiElement::BUTTON:
		{
			ButtonProperties(element);
			break;
		}
	case CGuiElement::TypeGuiElement::CHECKBUTTON:
		{
			CheckButtonProperties(element);
			break;
		}
	case CGuiElement::TypeGuiElement::EDITABLE_TEXT_BOX:
		{
			EditableTextBoxProperties(element);
			break;
		}
	case CGuiElement::TypeGuiElement::IMAGE:
		{
			ImageProperties(element);
			break;
		}
	case CGuiElement::TypeGuiElement::PROGRESS_BAR:
		{
			ProgressBarProperties(element);
			break;
		}
	case CGuiElement::TypeGuiElement::SLIDER:
		{
			SliderProperties(element);
			break;
		}
	case CGuiElement::TypeGuiElement::STATIC_TEXT:
		{
			StaticTextProperties(element);
			break;
		}
	}
}

#pragma warning(pop)

void CElementProperties::ButtonProperties(CGuiElement *element)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	MFCProperty->RemoveAll();

	//Añadir propiedades comunes
	AddBasicAppearanceProperties( element );

	//Añadir propiedades de información
	AddBasicInformationProperties( element );

	//Texturas
	CMFCPropertyGridProperty* pTexture = new CMFCPropertyGridProperty(_T("Texturas"));
	CGUIButton *l_pButton = static_cast<CGUIButton*>(element);
	CTexture *l_pTexture = l_pButton->GetNormalTexture();
	std::string l_NormalPath = l_pTexture != NULL ? l_pTexture->GetFileName() : "";

	l_pTexture = l_pButton->GetOverTexture();
	std::string l_OverPath = l_pTexture != NULL ? l_pTexture->GetFileName() : "";

	l_pTexture = l_pButton->GetClickedTexture();
	std::string l_ClickedPath = l_pTexture != NULL ? l_pTexture->GetFileName() : "";

	l_pTexture = l_pButton->GetDeactivatedTexture();
	std::string l_DeactivatedPath = l_pTexture != NULL ? l_pTexture->GetFileName() : "";

	static const TCHAR szFilter[] = _T("JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|Todos los archivos(*.*)|*.*||");
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Normal"), TRUE, _T(l_NormalPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura en estado de reposo")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Over"), TRUE, _T(l_OverPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura cuando el mouse está encima")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Clicked"), TRUE, _T(l_ClickedPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura cuando el mouse hace click")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Deactivated"), TRUE, _T(l_DeactivatedPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura cuando el botón está desactivado")));
	MFCProperty->AddProperty(pTexture);

	//Script
	CMFCPropertyGridProperty* pScript = new CMFCPropertyGridProperty(_T("Script"));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnLoad"), (_variant_t) _T( l_pButton->GetOnLoad().c_str() ), _T("Especifica el código de scripting al cargar el elemento")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnSave"), (_variant_t) _T( l_pButton->GetOnSave().c_str() ), _T("Especifica el código de scripting al guardar el elemento")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnClicked"), (_variant_t) _T( l_pButton->GetOnClicked().c_str() ), _T("Especifica el código de scripting al hacer click")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnOver"), (_variant_t) _T( l_pButton->GetOnOver().c_str() ), _T("Especifica el código de scripting cuando tiene el mouse encima")));

	MFCProperty->AddProperty(pScript);
}

void CElementProperties::CheckButtonProperties(CGuiElement *element)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	MFCProperty->RemoveAll();

	//Añadir propiedades comunes
	AddBasicAppearanceProperties( element );

	//Añadir propiedades de información
	AddBasicInformationProperties( element );

	//Texturas
	CMFCPropertyGridProperty* pTexture = new CMFCPropertyGridProperty(_T("Texturas"));
	CGUICheckButton *l_pCheckButton = static_cast<CGUICheckButton*>(element);
	CTexture *l_pTexture = l_pCheckButton->GetOnTexture();
	std::string l_OnPath = l_pTexture != NULL ? l_pTexture->GetFileName() : "";

	l_pTexture = l_pCheckButton->GetOffTexture();
	std::string l_OffPath = l_pTexture != NULL ? l_pTexture->GetFileName() : "";

	l_pTexture = l_pCheckButton->GetDeactivatedTexture();
	std::string l_DeactivatedPath = l_pTexture != NULL ? l_pTexture->GetFileName() : "";

	static const TCHAR szFilter[] = _T("JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|Todos los archivos(*.*)|*.*||");
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("On"), TRUE, _T(l_OnPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura cuando el check button está encendido")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Off"), TRUE, _T(l_OffPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura cuando el check button está apagado")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Deactivated"), TRUE, _T(l_DeactivatedPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura cuando el elemento está desactivado")));
	MFCProperty->AddProperty(pTexture);

	//Script
	CMFCPropertyGridProperty* pScript = new CMFCPropertyGridProperty(_T("Script"));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnLoad"), (_variant_t) _T( l_pCheckButton->GetOnLoad().c_str() ), _T("Especifica el código de scripting al cargar el elemento")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnSave"), (_variant_t) _T( l_pCheckButton->GetOnSave().c_str() ), _T("Especifica el código de scripting al guardar el elemento")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnOver"), (_variant_t) _T( l_pCheckButton->GetOnOver().c_str() ), _T("Especifica el código de scripting cuando tiene el mouse encima")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnCheckOn"), (_variant_t) _T( l_pCheckButton->GetOnCheckOn().c_str() ), _T("Especifica el código de scripting al encender el check button")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnCheckOff"), (_variant_t) _T( l_pCheckButton->GetOnCheckOff().c_str() ), _T("Especifica el código de scripting al apagar el check button")));

	MFCProperty->AddProperty(pScript);
}

void CElementProperties::EditableTextBoxProperties(CGuiElement *element)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	MFCProperty->RemoveAll();

	CGUIEditableTextBox *l_pTextBox = static_cast<CGUIEditableTextBox*>(element);

	//Añadir propiedades comunes
	AddBasicAppearanceProperties( element );

	//Añadir propiedades de información
	AddBasicInformationProperties( element );

	//Estilo
	CMFCPropertyGridProperty* pStyle = new CMFCPropertyGridProperty(_T("Estilo"));
	
	CFontManager *FM = CORE->GetFontManager();
	std::string font = FM->GetTTFName( l_pTextBox->GetFontID() );

	int found = font.rfind("\\");
	if( found == -1 ) found = font.rfind("/");
	std::string default_font = font.substr(found+1, font.size() );

	pProp = new CMFCPropertyGridProperty(_T("Font"), _T(default_font.c_str()), _T("Especifica la fuente"));
	for(uint32 i=0; i<FM->GetNumFonts(); ++i)
	{
		int found = FM->GetTTFName(i).rfind("\\");
		if( found == -1 ) found = FM->GetTTFName(i).rfind("/");
		std::string font = FM->GetTTFName(i).substr(found+1, FM->GetTTFName(i).size() );

		pProp->AddOption( _T(font.c_str()) );
	}
	pProp->AllowEdit(FALSE);
	pStyle->AddSubItem(pProp);

	CColor color = l_pTextBox->GetBackGroundColor();
	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Background Color"), RGB(color.GetRed(), color.GetGreen(), color.GetBlue()), NULL, _T("Especifica el color de fondo"));
	pColorProp->EnableOtherButton(_T("Otro..."));
	pColorProp->EnableAutomaticButton(_T("Predeterminado"), ::GetSysColor(COLOR_3DFACE));
	pStyle->AddSubItem(pColorProp);

	color = l_pTextBox->GetTextColor();
	CMFCPropertyGridColorProperty* pColorTextProp = new CMFCPropertyGridColorProperty(_T("Text Color"), RGB(color.GetRed(), color.GetGreen(), color.GetBlue()), NULL, _T("Especifica el color del texto"));
	pColorTextProp->EnableOtherButton(_T("Otro..."));
	pColorTextProp->EnableAutomaticButton(_T("Predeterminado"), ::GetSysColor(COLOR_3DFACE));
	pStyle->AddSubItem(pColorTextProp);

	MFCProperty->AddProperty(pStyle);

	//Texturas
	CMFCPropertyGridProperty* pTexture = new CMFCPropertyGridProperty(_T("Texturas"));
	CTexture *l_pTexture = l_pTextBox->GetBackGroundTexture();
	std::string l_BackgroundPath = l_pTexture != NULL ? l_pTexture->GetFileName() : "";

	static const TCHAR szFilter[] = _T("JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|Todos los archivos(*.*)|*.*||");
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Background"), TRUE, _T(l_BackgroundPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura de fondo")));

	MFCProperty->AddProperty(pTexture);

	//Script
	CMFCPropertyGridProperty* pScript = new CMFCPropertyGridProperty(_T("Script"));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnLoad"), (_variant_t) _T( element->GetOnLoad().c_str() ), _T("Especifica el código de scripting al cargar el elemento")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnSave"), (_variant_t) _T( element->GetOnSave().c_str() ), _T("Especifica el código de scripting al guardar el elemento")));

	MFCProperty->AddProperty(pScript);
}

void CElementProperties::ImageProperties(CGuiElement *element)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	MFCProperty->RemoveAll();

	//Añadir propiedades comunes
	AddBasicAppearanceProperties( element );

	//Añadir propiedades de información
	AddBasicInformationProperties( element );

	//Texturas
	CMFCPropertyGridProperty* pTexture = new CMFCPropertyGridProperty(_T("Texturas"));
	
	CTexture* texture = ( static_cast<CGUIImage*>(element) )->GetTexture( "normal" );
	CTexture* texture_default = ( static_cast<CGUIImage*>(element) )->GetTexture( "default_normal" );
	std::string texture_path = texture != NULL ? texture->GetFileName() : texture_default->GetFileName();

	static const TCHAR szFilter[] = _T("JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|Todos los archivos(*.*)|*.*||");
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Normal"), TRUE, _T(texture_path.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura")));

	MFCProperty->AddProperty(pTexture);

	//Script
	CMFCPropertyGridProperty* pScript = new CMFCPropertyGridProperty(_T("Script"));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnLoad"), (_variant_t) _T( element->GetOnLoad().c_str() ), _T("Especifica el código de scripting al cargar")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnSave"), (_variant_t) _T( element->GetOnSave().c_str() ), _T("Especifica el código de scripting al guardar")));

	MFCProperty->AddProperty(pScript);
}

void CElementProperties::ProgressBarProperties(CGuiElement *element)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	CGUIProgressBar *l_pProgressBar = static_cast<CGUIProgressBar*>(element);

	MFCProperty->RemoveAll();

	//Añadir propiedades comunes
	AddBasicAppearanceProperties( element );

	//Añadir propiedades de información
	AddBasicInformationProperties( element );

	//Estilo
	CMFCPropertyGridProperty* pStyle = new CMFCPropertyGridProperty(_T("Estilo"));
	
	CFontManager *FM = CORE->GetFontManager();
	std::string font = FM->GetTTFName( l_pProgressBar->GetFontID() );

	int found = font.rfind("\\");
	if( found == -1 ) found = font.rfind("/");
	std::string default_font = font.substr(found+1, font.size() );

	pProp = new CMFCPropertyGridProperty(_T("Font"), _T(default_font.c_str()), _T("Especifica la fuente"));
	for(uint32 i=0; i<FM->GetNumFonts(); ++i)
	{
		int found = FM->GetTTFName(i).rfind("\\");
		if( found == -1 ) found = FM->GetTTFName(i).rfind("/");
		std::string font = FM->GetTTFName(i).substr(found+1, FM->GetTTFName(i).size() );

		pProp->AddOption( _T(font.c_str()) );
	}
	pProp->AllowEdit(FALSE);
	pStyle->AddSubItem(pProp);

	CColor color = l_pProgressBar->GetBackGroundColor();
	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Background Color"), RGB(color.GetRed(), color.GetGreen(), color.GetBlue()), NULL, _T("Especifica el color de fondo"));
	pColorProp->EnableOtherButton(_T("Otro..."));
	pColorProp->EnableAutomaticButton(_T("Predeterminado"), ::GetSysColor(COLOR_3DFACE));
	pStyle->AddSubItem(pColorProp);

	color = l_pProgressBar->GetProgressColor();
	CMFCPropertyGridColorProperty* pColorProgressProp = new CMFCPropertyGridColorProperty(_T("Progress Color"), RGB(color.GetRed(), color.GetGreen(), color.GetBlue()), NULL, _T("Especifica el color que indica el progreso de la barra"));
	pColorProgressProp->EnableOtherButton(_T("Otro..."));
	pColorProgressProp->EnableAutomaticButton(_T("Predeterminado"), ::GetSysColor(COLOR_3DFACE));
	pStyle->AddSubItem(pColorProgressProp);

	color = l_pProgressBar->GetTextColor();
	CMFCPropertyGridColorProperty* pColorTextProp = new CMFCPropertyGridColorProperty(_T("Text Color"), RGB(color.GetRed(), color.GetGreen(), color.GetBlue()), NULL, _T("Especifica el color del texto"));
	pColorTextProp->EnableOtherButton(_T("Otro..."));
	pColorTextProp->EnableAutomaticButton(_T("Predeterminado"), ::GetSysColor(COLOR_3DFACE));
	pStyle->AddSubItem(pColorTextProp);

	MFCProperty->AddProperty(pStyle);

	//Texturas
	CMFCPropertyGridProperty* pTexture = new CMFCPropertyGridProperty(_T("Texturas"));
	CTexture *l_pTexture = l_pProgressBar->GetBackgroundTexture();
	CTexture *l_pProgressTexture = l_pProgressBar->GetProgressTexture();
	std::string l_BackgroundPath = l_pTexture != NULL ? l_pTexture->GetFileName() : "";
	std::string l_ProgressPath = l_pProgressTexture != NULL ? l_pProgressTexture->GetFileName() : "";

	static const TCHAR szFilter[] = _T("JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|Todos los archivos(*.*)|*.*||");
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Background"), TRUE, _T(l_BackgroundPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura de fondo")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Progress"), TRUE, _T(l_ProgressPath.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura de progreso")));

	MFCProperty->AddProperty(pTexture);

	//Script
	CMFCPropertyGridProperty* pScript = new CMFCPropertyGridProperty(_T("Script"));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnLoad"), (_variant_t) _T( l_pProgressBar->GetOnLoad().c_str() ), _T("Especifica el código de scripting al cargar")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnSave"), (_variant_t) _T( l_pProgressBar->GetOnSave().c_str() ), _T("Especifica el código de scripting al guardar")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnComplete"), (_variant_t) _T( l_pProgressBar->GetOnComplete().c_str() ), _T("Especifica el código de scripting al completar la barra")));

	MFCProperty->AddProperty(pScript);
}

void CElementProperties::SliderProperties(CGuiElement *element)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	MFCProperty->RemoveAll();

	CGUISlider *l_pSlider = static_cast<CGUISlider*>(element);

	//Añadir propiedades comunes
	AddBasicAppearanceProperties( element );

	//Añadir propiedades de información
	AddBasicInformationProperties( element );

	//Botón
	CMFCPropertyGridProperty* pButton = new CMFCPropertyGridProperty(_T("Botón"));
	MFCProperty->AddProperty(pButton);

	//TODO: Arreglar el problema del tamaño del botón del slider
	/*CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Tamaño Botón(%)"), 0, TRUE);
	pProp = new CMFCPropertyGridProperty( _T("Ancho"), (_variant_t) l_pSlider->GetButtonWidth(), _T("Ancho del botón del slider"));
	pProp->EnableSpinControl(TRUE, 1, 100);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Alto"), (_variant_t) l_pSlider->GetButtonHeight(), _T("Alto del botón del slider"));
	pProp->EnableSpinControl(TRUE, 1, 100);
	pSize->AddSubItem(pProp);

	MFCProperty->AddProperty(pSize);*/

	//texturas
	CMFCPropertyGridProperty* pTexture = new CMFCPropertyGridProperty(_T("Texturas"));
	CTexture *background = l_pSlider->GetBackgroundTexture();
	CTexture *normal = l_pSlider->GetNormalButtonTexture();
	CTexture *over = l_pSlider->GetOverButtonTexture();
	CTexture *clicked = l_pSlider->GetClickedButtonTexture();
	CTexture *deactivated = l_pSlider->GetDeactivatedButtonTexture();

	std::string normal_path = normal != NULL ? normal->GetFileName() : "";
	std::string over_path = over != NULL ? over->GetFileName() : "";
	std::string clicked_path = clicked != NULL ? clicked->GetFileName() : "";
	std::string deactivated_path = deactivated != NULL ? deactivated->GetFileName() : "";
	std::string background_path = background != NULL ? background->GetFileName() : "";

	static const TCHAR szFilter[] = _T("JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|Todos los archivos(*.*)|*.*||");
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Normal"), TRUE, _T(normal_path.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura para el botón")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Over"), TRUE, _T(over_path.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura para el botón")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Clicked"), TRUE, _T(clicked_path.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura para el botón")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Deactivated"), TRUE, _T(deactivated_path.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura para el botón")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Slider"), TRUE, _T(background_path.c_str()), _T("jpg"), 0, szFilter, _T("Especifica la textura para el slider")));
	MFCProperty->AddProperty(pTexture);

	//Scripts
	//Script
	CMFCPropertyGridProperty* pScript = new CMFCPropertyGridProperty(_T("Script"));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnLoad"), (_variant_t) _T( l_pSlider->GetOnLoad().c_str() ), _T("Especifica el código de scripting al cargar")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnSave"), (_variant_t) _T( l_pSlider->GetOnSave().c_str() ), _T("Especifica el código de scripting al guardar")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnChangeValue"), (_variant_t) _T( l_pSlider->GetOnChangeValue().c_str() ), _T("Especifica el código de scripting al mover el slider")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnOver"), (_variant_t) _T( l_pSlider->GetOnOverAction().c_str() ), _T("Especifica el código de scripting al estar encima del botón del slider")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("OnClicked"), (_variant_t) _T( l_pSlider->GetOnClickedAction().c_str() ), _T("Especifica el código de scripting al clickar en el botón del slider")));

	MFCProperty->AddProperty(pScript);
}

void CElementProperties::StaticTextProperties(CGuiElement *element)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	MFCProperty->RemoveAll();

	//Añadir propiedades comunes
	AddBasicAppearanceProperties( element );

	//Añadir propiedades de información
	AddBasicInformationProperties( element );
}

CMFCPropertyGridProperty* CElementProperties::AddBasicAppearanceProperties(CGuiElement *element)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	CGuiElement *l_pElement = element;

	if( element != NULL )
	{
		Vect2i screen = CORE->GetRenderManager()->GetScreenSize();

		CMFCPropertyGridProperty* pAppearance = new CMFCPropertyGridProperty(_T("Apariencia"));
		MFCProperty->AddProperty(pAppearance);

		//Visible & Active
		pAppearance->AddSubItem(new CMFCPropertyGridProperty(_T("Activo"), (_variant_t) true, _T("Especifica si está activo el objeto")));
		pAppearance->AddSubItem(new CMFCPropertyGridProperty(_T("Visible"), (_variant_t) true, _T("Especifica si está visible el objeto")));

		//Flip
		if( element->GetType() == CGuiElement::TypeGuiElement::IMAGE )
		{
			CGUIImage *l_pImage = static_cast<CGUIImage*>(element);
			
			std::string l_szFlip = "None";
			switch( l_pImage->GetFlip() )
			{
			case NONE_FLIP: l_szFlip = "None"; break;
			case FLIP_X: l_szFlip = "Flip X"; break;
			case FLIP_Y: l_szFlip = "Flip Y"; break;
			default: l_szFlip = "None"; break;
			}

			pProp = new CMFCPropertyGridProperty(_T("Flip"), _T(l_szFlip.c_str()), _T("Especifica el flip del objeto"));
			pProp->AddOption(_T("None"));
			pProp->AddOption(_T("Flip X"));
			pProp->AddOption(_T("Flip Y"));
			pProp->AllowEdit(FALSE);

			pAppearance->AddSubItem(pProp);
		}

		//Propiedad de posición
		Vect2i pos = Vect2i( (int)l_pElement->GetPositionPercent().x, (int)l_pElement->GetPositionPercent().y);
		CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("Posición (%)"), 0, TRUE);
		pProp = new CMFCPropertyGridProperty(_T("X"), (_variant_t) pos.x, _T("Posición X del objecto"));
		pProp->EnableSpinControl(TRUE, 1, screen.y);
		pPosition->AddSubItem(pProp);
		

		pProp = new CMFCPropertyGridProperty( _T("Y"), (_variant_t) pos.y, _T("Posición Y del objecto"));
		pProp->EnableSpinControl(TRUE, 1, screen.x);
		pPosition->AddSubItem(pProp);

		MFCProperty->AddProperty(pPosition);

		//Propiedad de tamaño
		Vect2i size = Vect2i( (int)l_pElement->GetWidthPercent(), (int)l_pElement->GetHeightPercent() );
		CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Tamaño (%)"), 0, TRUE);
		pProp = new CMFCPropertyGridProperty( _T("Ancho"), (_variant_t) size.x, _T("Ancho del objeto"));
		pProp->EnableSpinControl(TRUE, 1, screen.x);
		pSize->AddSubItem(pProp);

		pProp = new CMFCPropertyGridProperty(_T("Alto"), (_variant_t) size.y, _T("Alto del objeto"));
		pProp->EnableSpinControl(TRUE, 1, screen.y);
		pSize->AddSubItem(pProp);


		MFCProperty->AddProperty(pSize);

		return pAppearance;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CElementProperties::AddBasicAppearanceProperties->No se ha encontrado el elemento para obtener las propiedades.");
	}

	return NULL;
}

CMFCPropertyGridProperty* CElementProperties::AddBasicInformationProperties(CGuiElement *element)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	if( element != NULL )
	{
		CMFCPropertyGridProperty* pInformation = new CMFCPropertyGridProperty(_T("Información"));
		MFCProperty->AddProperty(pInformation);

		pProp = new CMFCPropertyGridProperty(_T("(ID)"), element->GetID().c_str() );
		pProp->Enable(FALSE);
		pInformation->AddSubItem(pProp);
		
		pProp = new CMFCPropertyGridProperty(_T("(Tipo)"), TypeElement2String( element->GetType() ).c_str() );
		pProp->Enable(FALSE);
		pInformation->AddSubItem(pProp);

		pProp = new CMFCPropertyGridProperty(_T("Nombre"), element->GetName().c_str());
		pProp->Enable(TRUE);
		pInformation->AddSubItem(pProp);

		pProp = new CMFCPropertyGridProperty(_T("Literal"), element->GetLiteral().c_str());
		pProp->Enable(TRUE);
		pInformation->AddSubItem(pProp);

		return pInformation;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CElementProperties::AddBasicInformationProperties->No se ha encontrado el elemento para obtener las propiedades.");
	}

	return NULL;
}

std::string CElementProperties::TypeElement2String(CGuiElement::TypeGuiElement type)
{
	std::string type_string;

	switch(type)
	{
	case CGuiElement::TypeGuiElement::BUTTON:
		{
			type_string = "Button";
			break;
		}
	case CGuiElement::TypeGuiElement::CHECKBUTTON:
		{
			type_string = "Check Button";
			break;
		}
	case CGuiElement::TypeGuiElement::STATIC_TEXT:
		{
			type_string = "Static Text";
			break;
		}
	case CGuiElement::TypeGuiElement::SLIDER:
		{
			type_string = "Slider";
			break;
		}
	case CGuiElement::TypeGuiElement::IMAGE:
		{
			type_string = "Image";
			break;
		}
	case CGuiElement::TypeGuiElement::EDITABLE_TEXT_BOX:
		{
			type_string = "Editable Text Box";
			break;
		}
	case CGuiElement::TypeGuiElement::PROGRESS_BAR:
		{
			type_string = "Progress Bar";
			break;
		}
	case CGuiElement::TypeGuiElement::DIALOG_BOX:
		{
			type_string = "Dialog Box";
			break;
		}
	case CGuiElement::TypeGuiElement::RADIO_BOX:
		{
			type_string = "Radio Box";
			break;
		}
	default:
		{
			type_string = "";
			break;
		}
	}

	return type_string;
}

#pragma warning(pop)