#pragma once

#ifndef _ELEMENT_PROPERTIES_H_
#define _ELEMENT_PROPERTIES_H_

#include <string>
#include "GuiElement.h"

class CElementProperties
{
public:
	static CElementProperties* GetInstance();
	void CleanUp();

	static void ElementProperties(CGuiElement *element);

	void SetMFCPropertyGricCtrl(CMFCPropertyGridCtrl *MFCProperty) { m_pMFCProperty = MFCProperty; }
	CMFCPropertyGridCtrl* GetMFCPropertyGricCtrl() const { return m_pMFCProperty; }

private:
	static void ButtonProperties(CGuiElement *element);
	static void ImageProperties(CGuiElement *element);

	static CMFCPropertyGridProperty* AddBasicAppearanceProperties(CGuiElement *element);
	static CMFCPropertyGridProperty* AddBasicInformationProperties(CGuiElement *element);

	static std::string TypeElement2String( CGuiElement::TypeGuiElement type );

private: 
	CElementProperties();
	~CElementProperties();

private:
	static CElementProperties *m_pElementProperties;
	CMFCPropertyGridCtrl *m_pMFCProperty;
};

#endif