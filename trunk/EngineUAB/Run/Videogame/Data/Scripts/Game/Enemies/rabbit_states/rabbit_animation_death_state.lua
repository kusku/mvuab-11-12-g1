class 'CRabbitAnimatedDeathState' (CState)
	function CRabbitAnimatedDeathState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedDeathState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedDeathState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("death")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedDeathState:Execute(_CCharacter)
		 -- print_logger(0, "CRabbitAnimatedDeathState:Execute")
	end
	
	function CRabbitAnimatedDeathState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedDeathState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("death")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedDeathState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedDeathState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedDeathState:__Finalize()
	
	end