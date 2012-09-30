loadfile "utils.lua"

class 'CCombatZone3Trigger' (CBoxTrigger)
	function CCombatZone3Trigger:__init()
		CBoxTrigger.__init(self)

		--self.action_time = CActionStateCallback(0,1)
		
	end
	
	function CCombatZone3Trigger:update(elapsed_time)
		--print_logger(0, "CCombatZone3Trigger:update(elapsed_time), ")
		--print_logger(0, "hit")
		self.elapsed_time = elapsed_time
	end
	
	function CCombatZone3Trigger:on_enter()
		--local t = { 'enemy22', 'enemy23', 'enemy24' }

		
		get_game_process():get_character_manager():get_enemy_by_name('enemy22').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy23').enable = true
		get_game_process():get_character_manager():get_enemy_by_name('enemy24').enable = true
		
		self.active = false
	end
	
	function CCombatZone3Trigger:on_stay()
	end
	
	function CCombatZone3Trigger:on_exit()
	end