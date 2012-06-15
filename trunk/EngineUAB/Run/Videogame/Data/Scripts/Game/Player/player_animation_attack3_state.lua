class 'CPlayerAnimationAttack3State' (CState)
	function CPlayerAnimationAttack3State:__init() 
		CState.__init(self)
	end

	function CPlayerAnimationAttack3State:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack3")
			_CCharacter:get_animation_model():execute_action( num, 0.3 )
		end
	end
	
	function CPlayerAnimationAttack3State:Execute(_CCharacter)
	end
	
	function CPlayerAnimationAttack3State:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack3")
			--_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CPlayerAnimationAttack3State:OnMessage(_CCharacter, _Msg)
	end
	
	function CPlayerAnimationAttack3State:__Finalize()
	end