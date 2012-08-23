class 'CRabbitAnimatedDefenseState' (CState)
	function CRabbitAnimatedDefenseState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedDefenseState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedDefenseState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("defense")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedDefenseState:Execute(_CCharacter)
		 -- print_logger(0, "CRabbitAnimatedDefenseState:Execute")
	end
	
	function CRabbitAnimatedDefenseState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedDefenseState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("defense")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedDefenseState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedDefenseState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedDefenseState:__Finalize()
	
	end