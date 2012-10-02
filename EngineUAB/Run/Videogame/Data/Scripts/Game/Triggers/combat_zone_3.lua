-- loadfile "utils.lua"

class 'CCombatZone3Trigger' (CBoxTrigger)
	function CCombatZone3Trigger:__init()
		CBoxTrigger.__init(self)

		--self.action_time = CActionStateCallback(0,1)
		self.action_time = 0
	end
	
	function CCombatZone3Trigger:update(elapsed_time)
		--print_logger(0, "CCombatZone3Trigger:update(elapsed_time), ")
		--print_logger(0, "hit")
		self.action_time = self.action_time + elapsed_time
		
	end
	
	function CCombatZone3Trigger:on_enter()
		-- local t = { "enemy22", "enemy23", "enemy24", "enemy25", "enemy26" , "enemy27", "enemy28" }
		
		-- local i = 1
		-- for i=1,# t do
			-- get_game_process():get_character_manager():get_enemy_by_name(t[i]).enable = true
		-- end 
		
		-- if ( self.action_time <= i ) then
			-- get_game_process():get_character_manager():get_enemy_by_name(t[i]).enable = true
			-- i = i + 1
		-- end 
			
		
		
		self.active = false
	end
	
	function CCombatZone3Trigger:on_stay()
	end
	
	function CCombatZone3Trigger:on_exit()
	end