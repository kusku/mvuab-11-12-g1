class 'CPlayerRunState' (CState)
	function CPlayerRunState:__init() 
		CState.__init(self)
	end

	function CPlayerRunState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CPlayerRunState:Execute(_CCharacter)
	end
	
	function CPlayerRunState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CPlayerRunState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CPlayerRunState:OnMessage")
		if ( _Msg.Msg == msg_attack ) then
			print_logger(0, "Missatge acceptat per la caperucita... aquí faria el que vull, en principi restà vida...")
			-- If depend tipus d'atac... treu més o menys vida... --
			_CCharacter:rest_life( 1 )
			--_CCharacter.graphic_fsm:change_state(_CCharacter.hit_state)
			print_logger(0, "Player life : ".._CCharacter.properties.life)
			
			return true
		end		
		return false
	end
	
	function CPlayerRunState:__Finalize()
	end