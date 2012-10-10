
class 'CCombatZone2Trigger' (CBoxTrigger)
	function CCombatZone2Trigger:__init()
		CBoxTrigger.__init(self)

		self.action_time = CActionStateCallback(0,0.1)
		self.action_time:init_action()
		
		self.enable_action_time = CActionStateCallback(0,0.5)
		self.enable_action_time:init_action()
		
		self.enemy_appeared = 1
		self.total_enemies  = 6 	-- Total de enemigos en la lista
		self.is_appeared = false
	end
	
	function CCombatZone2Trigger:update(elapsed_time)
		local t = { "enemy11", "enemy6" , "enemy7", "enemy8", "enemy10", "enemy5" }
		
		if ( self.action_time:is_action_started() ) then
			if ( self.action_time:is_action_finished() ) then
				-- Ahora incializo la parte de la aparición
				self.enable_action_time:init_action()
				self.enable_action_time:start_action()
				self.is_appeared = false
			else 
				local l_enemy = get_game_process():get_character_manager():get_enemy_by_name(t[self.enemy_appeared])
				if ( l_enemy.enable == false and self.is_appeared == false ) then
					l_enemy:appearance()
					self.is_appeared = true
					-- self.enable_action_time:init_action()
					-- self.enable_action_time:start_action()
				end
				self.action_time:update(elapsed_time)
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
	
	function CCombatZone2Trigger:on_enter()
		self.action_time:start_action()
		self.enemy_appeared = 1
		self.is_appeared = false
	end
	
	function CCombatZone2Trigger:on_stay()
	end
	
	function CCombatZone2Trigger:on_exit()
	end