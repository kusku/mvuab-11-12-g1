class 'CWolfIdleState' (CState)
	function CWolfIdleState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado idle de la caperucita")
	end

	function CWolfIdleState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfIdleState:Enter")

		if not ( _CCharacter == nil ) then
		
		end
	end
	
	function CWolfIdleState:Execute(_CCharacter)
		--print_logger(0, "CWolfIdleState:Execute")
		if ( is_player_detected() ) then 
			print_logger ( 0 , "player is detected ")
			
			local l_fsm = _CCharacter.logic_fsm
			if (l_fsm == nil) then
				print_logger(2, "maquina d'estats nil del enemic")
				return false
			else
				l_fsm:change_state(_CCharacter.run_state)
			end
		--else
			--_CCharacter:move_to ( Vect3f(0.0,0.0,0.0), _CCharacter.elapsed_time )
		end
	end
	
	function CWolfIdleState:OnExit(_CCharacter)
		--print_logger(0, "CWolfIdleState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfIdleState:OnMessage(_CCharacter)
		print_logger(0, "CWolfIdleState:OnMessage")	
	end
	
	function CWolfIdleState:__Finalize()
	
	end
	