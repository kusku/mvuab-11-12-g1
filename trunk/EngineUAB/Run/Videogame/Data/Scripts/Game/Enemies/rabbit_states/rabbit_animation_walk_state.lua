class 'CRabbitAnimatedWalkState' (CState)
	function CRabbitAnimatedWalkState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedWalkState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedWalkState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("walk")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedWalkState:Execute(_CCharacter, _elapsed_time)
		 -- print_logger(0, "CRabbitAnimatedWalkState:Execute")
	end
	
	function CRabbitAnimatedWalkState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedWalkState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("walk")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedWalkState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedWalkState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedWalkState:__Finalize()
	
	end