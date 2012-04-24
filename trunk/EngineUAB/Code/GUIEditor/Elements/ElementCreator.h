#pragma once

#ifndef _ELEMENT_CREATOR_H_
#define _ELEMENT_CREATOR_H_

#include "ElementManager.h"
#include "Math\Vector2.h"
#include <string>

class CGUIWindow;
class CGuiElement;

class CElementCreator
{
public:
	static void CreateElement(TElement type, CGUIWindow *window);	

private:
	static CGuiElement* CreateButton(CGUIWindow *window);
	static CGuiElement* CreateCheckButton(CGUIWindow *window);
	static CGuiElement* CreateEditableTextBox(CGUIWindow *window);
	static CGuiElement* CreateImage(CGUIWindow *window);
	static CGuiElement* CreateProgressBar(CGUIWindow *window);
	static CGuiElement*	CreateSlider(CGUIWindow *window);
	static CGuiElement* CreateStaticText(CGUIWindow *window);

	static Vect2f GetPositionToAdd(Vect2i screen);
	static std::string GetSufixNumber(CGUIWindow *window);

	static void CleanSelections(CGUIWindow *window);
};

#endif