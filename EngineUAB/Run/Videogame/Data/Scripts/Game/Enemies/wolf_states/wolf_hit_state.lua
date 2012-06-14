class 'CWolfHitState' (CState)
	function CWolfHitState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfHitState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfHitState:Enter")
		if not ( _CCharacter == nil ) then

		end
	end
	
	function CWolfHitState:Execute(_CCharacter)
		--print_logger(0, "CWolfHitState:Execute")
	end
	
	function CWolfHitState:OnExit(_CCharacter)
		--print_logger(0, "CWolfHitState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfHitState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfHitState:OnMessage")
		return false
	end
	
	function CWolfHitState:__Finalize()
	
	end