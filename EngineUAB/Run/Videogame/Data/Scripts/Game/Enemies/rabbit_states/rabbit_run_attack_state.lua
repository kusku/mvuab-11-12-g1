class 'CRabbitRunAttackState' (CState)
	function CRabbitRunAttackState:__init(name) 
		CState.__init(self, name)
		print_logger(0, "Inicio del estado STILL ATTACK del rabbit")
	end

	function CRabbitRunAttackState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitRunAttackState:OnEnter")
		self.action_animation_time = 0.0
	
		if not ( _CCharacter == nil ) then
			self.active_animation_name = _CCharacter:get_animation_id("attack_2")
			_CCharacter:get_animation_model():execute_action( self.active_animation_name, 0 )
		else
			print_logger(2, "CRabbitRunAttackState:OnEnter-> Error en el _CCharacter, este es nil")
		end 
		
		-- Metemos más velocidad al ataque i menos massa para acelerar más 
		self.old_max_speed = _CCharacter.steering_entity.max_speed
		self.old_mass = _CCharacter.steering_entity.mass
		_CCharacter.steering_entity.max_speed = _CCharacter.steering_entity.max_speed * 100000
		_CCharacter.steering_entity.mass = 0.00500
		
		-- Activo el seek a saco a una posició en el momento de inicio de ataque
		_CCharacter.behaviors:seek_on()
		_CCharacter.behaviors.seek.target = _CCharacter.player.position
		
		-- Almacenamos la distancia actual para saber si luego nos hemos pasado
		self.distance = get_distance_to_player(_CCharacter, _CCharacter.player)
	end
	
	function CRabbitRunAttackState:Execute(_CCharacter)
		-- print_logger(0, "CRabbitRunAttackState:Execute")
			
		-- Compruebo si la animación a finalizado
		if ( self.action_animation_time >= _CCharacter.animated_model:get_current_animation_duration( "attack_2" ) ) then 
			if not ( _dispatchM == nil ) then
				_dispatchM:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), _CCharacter.player:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
				self.action_animation_time = 0.0
			else
				print_logger( 2, "CRabbitRunAttackState:Execute->El Dispatch es NULL" )
			end 
			
			-- Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
			_CCharacter.hits_done = _CCharacter.hits_done + 1

			-- Volvemos al estado anterior
			_CCharacter.behaviors:seek_off()
			_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
			_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _CCharacter.elapsed_time )
			_CCharacter.logic_fsm:revert_to_previous_state()
		else 
			self.action_animation_time =  self.action_animation_time + _CCharacter.elapsed_time 
			
			local l_distance = get_distance_to_player(_CCharacter, _CCharacter.player)
			
			-- Pararemos de correr si estamos en el momento de impacto o que la distancia al player no sea mayor que la inicial ya que indicará 
			-- que el ataque seguramente falló y así evitamos que exista un pequeño retroceso de volver hacia el player
			if ( is_player_inside_impact_distance( _CCharacter, _CCharacter.player ) or ( l_distance > self.distance ) ) then
				_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
				_CCharacter:face_to( _CCharacter.player.position, _CCharacter.elapsed_time )
			end 
			-- Rotamos al objetivo y movemos
			-- _CCharacter:face_to( self.target_position, _CCharacter.elapsed_time )
			_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _CCharacter.elapsed_time )
		end 
	end
	
	function CRabbitRunAttackState:OnExit(_CCharacter)
		-- Limpiamos animaciones
		self.active_animation_name = _CCharacter:get_animation_id("attack_2")
		_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
		
		self.active_animation_name = _CCharacter:get_animation_id("run")
		_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )

		-- Quitamos el behaviur
		_CCharacter.behaviors:seek_off()
					
		-- Restauramos la velocidad original
		_CCharacter.steering_entity.max_speed = self.old_max_speed
		_CCharacter.steering_entity.mass = self.old_mass
		
		-- print_logger(1, " MAX SPEED: ".._CCharacter.steering_entity.max_speed)
		-- print_logger(1, " MASS: ".._CCharacter.steering_entity.mass)
	end
	
	function CRabbitRunAttackState:OnMessage(_CCharacter, _Msg)
		return false
	end
	
	function CRabbitRunAttackState:__Finalize()
		
	end
