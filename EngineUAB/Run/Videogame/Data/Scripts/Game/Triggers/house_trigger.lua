class 'CHouseTrigger' (CBoxTrigger)
	function CHouseTrigger:__init()
		CBoxTrigger.__init(self)
	end
	
	function CHouseTrigger:update(elapsed_time)
	end
	
	function CHouseTrigger:on_enter()
		get_game_process():get_character_manager():get_enemy_by_name('enemy18').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy19').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy20').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy21').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy1').enable = true
		
		self.active = false
	end
	
	function CHouseTrigger:on_execute()
	end
	
	function CHouseTrigger:on_exit()
	end