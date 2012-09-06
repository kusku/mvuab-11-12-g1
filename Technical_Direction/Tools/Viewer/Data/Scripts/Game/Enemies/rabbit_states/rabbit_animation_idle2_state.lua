class 'CRabbitAnimatedIdle2State' (CState)
	function CRabbitAnimatedIdle2State:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedIdle2State:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedIdle2State:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle2")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedIdle2State:Execute(_CCharacter, _elapsed_time)
		 -- print_logger(0, "CRabbitAnimatedIdle2State:Execute")
	end
	
	function CRabbitAnimatedIdle2State:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedIdle2State:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle2")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedIdle2State:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedIdle2State:OnMessage")
		return false
	end
	
	function CRabbitAnimatedIdle2State:__Finalize()
	
	end