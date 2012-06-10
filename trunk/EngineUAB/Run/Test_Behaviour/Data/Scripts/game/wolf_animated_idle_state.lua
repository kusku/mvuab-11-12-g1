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
		if ( _CCharacter.is_player_detected() ) then 
			print_logger ( 0 , "player is detected ")
			
			print_logger(0 , "posición anterior :".._CCharacter.position.x.." ".._CCharacter.position.y.." ".._CCharacter.position.z)
			_CCharacter.position = _CCharacter.position
			
			l_fsm = _CCharacter.graphic_fsm
			if (l_fsm == nil) then
				print_logger(2, "maquina d'estats nil del enemic")
				return false
			else
				print_logger (0 , "Canvi d'estat a RUN" )
				l_fsm:change_state(_CCharacter.animation_run_state)
			end
		else
			print_logger ( 0 , "player not detected ")
			_CCharacter:move_to ( Vect3f(0.0,0.0,0.0), _CCharacter.elapsed_time )
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