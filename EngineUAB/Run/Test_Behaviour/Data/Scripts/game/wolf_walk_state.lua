class 'CWolfWalkState' (CState)
	function CWolfWalkState:__init() 
		CState.__init(self)
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
	
	function CWolfWalkState:OnMessage(_CCharacter)
		print_logger(0, "CWolfWalkState:OnMessage")
	end
	
	function CWolfWalkState:__Finalize()
	
	end