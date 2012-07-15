class 'CBridgeTrigger' (CBoxTrigger)
	function CBridgeTrigger:__init()
		CBoxTrigger.__init(self)
	end
	
	function CBridgeTrigger:update(elapsed_time)
	end
	
	function CBridgeTrigger:on_enter()
		get_game_process():get_character_manager():get_enemy_by_name('enemy10').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy11').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy12').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy13').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy14').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy15').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy16').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy17').enable = true
		
		self.active = false
	end
	
	function CBridgeTrigger:on_execute()
	end
	
	function CBridgeTrigger:on_exit()
	end