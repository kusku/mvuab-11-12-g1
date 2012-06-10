class 'CWolfAnimatedStillAttackState' (CState)
	function CWolfAnimatedStillAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfAnimatedStillAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedStillAttackState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedStillAttackState:Execute(_CCharacter)
		--print_logger(0, "CWolfAnimatedStillAttackState:Execute")
	end
	
	function CWolfAnimatedStillAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedStillAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedStillAttackState:OnMessage(_CCharacter)
		print_logger(0, "CWolfAnimatedStillAttackState:OnMessage")
	end
	
	function CWolfAnimatedStillAttackState:__Finalize()
	
	end