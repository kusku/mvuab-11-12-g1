class 'CRabbitPreparedToAttackState' (CState)
	function CRabbitPreparedToAttackState:__init(name) 
		CState.__init(self, name)
		self.type = "CRabbitPreparedToAttackState"
		print_logger(0, "Inicio del estado de preparación para atacar del rabbit")
		
		-- self.is_already_in_angle = false
	end

	function CRabbitPreparedToAttackState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitPreparedToAttackState:Enter")
		-- if not ( _CCharacter == nil ) then
			-- self.active_animation_name = _CCharacter:get_animation_id("walk")
			-- --_CCharacter.active_animation_name = self:GetRandomAnimationName()
			-- -- print_logger(0, "CRabbitPreparedToAttackState:Enter->Num animacion "..self.active_animation_name)
		-- end
		
		self.action_started = false
		
		-- Me permite asegurarme que ya he estado en angulo hace un momento. De esta manera puedo controlar la primera vez que el enemigo se coloca en posición
		-- hago la pausa para luego atacar
		
		-- -_CCharacter.attack_animation_time = 0.0
		_CCharacter.behaviors:separation_on()
		_CCharacter.behaviors:collision_avoidance_on()
		_CCharacter.behaviors:obstacle_wall_avoidance_on()
	end
	
	function CRabbitPreparedToAttackState:Execute(_CCharacter)
		-- print_logger(0, "CRabbitPreparedToAttackState:Execute")
		
		-- -- 1) Caso en que ataco al player. Si está focalizado y suficientemente cerca de atacar lo hace independientemente del angulo de visión del player
		if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
			print_logger(0, "CRabbitPreparedToAttackState:Execute-> is_player_attackable")
			_CCharacter.behaviors:pursuit_off()
			_CCharacter.behaviors:seek_off()
			_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
			_CCharacter.hits_done = 2		-- Esto permite hacer una pausa al entrar en el estado de ataque antes de atacar por obligar estar fatigado y permitir ver al player qué va a hacer el enemigo
			_CCharacter.logic_fsm:change_state(_CCharacter.attack_state)
			
		-- 2) Si el player NO es atacable pero casi nos aproximamos. Buscamos el hueco que no col·lisione con nada.
		elseif ( is_enemy_prepared_to_attack ( _CCharacter, _CCharacter.player ) ) then
			print_logger(0, "CRabbitPreparedToAttackState:Execute-> is_enemy_prepared_to_attack ")
			-- Si el player puede atacar porque és uno de los más cercanos pero aun no és el elegido
			if ( _CCharacter.ready_to_attack ) then 
				-- Este enemigo puede atacar. Ahora miro si está dentro del angulo de vision pero no es el elegido para atacar. Por tanto, vamos hacia el player para tener opciones de ser
				-- el elegido para atacar
				local l_angle = 12			--math.pi/15		-- 12 graus de fustrum
				go_in_to_fustrum(_CCharacter, l_angle)
				
				--local enemy = :is_enemy_visible_in_angle(_enemy.properties.prepared_attack_distance, math.pi/6)
				-- print_logger(0, "CRabbitPreparedToAttackState:Execute-> Character is ready: ".._CCharacter.name)
				
				-- -- 1) Hacemos saltos laterales 
				-- -- _CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
			
				-- -- 2) movemos un poco Wander...
				-- --_CCharacter.logic_fsm:change_state(_CCharacter.lateral_pursuit_state)
				
				-- -- if ( self.action_started ) then
					-- -- if ( self.action_time >= self.total_action_time ) then
						-- -- self.action_started = false
						-- -- _CCharacter.behaviours:seek_off()
						-- -- _CCharacter.behaviours:wander_off()
					-- -- else 
						-- -- self.action_time = self.action_time + _CCharacter.elapsed_time
					-- -- end 				
				-- -- else 
					-- -- _CCharacter.steering_entity.velocity = Vect3f(0,0,0)
					
					-- -- -- Tiempo de uso que lleva moviendose en esa dirección
					-- -- self.action_time = 0.0
				
					-- -- if ( self.begin_moving ) then 
					-- -- -- Miro el tiempo total
					-- -- math.randomseed(360)
					-- -- self.total_action_time = math.random(1)
					-- -- print_logger(1, "self.total_action_time : "..self.total_action_time)
				
					-- -- -- Obtengo la nueva dirección a moverme
					-- -- --self.new_rotation = math.random()
					-- -- -- self.new_rotation =
					-- -- print_logger(1, "rotació : "..self.new_rotation)
					
					-- -- self.new_direction = _CCharacter.player.position -_CCharacter.position 
					-- -- self.new_direction:normalize(1.0)
					-- -- self.new_direction:rotate_y(self.new_rotation)
					-- -- self.new_direction = self.new_direction + _CCharacter.properties.min_speed * _CCharacter.elapsed_time
					
					-- -- -- actualizamos la posición
					-- -- self.new_position = _CCharacter.position + self.new_direction * _CCharacter.elapsed_time 
					-- -- -- print_logger(1, " l_pos.x : "..self.new_position.x.." l_pos.y : "..self.new_position.y.." l_pos.z : "..self.new_position.z)
				
					-- -- self.new_position = _CCharacter.steering_entity:get_final_position_to_throwRay(self.new_rotation)
					
					-- -- -- Colocamos la dirección a ir en el seek i lo activamos
					-- -- _CCharacter.behaviours.seek.target = self.new_position
					-- -- _CCharacter.behaviours:seek_on()
					-- -- -- _CCharacter.behaviours.wander.target = self.new_position
					-- -- -- _CCharacter.behaviours:wander_on()
					
					
					-- -- -- Flag que indica que nos empezamos a mover en esa dirección
					-- -- self.action_started = true
					
					-- -- -- Metemos la animación pertinente
					self.active_animation_name = _CCharacter:get_animation_id("idle")
					_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
					self.active_animation_name = _CCharacter:get_animation_id("walk")
					_CCharacter:get_animation_model():blend_cycle( self.active_animation_name, 0.3 )
				-- -- end 			
				
			
			-- -- -- Si el enemigo no está listo para atacar ya que està más lejos que los que deben atacar. Reseteamos velocidad y encaramos al player
			else
				print_logger(0, "CRabbitPreparedToAttackState:Execute-> Character is NOT ready: ".._CCharacter.name)
				self.active_animation_name = _CCharacter:get_animation_id("walk")
				_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
				self.active_animation_name = _CCharacter:get_animation_id("idle")
				_CCharacter:get_animation_model():blend_cycle( self.active_animation_name, 0.3 )
				_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
				_CCharacter:face_to( _CCharacter.player.position, _CCharacter.elapsed_time )
				-- _CCharacter.behaviours.wander.target = _CCharacter.player.position
				-- _CCharacter.behaviours:wander_on()
				-- _CCharacter.behaviours.seek.target = _CCharacter.player.position
				-- _CCharacter.behaviours:seek_on()
				_CCharacter.behaviours:seek_off()
				_CCharacter.behaviours:pursuit_off()
				
				_CCharacter:face_to( _CCharacter.behaviours.seek.target, _CCharacter.elapsed_time )
				_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _CCharacter.elapsed_time )
			end

		-- Podriamos tratar otra distancia de aproximacion
		-- elseif ( is_player_inside_aproximation_distance ) then 
		-- _CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
			
		-- Si no es atacable ni se prepara volvemos a perseguirlo
		else 
			-- print_logger(0, "CRabbitPreparedToAttackState:Execute-> Vuelvo a perseguir...")
			_CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
		end 
	end -- End Function
	
	function CRabbitPreparedToAttackState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitPreparedToAttackState:Exit")
		-- if not ( _CCharacter == nil ) then
			-- num = _CCharacter:get_animation_id(self.active_animation_name)
			-- _CCharacter:get_animation_model():clear_cycle( num, 0.7 )
		-- end
		self.active_animation_name = _CCharacter:get_animation_id("walk")
		_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
		self.active_animation_name = _CCharacter:get_animation_id("idle")
		_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
				
		_CCharacter.behaviors:separation_off()
		_CCharacter.behaviors:collision_avoidance_off()
		_CCharacter.behaviors:obstacle_wall_avoidance_off()
	end
	
	function CRabbitPreparedToAttackState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CRabbitPreparedToAttackState:OnMessage-> Missatge rebut: ".._Msg.msg)
		-- if ( _Msg.msg == msg_attack ) then
			-- -- Solo me pongo en hit si realmente he finalizado el estado de ataqueç
			-- print_logger ( 1, "ARA REBO!" )
			-- if ( self.attack_animation_time < _CCharacter.animated_model:get_current_animation_duration(self.active_animation_name) ) then 
				-- _CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			-- else
				-- _CCharacter:rest_life(  _CCharacter.player.properties.strong )
			-- end
			-- return true
		-- end
		return false
	end

	function CRabbitPreparedToAttackState:__Finalize()
	
	end

	-- function CRabbitPreparedToAttackState:GetRandomAnimationName()
		-- local AnimationType = math.random(1, 2) 
		-- if ( AnimationType == 1 ) then
			-- return "jump_left"
		-- else
			-- return "jump_right"
		-- end
	
	-- end 
	
	-- --------------------------------------------------------------------------------------------------------------------
	--	GetRandomJumpName : Devuelve la dirección donde saltamos de las 8 posibles (igual que las coordenadas de un reloj)
	-- --------------------------------------------------------------------------------------------------------------------
	function CRabbitPreparedToAttackState:GetRandomDirection()
		-- 8 direcciones de saltos posibles
		local DirectionType = math.random(1,8) 
		if ( DirectionType == 1 ) then
			return "foward"
		elseif ( DirectionType == 2 ) then
			return "foward-right"
		elseif ( DirectionType == 3 ) then
			return "right"
		elseif ( DirectionType == 4 ) then
			return "back-right"
		elseif ( DirectionType == 5 ) then
			return "back"
		elseif ( DirectionType == 6 ) then
			return "back-left"
		elseif ( DirectionType == 7 ) then
			return "left"
		elseif ( DirectionType == 8 ) then
			return "foward-left"
		end
	
	end 