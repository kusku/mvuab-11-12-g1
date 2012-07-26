class 'CPlayerAnimationRunState' (CState)
	function CPlayerAnimationRunState:__init(name) 
		CState.__init(self, name)
	end

	function CPlayerAnimationRunState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CPlayerAnimationRunState:Execute(_CCharacter)
	end
	
	function CPlayerAnimationRunState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CPlayerAnimationRunState:OnMessage(_CCharacter, _Msg)
		return false
	end
	
	function CPlayerAnimationRunState:__Finalize()
	end