loadfile "utils.lua"

function show_fps_state()
	local value = core:get_debug_gui_manager():get_debug_render():is_fps_visible()
	core:get_debug_options():set_bool(value)
end

function show_delta_time_state()
	local value = core:get_debug_gui_manager():get_debug_render():is_delta_time_visible()
	core:get_debug_options():set_bool(value)
end

function show_gamepad_state()
	local value = core:get_debug_gui_manager():get_debug_render():is_gamepad_visible()
	core:get_debug_options():set_bool(value)
end

function toggle_show_fps()
	core:get_debug_gui_manager():get_debug_render():toggle_fps()
end

function toggle_show_delta_time()
	core:get_debug_gui_manager():get_debug_render():toggle_delta_time()
end

function toggle_show_gamepad()
	core:get_debug_gui_manager():get_debug_render():toggle_gamepad()
end