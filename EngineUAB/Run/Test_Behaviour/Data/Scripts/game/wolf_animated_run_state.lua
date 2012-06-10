class 'CWolfAnimatedRunState' (CState)
	function CWolfAnimatedRunState:__init() 
		CState.__init(self)
		self.position = Vect2f(0.0,0.0)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfAnimatedRunState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedRunState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedRunState:Execute(_CCharacter)
		--print_logger(0, "CWolfAnimatedRunState:Execute")
	end
	
	function CWolfAnimatedRunState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedRunState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedRunState:OnMessage(_CCharacter)
		print_logger(0, "CWolfAnimatedRunState:OnMessage")
	end
	
	function CWolfAnimatedRunState:__Finalize()
	
	end