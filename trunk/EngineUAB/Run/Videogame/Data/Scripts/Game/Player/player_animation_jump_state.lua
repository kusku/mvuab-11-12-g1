class 'CPlayerAnimationJumpState' (CState)
	function CPlayerAnimationJumpState:__init() 
		CState.__init(self)
	end

	function CPlayerAnimationJumpState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("jump")
			_CCharacter:get_animation_model():execute_action( num, 1.0 )
		end
	end
	
	function CPlayerAnimationJumpState:Execute(_CCharacter)
	end
	
	function CPlayerAnimationJumpState:OnExit(_CCharacter)
	end
	
	function CPlayerAnimationJumpState:OnMessage(_CCharacter, _Msg)
		return false
	end
	
	function CPlayerAnimationJumpState:__Finalize()
	end