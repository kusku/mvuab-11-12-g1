class 'CPlayerIdleState' (CState)
	function CPlayerIdleState:__init() 
		CState.__init(self)
	end

	function CPlayerIdleState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CPlayerIdleState:Execute(_CCharacter)
	end
	
	function CPlayerIdleState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CPlayerIdleState:OnMessage(_CCharacter)
	end
	
	function CPlayerIdleState:__Finalize()
	end