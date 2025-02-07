#pragma once

#ifndef __COREDEFS_H__
#define __COREDEFS_H__

#include "Math\Color.h"
#include "Math\Vector2.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include <vector>

#define DEBUG_MODE true

// ********* RELOADS *********** 
#define ACTION_RELOAD_ALL					"ReloadAll"
#define ACTION_RELOAD_TTFS					"ReloadTTFs"
#define ACTION_RELOAD_LANGUAGES				"ReloadLanguageXMLs"
#define ACTION_RELOAD_ACTIONS				"ReloadActions"
#define ACTION_RELOAD_COMMANDS				"ReloadCommands"
#define ACTION_RELOAD_RENDERABLE_OBJECTS	"ReloadRenderableObjects"
#define ACTION_RELOAD_LAYERS				"ReloadLayers"
#define ACTION_RELOAD_POOLS					"ReloadPools"
#define ACTION_RELOAD_MESHES				"ReloadMeshes"
#define ACTION_RELOAD_ANIMATIONS			"ReloadAnimations"
#define ACTION_RELOAD_EFFECTS				"ReloadEffects"
#define ACTION_RELOAD_SHADERS				"ReloadShaders"
#define ACTION_RELOAD_LIGHTS				"ReloadLightsModels"
#define ACTION_RELOAD_SCRIPTS				"ReloadScripts"
#define ACTION_RELOAD_PHYSIS				"ReloadPhysis"
#define ACTION_RELOAD_GUI					"ReloadGUI"
#define ACTION_RELOAD_SOUNDS				"ReloadSounds"
#define ACTION_RELOAD_TRIGGERS				"ReloadTriggers"
#define ACTION_RELOAD_PARTICLES				"ReloadParticles"
#define ACTION_RELOAD_RAILS					"ReloadRails"



// ********* CONSOLE *********** 
#define ACTION_CONSOLE						"Console"

// ********* DEBUG GUI *********** 
#define ACTION_DEBUG_INFO					"DebugInfo"
#define ACTION_DEBUG_OPTIONS				"DebugOptions"
#define ACTION_DEBUG_NEXT_PAGE				"NextPage"
#define ACTION_DEBUG_PREVIOUS_PAGE			"PrevPage"
#define ACTION_DEBUG_NEXT_LINE				"NextLine"
#define ACTION_DEBUG_PREVIOUS_LINE			"PrevPage"
#define ACTION_DEBUG_DO_ACTION				"DoAction"

// --------- LOGGER ---------
#define ACTION_LOGGER						"Logger"
#define ACTION_LOGGER_PAGEDOWN				"LogRender_PageDown"
#define ACTION_LOGGER_PAGEUP				"LogRender_PageUp"
#define ACTION_LOGGER_PREVLINE				"LogRender_PrevLine"
#define ACTION_LOGGER_NEXTLINE				"LogRender_NextLine"
#define ACTION_SAVE_LOG_FILE				"SaveLogsInFile"

// --------- MODIFIERS ---------
#define ACTION_MODIFIERS					"Modifiers"
#define ACTION_SHOW_MODIFIERS				"ModifiersShow"
#define ACTION_PREVIOUS_MODIFIERS			"Modifier_Previous"
#define ACTION_NEXT_MODIFIERS				"Modifier_Next"
#define ACTION_GOTO_MODIFIERS				"GoToModifier"
#define ACTION_GOTO_ROOT_MODIFIERS			"GoToRootModifier"
#define ACTION_ADD_VALUE_BY_PASS_MODIFIERS	"AddValueToModifierByPass"
#define ACTION_SUBS_VALUE_BY_PASS_MODIFIERS	"SubsValueToModifierByPass"
#define ACTION_ADD_VALUE_MODIFIERS			"AddValueToModifier"
#define ACTION_SUBS_VALUE_MODIFIERS			"SubsValueToModifier"

// ********* MISC *********** 
#define ACTION_PRINT_SCREEN_TO_FILE			"SaveScreenToFile" 
#define ACTION_ACTIVATE_DEACTIVATE_TIMER	"ActiveDeactiveTimerAceleration"
#define ACTION_FINISH_RAIL_ACTIONS			"SkipRailVision"

struct SConfig
{
	//Screen params
	bool bFullscreen;
	bool bDebugMode;
	Vect2i position;
	Vect2i windowResolution;
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
	std::string scene_renderer_gui_command_manager_path;

	//Renderable Object Techniques
	std::string renderable_object_techniques_path;

	//Script params
	std::string scripts_path;

	std::string modifiers_path;
	std::string debug_options_path;

	//Billboards
	std::string billboards_path;

	//Particles
	std::string particle_emitters_path;
	std::string particle_settings_path;
	std::string particle_systems_path;
	std::string particle_emitter_systems_path;
	std::string particle_startup_instances_path;

	// Parametros del sistema gui
	std::string gui_path;

	// Parametros del sistema de triggers
	std::string triggers_system_path;

	// Parametros del Sonido
	std::string sound_banks_path;
	std::string speakers_path;

	// Parametros del WayPoints
	std::string waypoints_path;

	// Parametros del Animal Movement
	std::string animal_movement_path;

	// Parametros del Animal Movement
	std::string steering_behavior_settings_path;

	//Par�meteros de los ra�les
	std::string rails_path;

	// Parametros del PhysX Objects
	std::string physx_obj_path;

	// Parametros del PhysX Objects
	std::string limit_obj_path;

	//Parametros de Graphics
	bool motion_blur_enable;
	bool z_blur_enable;
	bool bloom_enable;
	unsigned int particle_level;
};

#endif __COREDEFS_H__