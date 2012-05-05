#pragma once

#ifndef _ELEMENT_SAVER_H
#define _ELEMENT_SAVER_H

#include <d3dx9.h>
#include "GraphicsDefs.h"
#include <string>

class CGuiElement;
class CGUIWindow;
class CColor;

class CElementSaver
{
public:
	static void SaveProperties( CGuiElement *element );
	static void SaveWindowProperties( CGUIWindow *window );

private:
	static void AnimatedImageProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );
	static void SaveButtonProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );
	static void SaveCheckButtonProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );
	static void SaveDialogBoxProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );
	static void SaveEditableTextBoxProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );
	static void SaveImageProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );
	static void SaveProgressBarProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );
	static void SaveSliderProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );
	static void SaveStaticTextProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );

	static ETypeFlip String2Flip(const std::string &type);

	static CColor ConvertColor (COleVariant variant);
};

#endif