class 'CWolfStillAttackState' (CState)
	function CWolfStillAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfStillAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfStillAttackState:Enter")
		if not ( _CCharacter == nil ) then
			
		end
	end
	
	function CWolfStillAttackState:Execute(_CCharacter)
		--print_logger(0, "CWolfStillAttackState:Execute")
	end
	
	function CWolfStillAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfStillAttackState:Exit")
		if not ( _CCharacter == nil ) then
			
		end
	end
	
	function CWolfStillAttackState:OnMessage(_CCharacter)
		print_logger(0, "CWolfStillAttackState:OnMessage")
	end
	
	function CWolfStillAttackState:__Finalize()
	
	end