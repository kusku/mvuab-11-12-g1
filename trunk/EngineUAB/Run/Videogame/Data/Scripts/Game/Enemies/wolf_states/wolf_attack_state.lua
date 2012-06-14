class 'CWolfAttackState' (CState)
	function CWolfAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAttackState:Enter")
		
	end
	
	function CWolfAttackState:Execute(_CCharacter)
		--print_logger(0, "CWolfAttackState:Execute")
		if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
			_CCharacter:move_to( _CCharacter.player.position, _CCharacter.elapsed_time )
			-- TODO!!! Enviariamos un mensaje de hit al player!!
			
		
				--let the wife know I'm home
				-- Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
							  -- -- pMiner->ID(),        //ID of sender
							  -- -- ent_Elsa,            //ID of recipient
							  -- -- Msg_HiHoneyImHome,   //the message
							  -- -- NO_ADDITIONAL_INFO);    
			-- end
			
		else
			_CCharacter:move_to( _CCharacter.position, _CCharacter.elapsed_time )
			_CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
		end
	end
	
	function CWolfAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAttackState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfAttackState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAttackState:OnMessage")
		-- if ( _Msg == Msg_Attack ) then
			-- print_logger(0, "Missatge acceptat per el llob")
			-- return true
		-- end
		return false
	end
	
	function CWolfAttackState:__Finalize()
	
	end
