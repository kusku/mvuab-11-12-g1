class 'CWolfHitState' (CState)
	function CWolfHitState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfHitState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfHitState:Enter")
		if not ( _CCharacter == nil ) then
			local num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():blend_cycle( num, 1 )
		end
		self.hit_animation_time = 0.0
	end
	
	function CWolfHitState:Execute(_CCharacter)
		--print_logger(0, "CWolfHitState:Execute")
		if ( self.hit_animation_time >= _CCharacter.animated_model:get_current_animation_duration("hit") ) then
			print_logger(0, "CWolfHitState:Execute->Fi del hit")
			_CCharacter:rest_life( _CCharacter.player.properties.strong )
			_CCharacter.logic_fsm:revert_to_previous_state()
		end
		self.hit_animation_time = self.hit_animation_time + _CCharacter.elapsed_time
	end
	
	function CWolfHitState:OnExit(_CCharacter)
		--print_logger(0, "CWolfHitState:Exit")
		_CCharacter.hit_animation_time = 0.0
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():clear_cycle( num, 0.7 )
		end
		print_logger(0, "CWolfHitState:OnExit->WOLF life : ".._CCharacter.properties.life)
	end
	
	function CWolfHitState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CWolfHitState:OnMessage")
		return false
	end
	
	function CWolfHitState:__Finalize()
	
	end