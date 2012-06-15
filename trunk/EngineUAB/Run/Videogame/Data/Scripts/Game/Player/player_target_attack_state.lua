class 'CPlayerTargetAttackState' (CState)
	function CPlayerTargetAttackState:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerTargetAttackState:OnEnter(_CCharacter)
	end
	
	function CPlayerTargetAttackState:Execute(_CCharacter)
		
	end
	
	function CPlayerTargetAttackState:OnExit(_CCharacter)
	end
	
	function CPlayerTargetAttackState:OnMessage(_CCharacter, _Msg)
	end
	
	function CPlayerTargetAttackState:__Finalize()
	end
	