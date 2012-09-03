class 'CWolfDeffenseState' (CState)
	function CWolfDeffenseState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfDeffenseState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfDeffenseState:Enter")
		if not ( _CCharacter == nil ) then
			
		end
	end
	
	function CWolfDeffenseState:Execute(_CCharacter, _elapsed_time)
		--print_logger(0, "CWolfDeffenseState:Execute")
	end
	
	function CWolfDeffenseState:OnExit(_CCharacter)
		--print_logger(0, "CWolfDeffenseState:Exit")
		if not ( _CCharacter == nil ) then
			
		end
	end
	
	function CWolfDeffenseState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfDeffenseState:OnMessage")
		return false
	end
	
	function CWolfDeffenseState:__Finalize()
	
	end
	