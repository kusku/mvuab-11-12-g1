function load_game_content()
	load_data()
	core:get_gui_manager():pop_windows()
	core:get_gui_manager():push_windows('Controls')
end

function load_game_data_process()
	load_data()
	core:get_process():load_game_data()
	core:set_game_mode(true) 
	core:get_gui_manager():set_visible_pointer_mouse(true)
end

function active_game_process()
	if core:get_action_to_input():do_action('SkipWindows') then 
		core:get_process():load_game_data()
		core:set_game_mode(true) 
	end
end

function change_to_game_over_gui_process()
	core:set_game_mode(false)
	core:get_gui_manager():pop_windows()
	core:get_gui_manager():push_windows('GameOver')
end

function change_to_gui_process()
	get_game_process():change_to_gui_process()
	core:get_gui_manager():push_windows('Main')
	--core:get_gui_manager():pop_windows()
end

function change_to_end_gui_process()
	core:set_game_mode(false)
	core:get_gui_manager():pop_windows()
	core:get_gui_manager():push_windows('End')
end

function go_to_main_menu()
	core:set_game_mode(true)
	get_game_process():change_to_gui_process()
	core:get_gui_manager():pop_windows()
	core:get_gui_manager():push_windows('Main')
end

function go_to_pause_game()
	core:set_game_mode(false)
	core:get_gui_manager():push_windows('Pause')
end

function go_to_play_game()
	core:set_game_mode(true)
	core:get_gui_manager():pop_windows()
end

function close_game()
	core:get_process():exit_game()
end

function set_options_values()
	local gui = core:get_gui_manager()
	local sound = core:get_sound_manager()
	
	gui:set_state_slider('volume_slider', options.custom_generic_volume/100)
	gui:set_state_slider('music_volume_slider', options.custom_music_volume/100)
	gui:set_state_slider('effects_volume_slider', options.custom_effects_volume/100)
	gui:set_state_check_button('mouse_check_button',options.custom_mouse_inverted)
	
	sound:set_rtpc('General_Volume', options.custom_generic_volume)
	sound:set_rtpc('Music_Volume', options.custom_music_volume)
	sound:set_rtpc('Effect_Volume', options.custom_effects_volume)
	
	core:get_action_to_input().mouse_inverted = options.custom_mouse_inverted
end

function restore_options()
	local gui = core:get_gui_manager()
	
	options.custom_generic_volume 	= options.default_generic_volume
	options.custom_music_volume		= options.default_music_volume
	options.custom_effects_volume	= options.default_effects_volume
	options.custom_mouse_inverted	= options.default_mouse_inverted
	
	set_options_values()
end