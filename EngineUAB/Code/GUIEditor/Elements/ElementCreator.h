#pragma once

#ifndef _ELEMENT_CREATOR_H_
#define _ELEMENT_CREATOR_H_

#include "Math\Vector2.h"
#include <string>

class CGUIWindow;

class CElementCreator
{
public:
	static void CreateButton(CGUIWindow *window);
	static void CreateCheckButton(CGUIWindow *window);
	static void CreateImage(CGUIWindow *window);
	

private:
	static Vect2f GetPositionToAdd(Vect2i screen);
	static std::string GetSufixNumber(CGUIWindow *window);

	static void CleanSelections(CGUIWindow *window);
};

#endif