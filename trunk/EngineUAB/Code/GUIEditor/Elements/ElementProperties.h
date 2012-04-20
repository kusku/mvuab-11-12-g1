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

	static void ButtonProperties(const std::string &guielement);
	static void ImageProperties(const std::string &guielement);

	void SetMFCPropertyGricCtrl(CMFCPropertyGridCtrl *MFCProperty) { m_pMFCProperty = MFCProperty; }
	CMFCPropertyGridCtrl* GetMFCPropertyGricCtrl() const { return m_pMFCProperty; }

private:
	static void AddBasicAppearanceProperties(const std::string &guielement);
	static void AddBasicInformationProperties(const std::string &guielement);

	static std::string TypeElement2String( CGuiElement::TypeGuiElement type );

private: 
	CElementProperties();
	~CElementProperties();

private:
	static CElementProperties *m_pElementProperties;
	CMFCPropertyGridCtrl *m_pMFCProperty;
};

#endif