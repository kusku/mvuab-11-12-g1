class 'CPlayerAnimationIdleState' (CState)
	function CPlayerAnimationIdleState:__init() 
		CState.__init(self)
	end

	function CPlayerAnimationIdleState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CPlayerAnimationIdleState:Execute(_CCharacter)
	end
	
	function CPlayerAnimationIdleState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CPlayerAnimationIdleState:OnMessage(_CCharacter, _Msg)
	end
	
	function CPlayerAnimationIdleState:__Finalize()
	end