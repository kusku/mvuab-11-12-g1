class 'CPlayerAnimationDefenseState' (CState)
	function CPlayerAnimationDefenseState:__init() 
		CState.__init(self)
	end

	function CPlayerAnimationDefenseState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("defense")
			_CCharacter:get_animation_model():blend_cycle( num, 0.2 )
		end
	end
	
	function CPlayerAnimationDefenseState:Execute(_CCharacter)
	end
	
	function CPlayerAnimationDefenseState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("defense")
			_CCharacter:get_animation_model():clear_cycle( num, 0.2 )
		end
	end
	
	function CPlayerAnimationDefenseState:OnMessage(_CCharacter, _Msg)
		return false
	end
	
	function CPlayerAnimationDefenseState:__Finalize()
	end