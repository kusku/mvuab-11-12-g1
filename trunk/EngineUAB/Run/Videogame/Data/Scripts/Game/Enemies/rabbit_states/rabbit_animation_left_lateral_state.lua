class 'CRabbitAnimatedLeftLateralState' (CState)
	function CRabbitAnimatedLeftLateralState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedLeftLateralState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedLeftLateralState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("left_lateral")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedLeftLateralState:Execute(_CCharacter)
		 -- print_logger(0, "CRabbitAnimatedLeftLateralState:Execute")
	end
	
	function CRabbitAnimatedLeftLateralState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedLeftLateralState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("left_lateral")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedLeftLateralState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedLeftLateralState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedLeftLateralState:__Finalize()
	
	end