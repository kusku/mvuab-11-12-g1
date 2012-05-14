loadfile "utils.lua"

function load_basics()
	core:load_fonts()
	core:load_languages()
	core:load_inputs()
	core:load_effects()
	core:load_physics()
	core:load_renderable_objects_techniques()
	core:load_render_commands()
	core:load_gui()
	core:load_sounds()
	core:load_debug_gui()
end

function load_data()
	core:load_static_meshes()
	core:load_animated_models()
	core:load_renderable_objects_layers()
	core:load_lights()
	core:load_billboards()
	core:load_particles()
	core:load_triggers()
end