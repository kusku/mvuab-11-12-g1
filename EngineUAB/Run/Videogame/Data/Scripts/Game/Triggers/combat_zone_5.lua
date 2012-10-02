
class 'CCombatZone5Trigger' (CBoxTrigger)
	function CCombatZone5Trigger:__init()
		CBoxTrigger.__init(self)

		self.action_time = CActionStateCallback(0,1)
		self.action_time:init_action()
		
		self.enable_action_time = CActionStateCallback(0,0.5)
		self.enable_action_time:init_action()
		
		self.enemy_appeared = 1
		self.total_enemies  = 5 	-- Total de enemigos en la lista
		self.is_appeared = false
	end
	
	function CCombatZone5Trigger:update(elapsed_time)
		--print_logger(0, "CCombatZone5Trigger:update -> Actualizacion trigger zona de combate")
		
		local t = { "enemy15", "enemy16", "enemy17", "enemy18", "enemy19"}
		
		if ( self.action_time:is_action_started() ) then
			if ( self.action_time:is_action_finished() ) then
				-- Ahora incializo la parte de la aparición
				self.enable_action_time:init_action()
				self.enable_action_time:start_action()
			else 
				self.action_time:update(elapsed_time)
				
				if ( get_game_process():get_character_manager():get_enemy_by_name(t[self.enemy_appeared]).enable == false ) then
					get_game_process():get_character_manager():get_enemy_by_name(t[self.enemy_appeared]):appearance()
					self.enable_action_time:init_action()
					self.enable_action_time:start_action()
				end
			end 
		end

		if ( self.enable_action_time:is_action_started() ) then
			if ( self.enable_action_time:is_action_finished() ) then
				--if ( get_game_process():get_character_manager():get_enemy_by_name(t[self.enemy_appeared]).enable == false ) then
					get_game_process():get_character_manager():get_enemy_by_name(t[self.enemy_appeared]).enable = true
					
					if (self.enemy_appeared == self.total_enemies ) then
						self.active = false
					else
						self.enemy_appeared = self.enemy_appeared + 1
						self.action_time:init_action()
						self.action_time:start_action()
					end 
				-- end
			else 
				self.enable_action_time:update(elapsed_time)
			end 
		end
	end
	
	function CCombatZone5Trigger:on_enter()
		self.action_time:start_action()
		self.enemy_appeared = 1
	end
	
	function CCombatZone5Trigger:on_stay()
	end
	
	function CCombatZone5Trigger:on_exit()
	end