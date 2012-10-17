
class 'CCombatZone2Trigger' (CBoxTrigger)
	function CCombatZone2Trigger:__init()
		CBoxTrigger.__init(self)

		self.action_time = CActionStateCallback(0,0.1)
		self.action_time:init_action()
		
		self.enable_action_time = CActionStateCallback(0,0.2)
		self.enable_action_time:init_action()
		
		self.enemy_appeared = 1
		self.total_enemies  = 6 	-- Total de enemigos en la lista
		self.is_appeared = false
		self.first_time_entered = false -- Nos indica si es la primera vez que entra para evitar que vuelva a cargar enemigos y pete lua
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
					
					if (self.enemy_appeared ~= self.total_enemies ) then
						self.enemy_appeared = self.enemy_appeared + 1
						self.action_time:init_action()
						self.action_time:start_action()
					end 
				-- end
			else 
				self.enable_action_time:update(elapsed_time)
			end 
		end
		
		if ( self:are_enemies_dead() ) then 
			core:get_limit_manager():activate_group("zona2", false)
			_soundM:play_event('zona2_wall001','Stop_EFX_fire_forest_inferno')
			_soundM:play_event('zona2_wall004','Stop_EFX_fire_forest_inferno')
			_soundM:play_event('zona2_wall003','Stop_EFX_fire_forest_inferno')
			_soundM:play_event('zona2_wall002','Stop_EFX_fire_forest_inferno')
			self.active = false
		end
	end
	
	function CCombatZone2Trigger:on_enter(user_data)
		if ( self.first_time_entered == false ) then 	
		
			local process = get_game_process()
			local l_player_user_data = process:get_character_manager():get_player().physic_controller.user_data
			
			if( core:get_physics_manager():compare_user_data(user_data, l_player_user_data) ) then
				self.action_time:start_action()
				self.enemy_appeared = 1
				self.is_appeared = false
			end 

			core:get_limit_manager():activate_group("zona2", true)
			_soundM:play_event('zona2_wall001','Play_EFX_fire_forest_inferno')
			_soundM:play_event('zona2_wall004','Play_EFX_fire_forest_inferno')
			_soundM:play_event('zona2_wall003','Play_EFX_fire_forest_inferno')
			_soundM:play_event('zona2_wall002','Play_EFX_fire_forest_inferno')
			self.first_time_entered = true
		end 
	end
	
	function CCombatZone2Trigger:on_stay(user_data)
	end
	
	function CCombatZone2Trigger:on_exit(user_data)
	end
	
	function CCombatZone2Trigger:are_enemies_dead()
		local l_previous_enemies_to_die = { "enemy11", "enemy6" , "enemy7", "enemy8", "enemy10", "enemy5" }
		local l_total_enemies_to_die = 6
		
		local l_are_dead = false
		
		for i=1,l_total_enemies_to_die do 
			local l_enemy = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[i])
			if ( l_enemy:is_alive() ) then
				l_are_dead = false
				break
			else 
				l_are_dead = true
			end  	
		end 
		return l_are_dead
	end