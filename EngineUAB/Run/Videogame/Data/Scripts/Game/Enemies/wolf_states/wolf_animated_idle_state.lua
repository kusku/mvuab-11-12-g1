class 'CWolfAnimatedIdleState' (CState)
	function CWolfAnimatedIdleState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfAnimatedIdleState:OnEnter(_CCharacter)
		-- print_logger(0, "CWolfAnimatedIdleState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CWolfAnimatedIdleState:Execute(_CCharacter)
		 -- print_logger(0, "CWolfAnimatedIdleState:Execute")
		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			-- Lo perseguimos --
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_run_state)
		else
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_idle_state)
		end
	end
	
	function CWolfAnimatedIdleState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedIdleState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedIdleState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAnimatedIdleState:OnMessage")	
		return false
	end
	
	function CWolfAnimatedIdleState:__Finalize()
	
	end