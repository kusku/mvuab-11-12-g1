class 'CWolfAnimatedRunState' (CState)
	function CWolfAnimatedRunState:__init() 
		CState.__init(self)
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
		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			--face_to_player( _CCharacter, _CCharacter.player, _CCharacter.elapsed_time )
			
			-- Si el player es atacable
			if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				_CCharacter.graphic_fsm:change_state(_CCharacter.animation_still_attack_state)
			else
				_CCharacter:move_to( _CCharacter.player.position, _CCharacter.elapsed_time )
			end
		else
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_idle_state)
		end
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