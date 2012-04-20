#pragma once

#ifndef _ELEMENT_CREATOR_H_
#define _ELEMENT_CREATOR_H_

#include "Math\Vector2.h"
#include <string>

class CGUIWindow;

class CElementCreator
{
public:
	static void CreateImage(CGUIWindow *window);
	static void CreateButton(CGUIWindow *window);

private:
	static Vect2f GetPositionToAdd(Vect2i screen);
	static std::string GetSufixNumber(CGUIWindow *window);

};

#endif