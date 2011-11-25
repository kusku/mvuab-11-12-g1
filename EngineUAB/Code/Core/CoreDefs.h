#ifndef _COREDEFS_H
#define _COREDEFS_H

#include "Math\Color.h"
#include "Math\Vector2.h"
#include <string>
#include <vector>

#define DEBUG_MODE true

struct SConfig
{
	//Screen params
	bool bFullscreen;
	Vect2i position;
	Vect2i resolution;
	CColor color_debug;
	CColor color_release;

	//Fonts & Languages params
	std::string default_language;
	std::string fonts_path;
	std::vector<std::string> languages_path;

	//Input params
	std::string input_path;
	bool bExclusiveModeInMouse;
	bool bDrawPointerMouse;
};

#endif