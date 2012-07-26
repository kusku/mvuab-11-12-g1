class 'CWolfAnimatedRunAttackState' (CState)
	function CWolfAnimatedRunAttackState:__init(name) 
		CState.__init(self, name)
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
	
	function CWolfAnimatedRunAttackState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAnimatedRunAttackState:OnMessage")
		return false
	end
	
	function CWolfAnimatedRunAttackState:__Finalize()
	
	end
