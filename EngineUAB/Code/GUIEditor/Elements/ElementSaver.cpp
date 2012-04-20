#include "stdafx.h"
#include "ElementSaver.h"
#include "ElementProperties.h"
#include "GuiElement.h"
#include "Controls\GUIImage.h"
#include <comdef.h>

#pragma warning(push)
#pragma warning(disable : 4482)

void CElementSaver::SaveProperties(CGuiElement *element)
{
	CGuiElement::TypeGuiElement type = element->GetType();
	CMFCPropertyGridCtrl *properties = CElementProperties::GetInstance()->GetMFCPropertyGricCtrl();

	switch( type )
	{
	case CGuiElement::TypeGuiElement::BUTTON:
		{
			SaveButtonProperties( element, properties );
			break;
		}
	case CGuiElement::TypeGuiElement::IMAGE:
		{
			SaveImageProperties( element, properties );
			break;
		}
	}
}

void CElementSaver::SaveButtonProperties(CGuiElement *element, CMFCPropertyGridCtrl *properties)
{
	//-----------------------------------------
	//Propiedades de apariencia
	//-----------------------------------------
	COleVariant value = properties->GetProperty(0)->GetSubItem(0)->GetValue();
	element->SetActive( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(1)->GetValue();
	element->SetVisible( (value.boolVal == VARIANT_TRUE) );

	value = properties->GetProperty(0)->GetSubItem(2)->GetValue();
	BSTR name = value.bstrVal;
	std::string test = _bstr_t(name);
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

	//value = properties->GetProperty(0)->GetSubItem(2)->GetValue();
	//BSTR name = value.bstrVal;
	//image_element->SetFlip( String2Flip( std::string(_bstr_t(name)) ) );

	Vect2i pos;
	value = properties->GetProperty(1)->GetSubItem(0)->GetValue();
	pos.x = value.intVal;
	value = properties->GetProperty(1)->GetSubItem(1)->GetValue();
	pos.y = value.intVal;
	image_element->SetPositionPercent( Vect2f( (float)pos.x, (float)pos.y) );
}

//ETypeFlip CElementSaver::String2Flip(const std::string &type)
//{
//	if( type == "None" )
//	{
//		return NONE_FLIP;
//	}
//	else if( type == "Flip X" )
//	{
//		return FLIP_X;
//	}
//	else if( type == "Flip Y" )
//	{
//		return FLIP_Y;
//	}
//	
//	return NONE_FLIP;
//}

#pragma warning(pop)