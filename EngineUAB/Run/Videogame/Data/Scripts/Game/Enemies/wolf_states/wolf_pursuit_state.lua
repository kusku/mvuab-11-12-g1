class 'CWolfPursuitState' (CState)
	function CWolfPursuitState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfPursuitState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfPursuitState:Enter")
		if not ( _CCharacter == nil ) then
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
		end
	end
	
	function CWolfPursuitState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfPursuitState:OnMessage")
		return false
	end
	
	function CWolfPursuitState:__Finalize()
	
	end