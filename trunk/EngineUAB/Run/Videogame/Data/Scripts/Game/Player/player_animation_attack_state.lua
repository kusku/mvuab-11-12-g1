class 'CPlayerAnimationAttackState' (CState)
	function CPlayerAnimationAttackState:__init(name) 
		CState.__init(self, name)
	end

	function CPlayerAnimationAttackState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.1 )
			
			num = _CCharacter:get_animation_id("attack1")
			_CCharacter:get_animation_model():execute_action( num, 0.1 )
		end
	end
	
	function CPlayerAnimationAttackState:Execute(_CCharacter, _elapsed_time)
	end
	
	function CPlayerAnimationAttackState:OnExit(_CCharacter)
		num = _CCharacter:get_animation_id("idle")
		_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
	end
	
	function CPlayerAnimationAttackState:OnMessage(_CCharacter, _Msg)
	
		return false
	end
	
	function CPlayerAnimationAttackState:__Finalize()
	end