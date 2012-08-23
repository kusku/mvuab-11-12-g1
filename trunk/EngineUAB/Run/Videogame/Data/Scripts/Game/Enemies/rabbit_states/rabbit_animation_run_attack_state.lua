class 'CRabbitAnimatedRunAttackState' (CState)
	function CRabbitAnimatedRunAttackState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedRunAttackState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedRunAttackState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_2")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CRabbitAnimatedRunAttackState:Execute(_CCharacter)
		 -- print_logger(0, "CRabbitAnimatedRunAttackState:Execute")
	end
	
	function CRabbitAnimatedRunAttackState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedRunAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_2")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedRunAttackState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedRunAttackState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedRunAttackState:__Finalize()
	
	end