class 'CWolfAnimatedPursuitState' (CState)
	function CWolfAnimatedPursuitState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfAnimatedPursuitState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedPursuitState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedPursuitState:Execute(_CCharacter)
		-- print_logger(0, "CWolfAnimatedPursuitState:Execute")
		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			-- Si el player es atacable --
			if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				_CCharacter.graphic_fsm:change_state(_CCharacter.animation_still_attack_state)
			end
		else
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_idle_state)
		end
	end
	
	function CWolfAnimatedPursuitState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedPursuitState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedPursuitState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAnimatedPursuitState:OnMessage")
		return false
	end
	
	function CWolfAnimatedPursuitState:__Finalize()
	
	end