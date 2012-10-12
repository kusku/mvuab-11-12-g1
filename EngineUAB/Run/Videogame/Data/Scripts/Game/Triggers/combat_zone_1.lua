-- loadfile "utils.lua"

class 'CCombatZone1Trigger' (CBoxTrigger)
	function CCombatZone1Trigger:__init()
		CBoxTrigger.__init(self)

		self.action_time = CActionStateCallback(0,0.1)
		self.action_time:init_action()
		
		self.enable_action_time = CActionStateCallback(0,0.2)
		self.enable_action_time:init_action()
		
		self.enemy_appeared = 1
		self.total_enemies  = 3 	-- Total de enemigos en la lista
		self.is_appeared = false
	end
	
	function CCombatZone1Trigger:update(elapsed_time)
		--print_logger(0, "CCombatZone1Trigger:update -> Actualizacion trigger zona de combate")
		
		local t = { "enemy2", "enemy3", "enemy4" }
		
		if ( self.action_time:is_action_started() ) then
			if ( self.action_time:is_action_finished() ) then
				-- Ahora inicializo la parte de la aparición
				self.enable_action_time:init_action()
				self.enable_action_time:start_action()
			else 
				self.action_time:update(elapsed_time)
				
				local l_enemy = get_game_process():get_character_manager():get_enemy_by_name(t[self.enemy_appeared])
				if ( l_enemy.enable == false ) then
					l_enemy:appearance()
					l_enemy:face_to( get_game_process():get_character_manager():get_player().properties.position, elapsed_time)
					l_enemy:move_to2( l_enemy.steering_entity.velocity, elapsed_time)
					-- self.enable_action_time:init_action()
					-- self.enable_action_time:start_action()
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
	
	function CCombatZone1Trigger:on_enter(user_data)
		self.action_time:start_action()
		self.enemy_appeared = 1
		
		
	end
	
	function CCombatZone1Trigger:on_stay(user_data)
	end
	
	function CCombatZone1Trigger:on_exit(user_data)
	end