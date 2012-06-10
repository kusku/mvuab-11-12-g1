class 'CWolfAnimatedWalkState' (CState)
	function CWolfAnimatedWalkState:__init() 
		CState.__init(self)
		self.position = Vect2f(0.0,0.0)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfAnimatedWalkState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedWalkState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("walk")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedWalkState:Execute(_CCharacter)
		--print_logger(0, "CWolfAnimatedWalkState:Execute")
	end
	
	function CWolfAnimatedWalkState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedWalkState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("walk")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedWalkState:OnMessage(_CCharacter)
		print_logger(0, "CWolfAnimatedWalkState:OnMessage")
	end
	
	function CWolfAnimatedWalkState:__Finalize()
	
	end