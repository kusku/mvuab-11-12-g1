#pragma once

#ifndef _ELEMENT_SAVER_H
#define _ELEMENT_SAVER_H

#include <d3dx9.h>
#include "GraphicsDefs.h"
#include <string>

class CGuiElement;

class CElementSaver
{
public:
	static void SaveProperties( CGuiElement *element );

private:
	static void SaveButtonProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );
	static void SaveImageProperties( CGuiElement *element, CMFCPropertyGridCtrl *properties );

	static ETypeFlip String2Flip(const std::string &type);
};

#endif