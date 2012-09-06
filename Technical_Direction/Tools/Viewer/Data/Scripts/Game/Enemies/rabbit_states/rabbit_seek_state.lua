class 'CRabbitSeekState' (CState)
	function CRabbitSeekState:__init(name) 
		CState.__init(self, name)
		-- print_logger(0, "CRabbitSeekState:__init->Inicio del estado SEEK de la caperucita")
	end
	
	function CRabbitSeekState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitSeekState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
		_CCharacter.behaviors:seek_on()
	end
	
	function CRabbitSeekState:Execute(_CCharacter, _elapsed_time)
		--print_logger(0, "CRabbitSeekState:Execute")
		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			-- -- 1) Is in a distance to prepared and Attacable -- Aquí deberiamos detectar si existen colisiones y movernos lateralmente en caso que ya no esté en modo ataque
			if ( is_enemy_prepared_to_attack ( _CCharacter, _CCharacter.player ) ) then
				-- print_logger (0, "CRabbitPursuitState:Execute()-> is_player_inside_prepared_distance ".._CCharacter.name)
				_CCharacter.behaviors:pursuit_off()
				_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
				_CCharacter.logic_fsm:change_state(_CCharacter.prepared_to_attack_state)
				
			-- -- -- 2) Esta distancia és la más lejana para el ataque. Podriamos meter disparos de proyectiles de los enemigos. Ataques lejanos o preparación de acernamiento
			-- -- -- 	  al player para seguidamente aproximar-me ( simplemente debería reducir la distancia al player )
			-- -- --	  Cuando este esté en focus atacará. De momento no la uso y quizás vaya en el estado Pursuit!! OJO!!
			-- -- -- elseif ( is_enemy_aproximated_to_attack ( _CCharacter, _CCharacter.player ) ) then
				-- -- -- print_logger(0, "CRabbitPreparedToAttackState:Execute-> aproximation_to_attack")
				-- -- -- Rotamos al objetivo y movemos
				
				-- -- -- _CCharacter:face_to( _CCharacter.steering_entity.position, _elapsed_time )
				-- -- -- _CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
			
				-- -- -- -- En este caso atacamos, soy el foco y el player está preparado
				-- -- -- self:aproximation_to_attack(_CCharacter) 
				-- -- -- _CCharacter.behavwiours:seek_on()
		
		
		
			-- -- -- 3) En otro caso perseguimos
			else 
				-- print_logger(0, "CRabbitPursuitState:Execute-> seguim perseguint")
				_CCharacter.behaviors.seek.target = _CCharacter.player.position
					
				-- Rotamos al objetivo y movemos
				_CCharacter:face_to( _CCharacter.steering_entity.position, _elapsed_time )
				_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
			end
		else
			-- print_logger(0, "CRabbitPursuitState:Execute-> anem a idle")
			_CCharacter.logic_fsm:change_state(_CCharacter.idle_state)
		end 
	end
	
	function CRabbitSeekState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitSeekState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
		
		_CCharacter.behaviors:seek_off()
	end
	
	function CRabbitSeekState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitSeekState:OnMessage")
		if ( _Msg.msg == msg_attack ) then
			print_logger(0, "CRabbitSeekState:OnMessage->Mensaje acceptado por el Rabbit")
			-- If depend tipus d'atac... treu més o menys vida... --
			_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			return true
		end
		return false
	end
	
	function CRabbitSeekState:__Finalize()
	
	end