class 'CStartTrigger' (CBoxTrigger)
	function CStartTrigger:__init()
		CBoxTrigger.__init(self)
	end
	
	function CStartTrigger:update(elapsed_time)
	end
	
	function CStartTrigger:on_enter()
		get_game_process():get_character_manager():get_enemy_by_name('enemy2').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy3').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy4').enable = true
		
		self.active = false
	end
	
	function CStartTrigger:on_stay()
	end
	
	function CStartTrigger:on_exit()
	end