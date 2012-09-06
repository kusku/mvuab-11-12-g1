class 'CWolfAnimatedDefenseState' (CState)
	function CWolfAnimatedDefenseState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfAnimatedDefenseState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedDefenseState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("deffense")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedDefenseState:Execute(_CCharacter, _elapsed_time)
		--print_logger(0, "CWolfAnimatedDefenseState:Execute")
	end
	
	function CWolfAnimatedDefenseState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedDefenseState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("deffense")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedDefenseState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAnimatedDefenseState:OnMessage")
		return false
	end
	
	function CWolfAnimatedDefenseState:__Finalize()
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("defense")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	