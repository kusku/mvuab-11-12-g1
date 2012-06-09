class 'CWolfIdleState' (CState)
	function CWolfIdleState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado idle de la caperucita")
	end

	function CWolfIdleState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfIdleState:Enter")
		
		-- Cal fer aixó
		-- CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
		-- int i = l_Core->GetCoreModel()->getCoreAnimationId ( "idle" );
		-- _pCharacter->GetAnimatedModel()->BlendCycle ( i, 0.3f );
		
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfIdleState:Execute(_CCharacter)
		--print_logger(0, "CWolfIdleState:Execute")
		if ( _CCharacter.is_player_detected() ) then 
			print_logger ( 0 , "player is detected ")
		end 
	end
	
	function CWolfIdleState:OnExit(_CCharacter)
		--print_logger(0, "CWolfIdleState:Exit")
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
	
class 'CWolfWalkState' (CState)
	function CWolfWalkState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfWalkState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfWalkState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("walk")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfWalkState:Execute(_CCharacter)
		--print_logger(0, "CWolfWalkState:Execute")
	end
	
	function CWolfWalkState:OnExit(_CCharacter)
		--print_logger(0, "CWolfWalkState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("walk")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfWalkState:OnMessage(_CCharacter)
		print_logger(0, "CWolfWalkState:OnMessage")
	end
	
	function CWolfWalkState:__Finalize()
	
	end
	
class 'CWolfRunState' (CState)
	function CWolfRunState:__init() 
		CState.__init(self)
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
	
class 'CWolfStillAttackState' (CState)
	function CWolfStillAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfStillAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfStillAttackState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfStillAttackState:Execute(_CCharacter)
		--print_logger(0, "CWolfStillAttackState:Execute")
	end
	
	function CWolfStillAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfStillAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfStillAttackState:OnMessage(_CCharacter)
		print_logger(0, "CWolfStillAttackState:OnMessage")
	end
	
	function CWolfStillAttackState:__Finalize()
	
	end
	
class 'CWolfRunAttackState' (CState)
	function CWolfRunAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfRunAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfRunAttackState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfRunAttackState:Execute(_CCharacter)
		--print_logger(0, "CWolfRunAttackState:Execute")
	end
	
	function CWolfRunAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfRunAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfRunAttackState:OnMessage(_CCharacter)
		print_logger(0, "CWolfRunAttackState:OnMessage")
	end
	
	function CWolfRunAttackState:__Finalize()
	
	end
	
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
	