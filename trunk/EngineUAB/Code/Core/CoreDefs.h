#ifndef _COREDEFS_H
#define _COREDEFS_H

#include "Math\Color.h"
#include "Math\Vector2.h"
#include <string>
#include <vector>

#define DEBUG_MODE true

#define ACTION_LOGGER				"Logger"
#define ACTION_DEBUG_LOGGER			"DebugInfo"
#define ACTION_LOGGER_PAGEDOWN		"LogRender_PageDown"
#define ACTION_LOGGER_PAGEUP		"LogRender_PageUp"
#define ACTION_LOGGER_PREVLINE		"LogRender_PrevLine"
#define ACTION_LOGGER_NEXTLINE		"LogRender_NextLine"
#define ACTION_SAVE_LOG_FILE		"SaveLogsInFile"
#define ACTION_RELOAD_ALL			"ReloadAll"


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

	//Meshes & Objects params
	std::string static_meshes_path;
	std::string renderable_objects_path;
	std::string animated_models_path;

	std::string lights_path;
	std::string effects_path;

	std::string no_texture_path;

	//Scene Renderer Command params
	std::string scene_renderer_command_manager_path;

	//Renderable Object Techniques
	std::string renderable_object_techniques_path;

	//Script params
	std::string scripts_path;

	std::string modifiers_path;
	std::string debug_options_path;
};

#endif