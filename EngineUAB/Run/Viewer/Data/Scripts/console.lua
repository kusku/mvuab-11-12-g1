loadfile "utils.lua"

function toggle_console()
	core:get_debug_gui_manager():get_console():toggle()
end

function set_active_console(active)
	core:get_debug_gui_manager():get_console():set_active(active)
end