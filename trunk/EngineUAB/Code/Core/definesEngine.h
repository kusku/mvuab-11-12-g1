#ifndef _ENGINE_DEFINES_H
#define _ENGINE_DEFINES_H

#include "Math\Color.h"
#include "Math\Vector2.h"
#include <string>
#include <vector>

struct SConfig
{
	bool fullscreen;
	Vect2i position;
	Vect2i resolution;
	CColor color_debug;
	CColor color_release;

	std::string default_language;
	std::string fonts_path;
	std::vector<std::string> languages_path;
};

#endif