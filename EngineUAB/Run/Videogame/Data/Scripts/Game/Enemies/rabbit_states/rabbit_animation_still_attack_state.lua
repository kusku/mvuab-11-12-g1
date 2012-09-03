class 'CRabbitAnimatedStillAttackState' (CState)
	function CRabbitAnimatedStillAttackState:__init(name) 
		CState.__init(self,name)
		-- print_logger(0, "Inicio del estado animado idle del rabbit")
	end

	function CRabbitAnimatedStillAttackState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAnimatedStillAttackState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_1")
			_CCharacter:get_animation_model():	( num, 0.3 )
		end
	end

	function CRabbitAnimatedStillAttackState:Execute(_CCharacter, _elapsed_time)
		 -- print_logger(0, "CRabbitAnimatedStillAttackState:Execute")
	end
	
	function CRabbitAnimatedStillAttackState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAnimatedStillAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_1")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CRabbitAnimatedStillAttackState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitAnimatedStillAttackState:OnMessage")
		return false
	end
	
	function CRabbitAnimatedStillAttackState:__Finalize()
	
	end