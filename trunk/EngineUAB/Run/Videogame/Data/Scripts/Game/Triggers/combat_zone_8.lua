
class 'CCombatZone8Trigger' (CBoxTrigger)
	function CCombatZone8Trigger:__init()
		CBoxTrigger.__init(self)

		self.action_time = CActionStateCallback(0,1)
		self.action_time:init_action()
		
		self.enable_action_time = CActionStateCallback(0,0.2)
		self.enable_action_time:init_action()
		
		self.enemy_appeared = 1
		self.total_enemies  = 1 	-- Total de enemigos en la lista
		self.show_wolf = false
	end
	
	function CCombatZone8Trigger:update(elapsed_time)
		
		if ( self.show_wolf == false ) then 
			-- print_logger(0, "CCombatZone8Trigger:update -> Actualizacion trigger zona de combate")
			return
		end 
		
		local  t = { "enemy1" }
			
		if ( self.action_time:is_action_started() ) then
			if ( self.action_time:is_action_finished() ) then
				-- Ahora incializo la parte de la aparición
				self.enable_action_time:init_action()
				self.enable_action_time:start_action()
			else 
				self.action_time:update(elapsed_time)
				
				local l_enemy = get_game_process():get_character_manager():get_enemy_by_name(t[self.enemy_appeared])
				if ( l_enemy.enable == false ) then
					l_enemy:appearance()
					l_enemy:face_to( get_game_process():get_character_manager():get_player().properties.position, elapsed_time)
					l_enemy:move_to2( l_enemy.steering_entity.velocity, elapsed_time)
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
	
	function CCombatZone8Trigger:on_enter()
		self:can_activate()
		if ( self.show_wolf ) then
			get_game_process():get_hud().set_active_wolf_bar = true
			self.action_time:start_action()
			self.enemy_appeared = 1
		end 
		
		-- print_logger( 0, "CCombatZone8Trigger:on_enter()" )
	end
	
	function CCombatZone8Trigger:on_stay()
		-- print_logger( 0, "CCombatZone8Trigger:on_stay()" )
		self:can_activate()
	end
	
	function CCombatZone8Trigger:on_exit()
	end
	
	function CCombatZone8Trigger:can_activate()
		local  l_previous_enemies_to_die = { "enemy24", "enemy25", "enemy26", "enemy27" }
		
		local l_enemy1 = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[1])
		local l_enemy2 = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[2])
		local l_enemy3 = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[3])
		local l_enemy4 = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[4])
		
		if ( l_enemy1.enable == false and l_enemy2.enable == false and l_enemy3.enable == false and l_enemy4.enable == false ) then
			self.show_wolf = true
		else 
			self.show_wolf = false
		end 
	end 