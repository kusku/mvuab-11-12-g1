class 'CEndGameTrigger' (CBoxTrigger)
	function CEndGameTrigger:__init()
		CBoxTrigger.__init(self)
	end
	
	function CEndGameTrigger:update(elapsed_time)
	end
	
	function CEndGameTrigger:on_enter()
	end
	
	function CEndGameTrigger:on_stay()
		local process = get_game_process()
	
		if( process:get_character_manager():get_enemy_by_name('enemy1'):is_alive() ) then
			process:get_hud():active_texture('kill_wolf_info', true)
		else
			process:get_hud():active_texture('house_info', true)
	
			if( core:get_action_to_input():do_action('PlayerAction') ) then
				change_to_end_gui_process()
			end
		end
	end
	
	function CEndGameTrigger:on_exit()
		get_game_process():get_hud():active_texture('kill_wolf_info', false)
		get_game_process():get_hud():active_texture('house_info', false)
	end