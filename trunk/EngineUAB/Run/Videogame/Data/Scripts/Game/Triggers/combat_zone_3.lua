class 'CCombatZone3Trigger' (CBoxTrigger)
	function CCombatZone3Trigger:__init()
		CBoxTrigger.__init(self)

		--ActionState = CActionStateCallback(0,1)
		
	end
	
	function CCombatZone3Trigger:update(elapsed_time)
	end
	
	function CCombatZone3Trigger:on_enter()
		get_game_process():get_character_manager():get_enemy_by_name('enemy22').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy23').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy24').enable = true
		
		self.active = false
	end
	
	function CCombatZone3Trigger:on_stay()
	end
	
	function CCombatZone3Trigger:on_exit()
	end