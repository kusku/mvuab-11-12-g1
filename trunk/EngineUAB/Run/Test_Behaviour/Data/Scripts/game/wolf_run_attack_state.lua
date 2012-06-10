class 'CWolfRunAttackState' (CState)
	function CWolfRunAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfRunAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfRunAttackState:Enter")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfRunAttackState:Execute(_CCharacter)
		--print_logger(0, "CWolfRunAttackState:Execute")
	end
	
	function CWolfRunAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfRunAttackState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfRunAttackState:OnMessage(_CCharacter)
		print_logger(0, "CWolfRunAttackState:OnMessage")
	end
	
	function CWolfRunAttackState:__Finalize()
	
	end
