class 'CPlayerAnimationHitState' (CState)
	function CPlayerAnimationHitState:__init() 
		CState.__init(self)
	end

	function CPlayerAnimationHitState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():execute_action( num, 1.0 )
		end
	end
	
	function CPlayerAnimationHitState:Execute(_CCharacter)
	end
	
	function CPlayerAnimationHitState:OnExit(_CCharacter)
	end
	
	function CPlayerAnimationHitState:OnMessage(_CCharacter, _Msg)
		return false
	end
	
	function CPlayerAnimationHitState:__Finalize()
	end