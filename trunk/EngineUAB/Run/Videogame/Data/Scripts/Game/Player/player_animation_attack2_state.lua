class 'CPlayerAnimationAttack2State' (CState)
	function CPlayerAnimationAttack2State:__init(name) 
		CState.__init(self, name)
	end

	function CPlayerAnimationAttack2State:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.1 )
		
			num = _CCharacter:get_animation_id("attack2")
			_CCharacter:get_animation_model():execute_action( num, 0.1 )
		end
	end
	
	function CPlayerAnimationAttack2State:Execute(_CCharacter)
	end
	
	function CPlayerAnimationAttack2State:OnExit(_CCharacter)
		num = _CCharacter:get_animation_id("idle")
		_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
	end
	
	function CPlayerAnimationAttack2State:OnMessage(_CCharacter, _Msg)
		return false
	end
	
	function CPlayerAnimationAttack2State:__Finalize()
	end