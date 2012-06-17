class 'CWolfPursuitState' (CState)
	function CWolfPursuitState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfPursuitState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfPursuitState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfPursuitState:Execute(_CCharacter)
		--print_logger(0, "CWolfPursuitState:Execute")

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
	
	function CWolfPursuitState:OnExit(_CCharacter)
		--print_logger(0, "CWolfPursuitState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfPursuitState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfPursuitState:OnMessage")
		if ( _Msg.msg == msg_attack ) then
			print_logger(0, "Missatge acceptat per el llob")
			-- If depend tipus d'atac... treu més o menys vida... --
			_CCharacter:rest_life( 1 )
			print_logger(0, "WOLF life : ".._CCharacter.properties.life)
			return true
		end
		return false
	end
	
	function CWolfPursuitState:__Finalize()
	
	end