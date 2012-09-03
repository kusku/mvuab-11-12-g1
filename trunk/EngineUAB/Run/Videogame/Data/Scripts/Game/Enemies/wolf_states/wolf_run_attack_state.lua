class 'CWolfRunAttackState' (CState)
	function CWolfRunAttackState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfRunAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfRunAttackState:Enter")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfRunAttackState:Execute(_CCharacter, _elapsed_time)
		--print_logger(0, "CWolfRunAttackState:Execute")
	end
	
	function CWolfRunAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfRunAttackState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfRunAttackState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfRunAttackState:OnMessage")
		return false
	end
	
	function CWolfRunAttackState:__Finalize()
	
	end
