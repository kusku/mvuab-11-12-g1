class 'CWolfRunState' (CState)
	function CWolfRunState:__init() 
		CState.__init(self)
		self.position = Vect2f(0.0,0.0)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfRunState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfRunState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfRunState:Execute(_CCharacter)
		--print_logger(0, "CWolfRunState:Execute")
	end
	
	function CWolfRunState:OnExit(_CCharacter)
		--print_logger(0, "CWolfRunState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfRunState:OnMessage(_CCharacter)
		print_logger(0, "CWolfRunState:OnMessage")
	end
	
	function CWolfRunState:__Finalize()
	
	end