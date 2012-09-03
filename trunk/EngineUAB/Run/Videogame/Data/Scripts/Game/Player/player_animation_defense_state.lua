class 'CPlayerAnimationDefenseState' (CState)
	function CPlayerAnimationDefenseState:__init(name) 
		CState.__init(self, name)
	end

	function CPlayerAnimationDefenseState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("defense")
			_CCharacter:get_animation_model():blend_cycle( num, 0.2 )
		end
	end
	
	function CPlayerAnimationDefenseState:Execute(_CCharacter, _elapsed_time)
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