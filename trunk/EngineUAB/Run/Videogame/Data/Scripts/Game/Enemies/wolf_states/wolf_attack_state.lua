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
		print_logger(0, "Missatge: ".._Msg)
		if ( _Msg.Msg == msg_attack ) then
			print_logger(0, "Missatge acceptat per el llob")
			-- If depend tipus d'atac... treu més o menys vida... --
			self:_CCharacter( 1 )
			print_logger(0, "Player life : "..self.properties.life)
			return true
		end
		return false
	end
	
	function CWolfAttackState:__Finalize()
	
	end
