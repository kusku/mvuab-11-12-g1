class 'CRabbitStillAttackState' (CState)
	function CRabbitStillAttackState:__init(name) 
		CState.__init(self, name)
		self.type = "CRabbitStillAttackState"
		print_logger(0, "Inicio del estado STILL ATTACK del rabbit")
	end

	function CRabbitStillAttackState:OnEnter(_CCharacter)
		self.action_animation_time = 0.0
		self.action_started = false
	end
	
	function CRabbitStillAttackState:Execute(_CCharacter)
		-- print_logger(0, "CRabbitStillAttackState:Execute")
		
		-- Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
		if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
			if ( self.action_started ) then
				-- Compruebo si la animación a finalizado
				if ( self.action_animation_time >= _CCharacter.animated_model:get_current_animation_duration( "attack_1" ) ) then 
					if not ( _dispatchM == nil ) then
						_dispatchM:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), _CCharacter.player:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
					else
						print_logger( 2, "CRabbitStillAttackState:Execute->El Dispatch es NULL" )
					end 
					
					-- Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
					_CCharacter.hits_done = _CCharacter.hits_done + 1

					-- Volvemos al estado anterior
					_CCharacter.logic_fsm:revert_to_previous_state()
				else 
					self.action_animation_time =  self.action_animation_time + _CCharacter.elapsed_time
				end 
			else
				if ( is_player_inside_impact_distance( _CCharacter, _CCharacter.player ) ) then
					-- print_logger( 0, "CRabbitStillAttackState:Execute-> Impactamos!" )
					_CCharacter:face_to( _CCharacter.steering_entity.position, _CCharacter.elapsed_time )
					_CCharacter:move_to2( Vect3f(0,0,0), _CCharacter.elapsed_time )
				
					self.active_animation_id = _CCharacter:get_animation_id("run")
					_CCharacter:get_animation_model():clear_cycle( self.active_animation_id, 0.3 )
					
					self.active_animation_id = _CCharacter:get_animation_id("attack_1")
					_CCharacter:get_animation_model():execute_action( self.active_animation_id, 0.3 )
					
					self.action_started = true
					self.action_animation_time = 0.0
				else 
					-- Nos acercamos 
					-- _CCharacter.behaviors:pursuit_on()
					_CCharacter.behaviors:seek_on()
					_CCharacter.behaviors.seek.target = _CCharacter.player.position
					if not ( _CCharacter == nil ) then
						-- self.active_animation_name = _CCharacter:get_animation_id("attack_1")
						-- _CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
						
						self.active_animation_name = _CCharacter:get_animation_id("run")
						_CCharacter:get_animation_model():blend_cycle( self.active_animation_name, 0.3 )
					else 
						print_logger ( 2 , "error de caracter" )
					end
				
					-- Rotamos al objetivo y movemos
					_CCharacter:face_to( _CCharacter.steering_entity.position, _CCharacter.elapsed_time )
					_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _CCharacter.elapsed_time )
				end 
			end 
		else
			-- nos volvemos
			_CCharacter.logic_fsm:revert_to_previous_state()
		end 
	end
	
	function CRabbitStillAttackState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			-- Limpiamos animaciones
			-- self.active_animation_name = _CCharacter:get_animation_id("attack_1")
			-- _CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
			
			self.active_animation_name = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
		end
		_CCharacter.behaviors:seek_off()
	end
	
	function CRabbitStillAttackState:OnMessage(_CCharacter, _Msg)
		return false
	end
	
	function CRabbitStillAttackState:__Finalize()
	
	end
