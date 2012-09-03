class 'CRabbitIdleState' (CState)
	function CRabbitIdleState:__init(name) 
		CState.__init(self, name)
		-- print_logger(0, "Inicio del estado IDLE del Rabbit")
	end

	function CRabbitIdleState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitIdleState:Enter")
		-- if not ( _CCharacter == nil ) then
			-- num = _CCharacter:get_animation_id("idle")
			-- _CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		-- end
		-- if core:is_debug_mode() then
			-- core:get_debug_gui_manager().debug_render:set_state_name("idle")
		-- end
		
		-- self.gfsm = _CCharacter.graphic_fsm 
		-- self.gfsm:change_state(_CCharacter.animation_idle_state)
		
		self.action_time = 0
		self.total_animation_time = 0
	end
	
	function CRabbitIdleState:Execute(_CCharacter, _elapsed_time)
		-- print_logger(0, "CRabbitIdleState:Execute")
		
		-- local l_distance = get_distance_to_player(_CCharacter, _CCharacter.player)
		-- print_logger (1, "Distancia al player: "..l_distance)
		
		-- print_logger(0 , "Posici�n :".._CCharacter.steering_entity.position.x.." ".._CCharacter.steering_entity.position.y.." ".._CCharacter.steering_entity.position.z)
		-- print_logger(0 , "Antiga Posici�n :".._CCharacter.steering_entity.previous_position.x.." ".._CCharacter.steering_entity.previous_position.y.." ".._CCharacter.steering_entity.previous_position.z)
			
		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			-- Is Attacable --
			-- if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				-- --_CCharacter.graphic_fsm:change_state(_CCharacter.attack_state)
				
			-- -- Pursuit --
			-- else
				-- player no atacable i lo perseguimos pq lo hemos detectado
				-- print_logger(1, "CRabbitIdleState:Execute")
				-- print_logger(0, "_CCharacter.properties.detection_distance : " .._CCharacter.properties.detection_distance )
				-- print_logger(0, "_CCharacter.properties.chase_distance : " .._CCharacter.properties.chase_distance )
				
				
				-- 1) Caso en que est� lej�simo. Seguramente debe hacer un rodeo y usar los waypoints
				-- if ( ( l_distance > _CCharacter.properties.chase_distance ) and ( l_distance <= _CCharacter.properties.path_follow_distance ) ) then
					-- _CCharacter.logic_fsm:change_state(_CCharacter.path_follow_state)
					-- print_logger (1, "ara molt lluny")
				-- 2) Caso en que est� cerca y lo detecto pero no demasiado
				-- if ( ( l_distance <= _CCharacter.properties.detection_distance ) and ( l_distance > _CCharacter.properties.chase_distance ) ) then
					_CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animation_run_state)
					
					-------------------------------
					-- TEST de comportamientos

					-------------------------------
					-- _CCharacter.behaviors.pursuit.target = _CCharacter.player.position
					-- _CCharacter.behaviors.pursuit:update_evader_entity( _CCharacter.player.steering_entity )
					-- _CCharacter.behaviors.seek.target = _CCharacter.player.position
					
					-- _CCharacter.behaviors:separation_on()
					-- _CCharacter.behaviors:alignment_on()
					-- _CCharacter.behaviors:cohesion_on()
					-- _CCharacter.behaviors:pursuit_on()
					-- _CCharacter.behaviors:seek_on()
					-- _CCharacter.behaviors:collision_avoidance_on()
					-- _CCharacter.behaviors:flee_on()
					-- _CCharacter.behaviors:arrive_on()
					-- _CCharacter.behaviors:evade_on()
					-- _CCharacter.behaviors.evade.target	= _CCharacter.player.position
					-- _CCharacter.behaviors.evade:update_pursuer_entity(_CCharacter.player.steering_entity, 1)
					-- _CCharacter.behaviors:wander_on()
					-- _CCharacter:face_to( _CCharacter.steering_entity.position, _elapsed_time )
					-- _CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
					-------------------------------
					
					
					-- print_logger (1, "ara not tant lluny")
				-- 3) Caso en que est� cerquisimo y voy a por �l
				-- elseif ( l_distance <= _CCharacter.properties.chase_distance ) then
					-- _CCharacter.logic_fsm:change_state(_CCharacter.seek_state)
					-- print_logger (1, "ara molt aprop")
				
				-- end 
			-- end
		-- No detecto player --> no hago nada o patrullo. TODO!!
		else
			if ( self.action_time >= self.total_animation_time ) then
				-- pillamos la animaci�n idle que deseemos
				if ( self:GetRandomAnimationName() == 1 ) then
					_CCharacter.graphic_fsm:change_state(_CCharacter.animation_idle_state)
				else
					_CCharacter.graphic_fsm:change_state(_CCharacter.animation_idle2_state)
				end
				
				-- pillamos un tiempo de ejecuci�n aleatorio
				self.total_animation_time = self:GetRandomAnimationTime()
				self.action_time = 0 
			else 
				-- self.action_time = self.action_time + _elapsed_time
				self.action_time = self.action_time + _elapsed_time
			end 
			
			_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
			_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
		end
	end
	
	function CRabbitIdleState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitIdleState:Exit")
		-- if not ( _CCharacter == nil ) then
			-- num = _CCharacter:get_animation_id("idle")
			-- _CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		-- end
	end
	
	function CRabbitIdleState:OnMessage(_CCharacter, _Msg)
		-- print_logger(0, "CRabbitIdleState:OnMessage")	
		if ( _Msg.msg == msg_attack ) then
			_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			return true
		end
		return false
	end
	
	function CRabbitIdleState:__Finalize()
	
	end
	
	function CRabbitIdleState:GetRandomAnimationName()
		return math.random(1, 2) 
	end 
	
	function CRabbitIdleState:GetRandomAnimationTime()
		return math.random(1, 2) 
	end 