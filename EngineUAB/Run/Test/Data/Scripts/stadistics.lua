loadfile "utils.lua"

function get_draw_calls()
	local value = core:get_stadistics():get_draw_calls()
	core:get_debug_gui_manager():get_debug_options():set_int(value)
end

function get_draw_debug_lines()
	local value = core:get_stadistics():get_draw_debug_lines()
	core:get_debug_gui_manager():get_debug_options():set_int(value)
end

function get_rendering_vertices()
	local value = core:get_stadistics():get_vertices_in_frustum()
	core:get_debug_gui_manager():get_debug_options():set_int(value)
end

function get_rendering_triangles()
	local value = core:get_stadistics():get_triangles_in_frustum()
	core:get_debug_gui_manager():get_debug_options():set_int(value)
end