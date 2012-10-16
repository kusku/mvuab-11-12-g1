
class 'CCombatZone8Trigger' (CBoxTrigger)
	function CCombatZone8Trigger:__init()
		CBoxTrigger.__init(self)

		self.action_time = CActionStateCallback(0,0.1)
		self.action_time:init_action()
		
		self.enable_action_time = CActionStateCallback(0,0.5)
		self.enable_action_time:init_action()
		
		self.enemy_appeared = 1
		self.total_enemies  = 1 	-- Total de enemigos en la lista
		self.show_wolf = false
		self.is_appeared = false
	end
	
	function CCombatZone8Trigger:update(elapsed_time)
		
		if ( self.show_wolf == false ) then 
			return
		end 
		
		local  t = { "enemy1" }
			
		if ( self.action_time:is_action_started() ) then
			if ( self.action_time:is_action_finished() ) then
				-- Ahora inicializo la parte de la aparición
				self.enable_action_time:init_action()
				self.enable_action_time:start_action()
				self.is_appeared = false
			else 
				self.action_time:update(elapsed_time)
				
				local l_enemy = get_game_process():get_character_manager():get_enemy_by_name(t[self.enemy_appeared])
				if ( l_enemy.enable == false and self.is_appeared == false ) then
					l_enemy:appearance()
					l_enemy:face_to( get_game_process():get_character_manager():get_player().properties.position, elapsed_time)
					l_enemy:move_to2( l_enemy.steering_entity.velocity, elapsed_time)
					self.is_appeared = true
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
	
	function CCombatZone8Trigger:on_enter(user_data)
		local process = get_game_process()
		local l_player_user_data = process:get_character_manager():get_player().physic_controller.user_data
		
		if( core:get_physics_manager():compare_user_data(user_data, l_player_user_data) ) then
			self:can_activate(false)
			if ( self.show_wolf ) then
				get_game_process():get_hud().set_active_wolf_bar = true
				self.action_time:start_action()
				self.enemy_appeared = 1
				self.is_appeared = false
			end 
		end 

		core:get_limit_manager():activate_group("puente_final", true)
	end
	
	function CCombatZone8Trigger:on_stay(user_data)
		-- print_logger( 0, "CCombatZone8Trigger:on_stay()" )
		
		-- local process = get_game_process()
		-- local l_player_user_data = process:get_character_manager():get_player().physic_controller.user_data
		
		-- if( core:get_physics_manager():compare_user_data(user_data, l_player_user_data) ) then
			-- -- El parámetro me permite cambiar esto a si es para testear/debugar o no, así no es necesario matar a los enemigos para que salga el lobo
			-- self:can_activate(true)
			-- if ( self.show_wolf == true ) then
				-- print_logger( 0, "Activamos el lobo" )
				-- get_game_process():get_hud().set_active_wolf_bar = true
				-- self.action_time:start_action()
				-- self.enemy_appeared = 1
				-- self.is_appeared = false
			-- else 
				-- print_logger( 0, "devuelve false?" )
			-- end 
		-- end 
	end
	
	function CCombatZone8Trigger:on_exit(user_data)
	end
	
	function CCombatZone8Trigger:can_activate(for_test)
		local  l_previous_enemies_to_die = { "enemy24", "enemy25", "enemy26", "enemy27" }
		
		if ( for_test == true ) then 
			for i=1,4 do 
				local l_enemy = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[i])
				l_can_activate = l_can_ativate and ( l_enemy.enable == false )
				if ( l_can_activate == false ) then
					print_logger( 0, "CCombatZone8Trigger:can_activate()" )
					self.show_wolf = false
					break
				else 
					self.show_wolf = true
				end 
			end 
			
			-- print_logger( 0, "CCombatZone8Trigger:on_stay()" )
			-- local l_enemy1 = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[1])
			-- local l_enemy2 = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[2])
			-- local l_enemy3 = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[3])
			-- local l_enemy4 = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[4])
			
			-- if ( l_enemy1.enable == false and l_enemy2.enable == false and l_enemy3.enable == false and l_enemy4.enable == false ) then
				-- self.show_wolf = true
			-- else 
				-- self.show_wolf = false
			-- end 
		else
			local l_can_activate = false
			for i=1,4 do 
				local l_enemy = get_game_process():get_character_manager():get_enemy_by_name(l_previous_enemies_to_die[i])
				if ( l_enemy:is_alive() ) then
					self.show_wolf = false
					break
				else 
					self.show_wolf = true
				end  	
			end 
		end
	end 
	