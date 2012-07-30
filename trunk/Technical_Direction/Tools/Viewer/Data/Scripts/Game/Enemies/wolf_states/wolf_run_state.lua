class 'CWolfRunState' (CState)
	function CWolfRunState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfRunState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfRunState:Enter")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfRunState:Execute(_CCharacter)
		--print_logger(0, "CWolfRunState:Execute")
	end
	
	function CWolfRunState:OnExit(_CCharacter)
		--print_logger(0, "CWolfRunState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfRunState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfRunState:OnMessage")
		return false
	end
	
	function CWolfRunState:__Finalize()
	
	end