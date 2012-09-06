class 'CRabbitAnimatedJumpState' (CState)
	function CRabbitAnimatedJumpState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedJumpState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedJumpState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("jump")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedJumpState:Execute(_CCharacter, _elapsed_time)
		 -- print_logger(0, "CRabbitAnimatedJumpState:Execute")
	end
	
	function CRabbitAnimatedJumpState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedJumpState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("jump")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedJumpState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedJumpState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedJumpState:__Finalize()
	
	end