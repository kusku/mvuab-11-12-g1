class 'CWolfDeffenseState' (CState)
	function CWolfDeffenseState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfDeffenseState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfDeffenseState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("deffense")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfDeffenseState:Execute(_CCharacter)
		--print_logger(0, "CWolfDeffenseState:Execute")
	end
	
	function CWolfDeffenseState:OnExit(_CCharacter)
		--print_logger(0, "CWolfDeffenseState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("deffense")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfDeffenseState:OnMessage(_CCharacter)
		print_logger(0, "CWolfDeffenseState:OnMessage")
	end
	
	function CWolfDeffenseState:__Finalize()
	
	end
	