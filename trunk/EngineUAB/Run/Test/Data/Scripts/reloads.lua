function reload_all()
	local core = CSingleton_CCore.get_singleton()
	core:reload_all()
end

function reload_fonts()
	local core = CSingleton_CCore.get_singleton()
	core:reload_fonts()
end

function reload_languages()
	local core = CSingleton_CCore.get_singleton()
	core:reload_languages()
end

function reload_inputs()
	local core = CSingleton_CCore.get_singleton()
	core:reload_inputs()
end

function reload_render_commands()
	local core = CSingleton_CCore.get_singleton()
	core:reload_render_commands()
end

function reload_renderable_objects_layers()
	local core = CSingleton_CCore.get_singleton()
	core:reload_renderable_objects_layers()
end

function reload_meshes()
	local core = CSingleton_CCore.get_singleton()
	core:reload_meshes()
end

function reload_effects()
	local core = CSingleton_CCore.get_singleton()
	core:reload_effects()
end

function reload_pools()
	local core = CSingleton_CCore.get_singleton()
	core:reload_pools()
end

function reload_scripts()
	local core = CSingleton_CCore.get_singleton()
	core:reload_scripts()
end