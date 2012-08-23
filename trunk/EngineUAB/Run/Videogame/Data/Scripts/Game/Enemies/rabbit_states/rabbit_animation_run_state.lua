class 'CRabbitAnimatedRunState' (CState)
	function CRabbitAnimatedRunState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedRunState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedRunState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedRunState:Execute(_CCharacter)
		 -- print_logger(0, "CRabbitAnimatedRunState:Execute")
	end
	
	function CRabbitAnimatedRunState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedRunState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedRunState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedRunState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedRunState:__Finalize()
	
	end