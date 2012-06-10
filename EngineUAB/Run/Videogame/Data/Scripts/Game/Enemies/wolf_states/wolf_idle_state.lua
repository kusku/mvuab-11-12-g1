class 'CWolfIdleState' (CState)
	function CWolfIdleState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado idle de la caperucita")
		--self.position = Vect3f(0.0,0.0,0.0)
	end

	function CWolfIdleState:OnEnter(_CCharacter)
		print_logger(0, "CWolfIdleState:Enter")
		
		-- Cal fer aixó
		-- CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
		-- int i = l_Core->GetCoreModel()->getCoreAnimationId ( "idle" );
		-- _pCharacter->GetAnimatedModel()->BlendCycle ( i, 0.3f );
		
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfIdleState:Execute(_Character)
		--print_logger(0, "CWolfIdleState:Execute")
		_Character:is_player_detected()
	end
	
	function CWolfIdleState:OnExit(_CCharacter)
		print_logger(0, "CWolfIdleState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfIdleState:OnMessage(_CCharacter)
		print_logger(0, "CWolfIdleState:OnMessage")	
	end
	
	function CWolfIdleState:__Finalize()
	
	end
	