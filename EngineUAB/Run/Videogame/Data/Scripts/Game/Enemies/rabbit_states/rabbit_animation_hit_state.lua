class 'CRabbitAnimatedHitState' (CState)
	function CRabbitAnimatedHitState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedHitState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedHitState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedHitState:Execute(_CCharacter)
		 -- print_logger(0, "CRabbitAnimatedHitState:Execute")
	end
	
	function CRabbitAnimatedHitState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedHitState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedHitState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedHitState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedHitState:__Finalize()
	
	end