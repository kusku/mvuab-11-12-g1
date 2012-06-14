class 'CPlayerRunState' (CState)
	function CPlayerRunState:__init() 
		CState.__init(self)
	end

	function CPlayerRunState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CPlayerRunState:Execute(_CCharacter)
	end
	
	function CPlayerRunState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CPlayerRunState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CPlayerRunState:OnMessage")	
		return false
	end
	
	function CPlayerRunState:__Finalize()
	end