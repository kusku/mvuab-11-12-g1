class 'CWolfWanderState' (CState)
	function CWolfWanderState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfWanderState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfWanderState:Enter")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfWanderState:Execute(_CCharacter)
		--print_logger(0, "CWolfWanderState:Execute")
	end
	
	function CWolfWanderState:OnExit(_CCharacter)
		--print_logger(0, "CWolfWanderState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfWanderState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfWanderState:OnMessage")
		return false
	end
	
	function CWolfWanderState:__Finalize()
	
	end