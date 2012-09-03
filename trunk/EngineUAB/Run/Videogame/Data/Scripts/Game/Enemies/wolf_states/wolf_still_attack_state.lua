class 'CWolfStillAttackState' (CState)
	function CWolfStillAttackState:__init(name) 
		CState.__init(self, name)
		-- print_logger(0, "Inicio del estado STILL ATTACK de la caperucita")
	end

	function CWolfStillAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfStillAttackState:Enter")
		-- self.action_animation_time = 0.0
		-- self.action_started = false
	end
	
	function CWolfStillAttackState:Execute(_CCharacter, _elapsed_time)
		-- print_logger(0, "CWolfStillAttackState:Execute")
		
		_CCharacter.behaviors:seek_off()
		local l_player = get_game_process():get_character_manager():get_player()
		if ( is_player_attackable( _CCharacter, l_player ) ) then
			-- if ( self.action_started ) then
				-- -- print_logger(0, "CWolfStillAttackState:Execute->if ( self.action_started ) then")
			
				-- -- Compruebo si la animación a finalizado
				-- if ( self.action_animation_time >= _CCharacter.animated_model:get_current_animation_duration( "attack_1" ) ) then 
					-- if not ( _dispatchM == nil ) then
						-- _dispatchM:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), l_player:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
					-- else
						-- print_logger( 2, "CRabbitStillAttackState:Execute->El Dispatch es NULL" )
					-- end 
					
					-- -- Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
					-- _CCharacter.hits_done = _CCharacter.hits_done + 1

					-- -- Volvemos al estado anterior
					-- _CCharacter.logic_fsm:revert_to_previous_state()
					-- _CCharacter.graphic_fsm:graphic_to_previous_state()
				-- else 
					-- self.action_animation_time =  self.action_animation_time + _elapsed_time
				-- end 
			-- else
				-- if ( is_player_inside_impact_distance( _CCharacter, l_player ) ) then
					-- -- print_logger( 0, "CRabbitStillAttackState:Execute-> Impactamos!" )
					_CCharacter:face_to( _CCharacter.steering_entity.position, _elapsed_time )
					_CCharacter:move_to2( Vect3f(0,0,0), _elapsed_time )
					_CCharacter.graphic_fsm:change_state(_CCharacter.animation_still_attack_state)
					
					-- self.action_started = true
					-- self.action_animation_time = 0.0
				-- else 
					-- -- Nos acercamos 
					-- -- _CCharacter.behaviors:pursuit_on()
					-- -- print_logger(0, "CWolfStillAttackState:Execute->NOS ACERCAMOS")
					-- _CCharacter.behaviors:seek_on()
					-- _CCharacter.behaviors.seek.target = l_player.position
					-- _CCharacter.graphic_fsm:change_state(_CCharacter.animation_run_state)
					
					-- -- Rotamos al objetivo y movemos
					-- _CCharacter:face_to( _CCharacter.steering_entity.position, _elapsed_time )
					-- _CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
				-- end 
			-- end 
			-- print_logger(1,"Atacable")
		else
			-- nos volvemos
			-- _CCharacter.logic_fsm:revert_to_previous_state()
			-- _CCharacter.graphic_fsm:revert_to_previous_state()
			_CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_run_state)
			
		end
	end
	
	function CWolfStillAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfStillAttackState:Exit")
		-- if not ( _CCharacter == nil ) then
			
		-- end
	end
	
	function CWolfStillAttackState:OnMessage(_CCharacter, _Msg)
		-- print_logger(0, "CWolfStillAttackState:OnMessage")
		-- if ( _Msg.msg == msg_attack ) then
			-- _CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			-- _CCharacter.graphic_fsm:change_state(_CCharacter.animation_hit_state)
			-- return true
		-- end
		return false
	end
	
	function CWolfStillAttackState:__Finalize()
	
	end