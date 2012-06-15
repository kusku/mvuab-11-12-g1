class 'CPlayerAnimationDefenseState' (CState)
	function CPlayerAnimationDefenseState:__init() 
		CState.__init(self)
	end

	function CPlayerAnimationDefenseState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("defense")
			_CCharacter:get_animation_model():execute_action( num, 0.3 )
		end
	end
	
	function CPlayerAnimationDefenseState:Execute(_CCharacter)
	end
	
	function CPlayerAnimationDefenseState:OnExit(_CCharacter)
	end
	
	function CPlayerAnimationDefenseState:OnMessage(_CCharacter, _Msg)
	end
	
	function CPlayerAnimationDefenseState:__Finalize()
	end