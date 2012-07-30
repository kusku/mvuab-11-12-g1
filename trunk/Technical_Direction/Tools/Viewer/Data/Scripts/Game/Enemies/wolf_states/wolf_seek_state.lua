class 'CWolfSeekState' (CState)
	function CWolfSeekState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfSeekState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfSeekState:Enter")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfSeekState:Execute(_CCharacter)
		--print_logger(0, "CWolfSeekState:Execute")
	end
	
	function CWolfSeekState:OnExit(_CCharacter)
		--print_logger(0, "CWolfSeekState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfSeekState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfSeekState:OnMessage")
		return false
	end
	
	function CWolfSeekState:__Finalize()
	
	end