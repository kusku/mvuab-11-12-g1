class 'CRabbitPursuitState' (CState)
	function CRabbitPursuitState:__init(name) 
		CState.__init(self, name)
		print_logger(0, "CRabbitPursuitState:__init->Inicio del estado run de la caperucita")
	end

	function CRabbitPursuitState:OnEnter(_CCharacter)
		--print_logger(0, "CRabbitPursuitState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
		_CCharacter.behaviors.pursuit.target = _CCharacter.player.position
		_CCharacter.behaviors.pursuit:update_evader_entity( _CCharacter.player.steering_entity )
		_CCharacter.behaviors:pursuit_on()
		_CCharacter.behaviors:separation_on()
		_CCharacter.behaviors:collision_avoidance_on()
		_CCharacter.behaviors:obstacle_wall_avoidance_on()
	end
	
	function CRabbitPursuitState:Execute(_CCharacter)
		-- print_logger(0, "CRabbitPursuitState:Execute")
		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			-- print_logger(0, "CRabbitPursuitState:Execute-> player detected")
			-- Ahora si estoy a una distancia de ataque
			-- if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				-- print_logger(0, "CRabbitPursuitState:Execute--> Player atacable")
				-- _CCharacter.logic_fsm:change_state(_CCharacter.attack_state)
			
			-- -- 1) Is in a distance to prepared and Attacable -- Aquí deberiamos detectar si existen colisiones y movernos lateralmente en caso que ya no esté en modo ataque
			if ( is_enemy_prepared_to_attack ( _CCharacter, _CCharacter.player ) ) then
				-- print_logger (0, "CRabbitPursuitState:Execute()-> is_player_inside_prepared_distance ".._CCharacter.name)
				-- _CCharacter.behaviors:pursuit_off()
				-- _CCharacter.steering_entity.velocity = Vect3f(0,0,0)
				_CCharacter.logic_fsm:change_state(_CCharacter.prepared_to_attack_state)
						
				-- -- -- 2) Esta distancia és la más lejana para el ataque. Podriamos meter disparos de proyectiles de los enemigos. Ataques lejanos o preparación de acernamiento
				-- -- -- 	  al player para seguidamente aproximar-me ( simplemente debería reducir la distancia al player )
				-- -- --	  Cuando este esté en focus atacará. De momento no la uso y quizás vaya en el estado Pursuit!! OJO!!
				-- -- -- elseif ( is_enemy_aproximated_to_attack ( _CCharacter, _CCharacter.player ) ) then
					-- -- -- print_logger(0, "CRabbitPreparedToAttackState:Execute-> aproximation_to_attack")
					-- -- -- Rotamos al objetivo y movemos
					
					-- -- -- _CCharacter:face_to( _CCharacter.steering_entity.position, _CCharacter.elapsed_time )
					-- -- -- _CCharacter:move_to2( _CCharacter.steering_entity.velocity, _CCharacter.elapsed_time )
				
					-- -- -- -- En este caso atacamos, soy el foco y el player está preparado
					-- -- -- self:aproximation_to_attack(_CCharacter) 
					-- -- -- _CCharacter.behaviors:seek_on()
				
				
				
					-- -- -- 3) En otro caso perseguimos
			else 
				-- print_logger(0, "CRabbitPursuitState:Execute-> seguim perseguint")
				_CCharacter.behaviors.pursuit.target = _CCharacter.player.position
				_CCharacter.behaviors.pursuit:update_evader_entity( _CCharacter.player.steering_entity )
					
				-- Rotamos al objetivo y movemos
				_CCharacter:face_to( _CCharacter.steering_entity.position, _CCharacter.elapsed_time )
				_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _CCharacter.elapsed_time )
			end
		else
			-- print_logger(0, "CRabbitPursuitState:Execute-> anem a idle")
			_CCharacter.logic_fsm:change_state(_CCharacter.idle_state)
		end 
	end
	
	function CRabbitPursuitState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitPursuitState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
		
		_CCharacter.behaviors:pursuit_off()
		_CCharacter.behaviors:separation_off()
		_CCharacter.behaviors:collision_avoidance_off()
		_CCharacter.behaviors:obstacle_wall_avoidance_off()
	end
	
	function CRabbitPursuitState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitPursuitState:OnMessage")
		if ( _Msg.msg == msg_attack ) then
			print_logger(0, "CRabbitPursuitState:OnMessage->Mensaje acceptado por el Rabbit")
			-- If depend tipus d'atac... treu més o menys vida... --
			_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			return true
		end
		return false
	end
	
	function CRabbitPursuitState:__Finalize()
	
	end
	
	-- function CRabbitIdleState:MoveOnSide(_CCharacter)
		-- _CCharacter.behaviors:pursuit_off()
		-- print_logger(1, "CRabbitIdleState:MoveOnSide(_CCharacter)")
		
		-- -- print_logger(1, "self.action_time : "..self.action_time)
			
		-- if ( self.action_started == false ) then 
			-- -- print_logger(1, "rotacio "..self.rotation)
			
			-- self.new_direction = _CCharacter.player.position -_CCharacter.position 
			-- self.new_direction:normalize(1.0)
			-- self.new_direction:rotate_y(self.rotation)
			-- self.new_direction = self.new_direction + _CCharacter.properties.min_speed * _CCharacter.elapsed_time
			-- -- print_logger(1, " posicion x : "..self.new_direction.x.." posicion y : "..self.new_direction.y.." posicion z : "..self.new_direction.z)
				
			-- -- self.new_direction = vect3f(1,0,1)
			-- -- self.new_direction = _ccharacter.player.position
			
			-- -- _CCharacter.steering_entity.velocity = vect3f(0,0,0)

			-- _CCharacter.behaviors.seek.target = self.new_direction
			-- -- _CCharacter.behaviors.seek.target = Vect3f(-27.0,-30.0, -13.0)
			-- _CCharacter.behaviors:seek_on()
		
			-- -- _CCharacter.behaviors:pursuit_on()
			-- -- _CCharacter.behaviors.pursuit.target = Vect3f(-27.0,-30.0, -13.0)
			-- -- _CCharacter.behaviors.pursuit:update_evader_entity( _CCharacter.player.steering_entity )

			-- self.action_started = true
			-- if ( self.rotation == math.rad(-45) ) then 
				-- self.rotation = math.rad(45)
			-- else 
				-- self.rotation = math.rad(-45)
			-- end 
			-- self.action_time = 0.0
			-- print_logger(1, "self.action_time : "..self.action_time)

		-- else 
			-- -- print_logger(1, "self.action_time : "..self.action_time)
			-- -- if ( self.action_time < 0 ) then 
			-- -- -- self.action_time = self.action_time + _CCharacter.elapsed_time
			-- -- -- -- _CCharacter:face_to( _CCharacter.behaviors.seek.target , _CCharacter.elapsed_time )
			-- -- -- -- _CCharacter:move_to2( _CCharacter.behaviors.seek.target , _CCharacter.elapsed_time )
			-- -- else
				-- -- -- _CCharacter.behaviors:seek_off()
				-- -- -- _CCharacter.steering_entity.velocity = Vect3f(0,0,0)
				-- -- -- self.action_started = false			
				
			-- -- end 
		-- end 
	-- end 