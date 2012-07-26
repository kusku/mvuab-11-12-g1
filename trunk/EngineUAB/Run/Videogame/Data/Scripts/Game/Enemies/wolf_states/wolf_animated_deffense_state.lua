class 'CWolfAnimatedDeffenseState' (CState)
	function CWolfAnimatedDeffenseState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfAnimatedDeffenseState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedDeffenseState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("deffense")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedDeffenseState:Execute(_CCharacter)
		--print_logger(0, "CWolfAnimatedDeffenseState:Execute")
	end
	
	function CWolfAnimatedDeffenseState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedDeffenseState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("deffense")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedDeffenseState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAnimatedDeffenseState:OnMessage")
		return false
	end
	
	function CWolfAnimatedDeffenseState:__Finalize()
	
	end
	