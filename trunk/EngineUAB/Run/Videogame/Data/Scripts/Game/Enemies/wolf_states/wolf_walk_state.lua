class 'CWolfWalkState' (CState)
	function CWolfWalkState:__init(name) 
		CState.__init(self, name)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfWalkState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfWalkState:Enter")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfWalkState:Execute(_CCharacter)
		--print_logger(0, "CWolfWalkState:Execute")
	end
	
	function CWolfWalkState:OnExit(_CCharacter)
		--print_logger(0, "CWolfWalkState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfWalkState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfWalkState:OnMessage")
		return false
	end
	
	function CWolfWalkState:__Finalize()
	
	end