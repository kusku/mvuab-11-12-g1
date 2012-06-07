class 'CCaperucitaIdleState' (CState)
	function CCaperucitaIdleState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado idle de la caperucita")
	end

	function CCaperucitaIdleState:OnEnter(_CCharacter)
		--print_logger(0, "CCaperucitaIdleState:Enter")
		
		-- Cal fer això
		-- CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
		-- int i = l_Core->GetCoreModel()->getCoreAnimationId ( "idle" );
		-- _pCharacter->GetAnimatedModel()->BlendCycle ( i, 0.3f );
		
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CCaperucitaIdleState:Execute(_CCharacter)
		--print_logger(0, "CCaperucitaIdleState:Execute")
	end
	
	function CCaperucitaIdleState:OnExit(_CCharacter)
		--print_logger(0, "CCaperucitaIdleState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CCaperucitaIdleState:OnMessage(_CCharacter)
		print_logger(0, "CCaperucitaIdleState:OnMessage")	
	end
	
	function CCaperucitaIdleState:__Finalize()
	
	end
	
class 'CCaperucitaRunState' (CState)
	function CCaperucitaRunState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CCaperucitaRunState:OnEnter(_CCharacter)
		--print_logger(0, "CCaperucitaRunState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CCaperucitaRunState:Execute(_CCharacter)
		--print_logger(0, "CCaperucitaRunState:Execute")
	end
	
	function CCaperucitaRunState:OnExit(_CCharacter)
		--print_logger(0, "CCaperucitaRunState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CCaperucitaRunState:OnMessage(_CCharacter)
		print_logger(0, "CCaperucitaRunState:OnMessage")
	end
	
	function CCaperucitaRunState:__Finalize()
	
	end