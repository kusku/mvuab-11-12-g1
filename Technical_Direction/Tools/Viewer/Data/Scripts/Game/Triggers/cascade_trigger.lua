class 'CCascadeTrigger' (CBoxTrigger)
	function CCascadeTrigger:__init()
		CBoxTrigger.__init(self)
	end
	
	function CCascadeTrigger:update(elapsed_time)
	end
	
	function CCascadeTrigger:on_enter()
		get_game_process():get_character_manager():get_enemy_by_name('enemy5').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy6').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy7').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy8').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy9').enable = true
		
		self.active = false
	end
	
	function CCascadeTrigger:on_stay()
	end
	
	function CCascadeTrigger:on_exit()
	end