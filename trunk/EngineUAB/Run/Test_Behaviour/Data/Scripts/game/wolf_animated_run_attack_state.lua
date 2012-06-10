class 'CWolfAnimatedRunAttackState' (CState)
	function CWolfAnimatedRunAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfAnimatedRunAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedRunAttackState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedRunAttackState:Execute(_CCharacter)
		--print_logger(0, "CWolfAnimatedRunAttackState:Execute")
	end
	
	function CWolfAnimatedRunAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedRunAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedRunAttackState:OnMessage(_CCharacter)
		print_logger(0, "CWolfAnimatedRunAttackState:OnMessage")
	end
	
	function CWolfAnimatedRunAttackState:__Finalize()
	
	end
