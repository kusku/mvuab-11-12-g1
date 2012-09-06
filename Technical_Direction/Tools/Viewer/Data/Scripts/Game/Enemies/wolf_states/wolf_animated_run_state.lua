class 'CWolfAnimatedRunState' (CState)
	function CWolfAnimatedRunState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfAnimatedRunState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedRunState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedRunState:Execute(_CCharacter, _elapsed_time)
		-- print_logger(0, "CWolfAnimatedRunState:Execute")
	end
	
	function CWolfAnimatedRunState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedRunState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedRunState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAnimatedRunState:OnMessage")
		return false
	end
	
	function CWolfAnimatedRunState:__Finalize()
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end