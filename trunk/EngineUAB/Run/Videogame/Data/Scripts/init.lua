loadfile "utils.lua"

function load_basics()
	core:load_fonts()
	core:load_languages()
	core:load_inputs()
	core:load_effects()
	core:load_renderable_objects_techniques()
	core:load_render_commands()
	core:load_physics()
	core:load_gui()
	core:load_sounds()
	core:load_debug_gui()
end

function load_data()
	core:load_rails()
	core:load_physxobjs()
	core:load_static_meshes()
	core:load_animated_models()
	core:load_renderable_objects_layers()
	core:load_lights()
	core:load_billboards()
	core:load_particles()
	--core:load_triggers()
	core:load_animal_movement()
	core:load_waypoints()
	core:load_steering_behaviors_settings()
	core:load_limits()
end

function load_all()
	load_basics()
	load_data()
end

function unload_data()
	core:unload_physx_objs()
	core:unload_renderable_objects_layers()
	core:unload_static_meshes()
	core:unload_animated_models()
	core:unload_lights()
	core:unload_billboards()
	core:unload_particles()
	core:unload_triggers()
	core:unload_animal_movement()
	core:Unload_waypoints()
	core:Unload_limits()
end