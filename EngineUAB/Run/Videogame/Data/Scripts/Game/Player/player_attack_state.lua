class 'CPlayerAttackState' (CState)
	function CPlayerAttackState:__init() 
		CState.__init(self)
	end

	function CPlayerAttackState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack")
			_CCharacter:get_animation_model():execute_action( num, 0.3 )
		end
	end
	
	function CPlayerAttackState:Execute(_CCharacter)
	end
	
	function CPlayerAttackState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack")
			_CCharacter:get_animation_model():clear_cycle( num, 0.1 )
		end
	end
	
	function CPlayerAttackState:OnMessage(_CCharacter)
	end
	
	function CPlayerAttackState:__Finalize()
	end