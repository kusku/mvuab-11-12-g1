class 'CWolfSeekState' (CState)
	function CWolfSeekState:__init(name) 
		CState.__init(self, name)
		-- print_logger(0, "Inicio del estado SEEK del lobo")
	end

	function CWolfSeekState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfSeekState:Enter")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfSeekState:Execute(_CCharacter, _elapsed_time)
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