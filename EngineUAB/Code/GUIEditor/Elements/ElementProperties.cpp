#include "stdafx.h"
#include "ElementProperties.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "GuiElement.h"
#include "RenderManager.h"
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

void CElementProperties::ButtonProperties(const std::string &guielement)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	MFCProperty->RemoveAll();

	//Añadir propiedades comunes
	AddBasicAppearanceProperties( guielement );

	//Añadir propiedades de información
	AddBasicInformationProperties( guielement );

	//Texturas
	CMFCPropertyGridProperty* pTexture = new CMFCPropertyGridProperty(_T("Texturas"));

	static const TCHAR szFilter[] = _T("JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|Todos los archivos(*.*)|*.*||");
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Normal"), TRUE, _T(""), _T("jpg"), 0, szFilter, _T("Especifica la textura en estado de reposo")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Over"), TRUE, _T(""), _T("jpg"), 0, szFilter, _T("Especifica la textura cuando el mouse está encima")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Clicked"), TRUE, _T(""), _T("jpg"), 0, szFilter, _T("Especifica la textura cuando el mouse hace click")));
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Deactivated"), TRUE, _T(""), _T("jpg"), 0, szFilter, _T("Especifica la textura cuando el botón está desactivado")));
	MFCProperty->AddProperty(pTexture);

	//Script
	CMFCPropertyGridProperty* pScript = new CMFCPropertyGridProperty(_T("Script"));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("On clicked"), (_variant_t) _T(""), _T("Especifica el código de scripting al hacer click")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("On over"), (_variant_t) _T(""), _T("Especifica el código de scripting cuando tiene el mouse encima")));

	MFCProperty->AddProperty(pScript);
}

void CElementProperties::ImageProperties(const std::string &guielement)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	MFCProperty->RemoveAll();

	//Añadir propiedades comunes
	AddBasicAppearanceProperties( guielement );

	//Añadir propiedades de información
	AddBasicInformationProperties( guielement );

	//Texturas
	CMFCPropertyGridProperty* pTexture = new CMFCPropertyGridProperty(_T("Texturas"));

	static const TCHAR szFilter[] = _T("JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|Todos los archivos(*.*)|*.*||");
	pTexture->AddSubItem(new CMFCPropertyGridFileProperty(_T("Normal"), TRUE, _T(""), _T("jpg"), 0, szFilter, _T("Especifica la textura")));

	MFCProperty->AddProperty(pTexture);

	//Script
	CMFCPropertyGridProperty* pScript = new CMFCPropertyGridProperty(_T("Script"));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("On load"), (_variant_t) _T(""), _T("Especifica el código de scripting al cargar")));
	pScript->AddSubItem(new CMFCPropertyGridProperty(_T("On save"), (_variant_t) _T(""), _T("Especifica el código de scripting al guardar")));

	MFCProperty->AddProperty(pScript);
}

CMFCPropertyGridProperty* CElementProperties::AddBasicAppearanceProperties(const std::string &guielement)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	std::string window = CORE->GetGUIManager()->GetCurrentWindow();
	CGUIWindow *win = CORE->GetGUIManager()->GetWindow( window );
	CGuiElement *l_pElement = win->GetElement( guielement );

	if( l_pElement != NULL )
	{
		Vect2i screen = CORE->GetRenderManager()->GetScreenSize();

		CMFCPropertyGridProperty* pAppearance = new CMFCPropertyGridProperty(_T("Apariencia"));
		MFCProperty->AddProperty(pAppearance);

		//Visible & Active
		pAppearance->AddSubItem(new CMFCPropertyGridProperty(_T("Activo"), (_variant_t) true, _T("Especifica si está activo el objeto")));
		pAppearance->AddSubItem(new CMFCPropertyGridProperty(_T("Visible"), (_variant_t) true, _T("Especifica si está visible el objeto")));

		//Flip
		if( l_pElement->GetType() == CGuiElement::TypeGuiElement::IMAGE )
		{
			pProp = new CMFCPropertyGridProperty(_T("Flip"), _T("None"), _T("Especifica el flip del objeto"));
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

CMFCPropertyGridProperty* CElementProperties::AddBasicInformationProperties(const std::string &guielement)
{
	CMFCPropertyGridProperty* pProp = NULL;
	CMFCPropertyGridCtrl *MFCProperty = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	std::string window = CORE->GetGUIManager()->GetCurrentWindow();
	CGUIWindow *win = CORE->GetGUIManager()->GetWindow( window );
	CGuiElement *l_pElement = win->GetElement( guielement );

	if( l_pElement != NULL )
	{
		CMFCPropertyGridProperty* pInformation = new CMFCPropertyGridProperty(_T("Información"));
		MFCProperty->AddProperty(pInformation);

		pProp = new CMFCPropertyGridProperty(_T("(ID)"), l_pElement->GetID().c_str() );
		pProp->Enable(FALSE);
		pInformation->AddSubItem(pProp);
		
		pProp = new CMFCPropertyGridProperty(_T("(Tipo)"), TypeElement2String( l_pElement->GetType() ).c_str() );
		pProp->Enable(FALSE);
		pInformation->AddSubItem(pProp);

		pProp = new CMFCPropertyGridProperty(_T("Nombre"), l_pElement->GetName().c_str());
		pProp->Enable(TRUE);
		pInformation->AddSubItem(pProp);

		pProp = new CMFCPropertyGridProperty(_T("Literal"), l_pElement->GetLiteral().c_str());
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