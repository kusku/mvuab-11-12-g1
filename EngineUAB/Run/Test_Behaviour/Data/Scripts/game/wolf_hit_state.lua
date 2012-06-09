class 'CWolfHitState' (CState)
	function CWolfHitState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfHitState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfHitState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfHitState:Execute(_CCharacter)
		--print_logger(0, "CWolfHitState:Execute")
	end
	
	function CWolfHitState:OnExit(_CCharacter)
		--print_logger(0, "CWolfHitState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfHitState:OnMessage(_CCharacter)
		print_logger(0, "CWolfHitState:OnMessage")
	end
	
	function CWolfHitState:__Finalize()
	
	end