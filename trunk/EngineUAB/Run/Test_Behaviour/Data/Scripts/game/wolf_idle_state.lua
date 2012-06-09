class 'CWolfIdleState' (CState)
	function CWolfIdleState:__init() 
		CState.__init(self)
		self.position = Vect2f(0.0,0.0)
		print_logger(0, "Inicio del estado idle de la caperucita")
	end

	function CWolfIdleState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfIdleState:Enter")

		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfIdleState:Execute(_CCharacter)
		--print_logger(0, "CWolfIdleState:Execute")
		if ( _CCharacter.is_player_detected() ) then 
			print_logger ( 0 , "player is detected ")
			
			l_fsm = _CCharacter.graphic_fsm
			if (l_fsm == nil) then
				print_logger(2, "maquina d'estats nil del enemic")
				return false
			else
				l_fsm:change_state(_CCharacter.animation_run_state)
			end
		else
			_CCharacter:move_to ( Vect3f(0.0,0.0,0.0), _CCharacter.elapsed_time )
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