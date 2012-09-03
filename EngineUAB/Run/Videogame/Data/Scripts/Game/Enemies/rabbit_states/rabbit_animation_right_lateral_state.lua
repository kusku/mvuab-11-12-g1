class 'CRabbitAnimatedRightLateralState' (CState)
	function CRabbitAnimatedRightLateralState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedRightLateralState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedRightLateralState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("right_lateral")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedRightLateralState:Execute(_CCharacter, _elapsed_time)
		 -- print_logger(0, "CRabbitAnimatedRightLateralState:Execute")
	end
	
	function CRabbitAnimatedRightLateralState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedRightLateralState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("right_lateral")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedRightLateralState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedRightLateralState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedRightLateralState:__Finalize()
	
	end