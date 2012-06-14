class 'CWolfFleeState' (CState)
	function CWolfFleeState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfFleeState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfFleeState:Enter")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfFleeState:Execute(_CCharacter)
		--print_logger(0, "CWolfFleeState:Execute")
	end
	
	function CWolfFleeState:OnExit(_CCharacter)
		--print_logger(0, "CWolfFleeState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfFleeState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfFleeState:OnMessage")
		return false
	end
	
	function CWolfFleeState:__Finalize()
	
	end