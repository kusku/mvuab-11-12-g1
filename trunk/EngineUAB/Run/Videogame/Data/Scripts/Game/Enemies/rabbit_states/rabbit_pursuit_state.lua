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
	end
	
	function CRabbitPursuitState:Execute(_CCharacter)
		--print_logger(0, "CRabbitPursuitState:Execute")

		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			_CCharacter:move_to( _CCharacter.player.position, _CCharacter.elapsed_time )
			
			-- Is Attacable --
			if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				_CCharacter.logic_fsm:change_state(_CCharacter.attack_state)
			end
		else
			_CCharacter.logic_fsm:change_state(_CCharacter.idle_state)
		end 
	end
	
	function CRabbitPursuitState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitPursuitState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
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