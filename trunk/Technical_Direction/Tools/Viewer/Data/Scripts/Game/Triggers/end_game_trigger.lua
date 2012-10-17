class 'CEndGameTrigger' (CBoxTrigger)
	function CEndGameTrigger:__init()
		CBoxTrigger.__init(self)
	end
	
	function CEndGameTrigger:update(elapsed_time)
	end
	
	function CEndGameTrigger:on_enter(user_data)
	end
	
	function CEndGameTrigger:on_stay(user_data)
		local process = get_game_process()
		print_logger(0, "before user_data")
		local l_player_user_data = process:get_character_manager():get_player().physic_controller.user_data
		print_logger(0, "user_data")
		if( core:get_physics_manager():compare_user_data(user_data, l_player_user_data) ) then
			print_logger(0, "after user_data")
			if( process:get_character_manager():get_enemy_by_name('enemy1'):is_alive() ) then
				process:get_hud():active_texture('kill_wolf_info', true)
			else
				process:get_hud():active_texture('house_info', true)
		
				if( core:get_action_to_input():do_action('PlayerAction') ) then
					change_to_end_gui_process()
				end
			end
		end
	end
	
	function CEndGameTrigger:on_exit(user_data)
		local process = get_game_process()
		local l_player_user_data = process:get_character_manager():get_player().physic_controller.user_data
		
		if( core:get_physics_manager():compare_user_data(user_data, l_player_user_data) ) then
			process:get_hud():active_texture('kill_wolf_info', false)
			process:get_hud():active_texture('house_info', false)
		end
	end