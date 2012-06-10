class 'CWolfAnimatedIdleState' (CState)
	function CWolfAnimatedIdleState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado idle de la caperucita")
	end

	function CWolfAnimatedIdleState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedIdleState:Enter")

		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end

	function CWolfAnimatedIdleState:Execute(_CCharacter)
		--print_logger(0, "CWolfAnimatedIdleState:Execute")
		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			_CCharacter:move_to (_CCharacter.player.position, _CCharacter.elapsed_time)
			-- Si el player es atacable
			if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				_CCharacter.graphic_fsm:change_state(_CCharacter.animation_still_attack_state)
			else
				-- player no atacable i movemos hacia el player
				print_logger (0 , "movemos hacia el player con estado a RUN" )
				_CCharacter.graphic_fsm:change_state(_CCharacter.animation_run_state)
			end
			
			--print_logger(0 , "posición anterior :".._CCharacter.position.x.." ".._CCharacter.position.y.." ".._CCharacter.position.z)
			--_CCharacter.position = _CCharacter.position
			
		else
			-- print_logger ( 0 , "player not detected i el enemigo caminaria")
			-- _CCharacter.graphic_fsm:change_state(_CCharacter.animation_walk_state)
			--_CCharacter:move_to( Vect3f(0.0, 0.0, 0.0), _CCharacter.elapsed_time)
		end
	end
	
	function CWolfAnimatedIdleState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedIdleState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedIdleState:OnMessage(_CCharacter)
		print_logger(0, "CWolfAnimatedIdleState:OnMessage")	
	end
	
	function CWolfAnimatedIdleState:__Finalize()
	
	end