
class 'CCombatZone6Trigger' (CBoxTrigger)
	function CCombatZone6Trigger:__init()
		CBoxTrigger.__init(self)

		self.action_time = CActionStateCallback(0,0.1)
		self.action_time:init_action()
		
		self.enable_action_time = CActionStateCallback(0,0.2)
		self.enable_action_time:init_action()
		
		self.enemy_appeared = 1
		self.total_enemies  = 4 	-- Total de enemigos en la lista
		self.is_appeared = false
	end
	
	function CCombatZone6Trigger:update(elapsed_time)
		--print_logger(0, "CCombatZone6Trigger:update -> Actualizacion trigger zona de combate")
		
		local t = { "enemy20", "enemy21", "enemy22", "enemy23" }
		
		if ( self.action_time:is_action_started() ) then
			if ( self.action_time:is_action_finished() ) then
				-- Ahora inicializo la parte de la aparición
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
	
	function CCombatZone6Trigger:on_enter(user_data)
		local process = get_game_process()
		local l_player_user_data = process:get_character_manager():get_player().physic_controller.user_data
		
		if( core:get_physics_manager():compare_user_data(user_data, l_player_user_data) ) then
			self.action_time:start_action()
			self.enemy_appeared = 1
			self.is_appeared = false
		end 

		core:get_limit_manager():activate_group("zona6", true)
	end
	
	function CCombatZone6Trigger:on_stay(user_data)
	end
	
	function CCombatZone6Trigger:on_exit(user_data)
	end