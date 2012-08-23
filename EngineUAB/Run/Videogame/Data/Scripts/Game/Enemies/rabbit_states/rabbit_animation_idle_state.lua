class 'CRabbitAnimatedIdleState' (CState)
	function CRabbitAnimatedIdleState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedIdleState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedIdleState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedIdleState:Execute(_CCharacter)
		 -- print_logger(0, "CRabbitAnimatedIdleState:Execute")
	end
	
	function CRabbitAnimatedIdleState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedIdleState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedIdleState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedIdleState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedIdleState:__Finalize()
	
	end