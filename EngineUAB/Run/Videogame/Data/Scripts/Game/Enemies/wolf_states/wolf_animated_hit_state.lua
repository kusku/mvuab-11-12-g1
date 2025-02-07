class 'CWolfAnimatedHitState' (CState)
	function CWolfAnimatedHitState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfAnimatedHitState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedHitState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedHitState:Execute(_CCharacter, _elapsed_time)
		--print_logger(0, "CWolfAnimatedHitState:Execute")
	end
	
	function CWolfAnimatedHitState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedHitState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedHitState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAnimatedHitState:OnMessage")
		return false
	end
	
	function CWolfAnimatedHitState:__Finalize()
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end