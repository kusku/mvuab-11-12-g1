class 'CPlayerDefenseState' (CState)
	function CPlayerDefenseState:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerDefenseState:OnEnter(_CCharacter)
		core:get_debug_gui_manager().debug_render:set_state_name("Defense")
	end
	
	function CPlayerDefenseState:Execute(_CCharacter)
		if not self.action_2_input:do_action('DefensePlayer') then
			_CCharacter.logic_fsm:change_state(_CCharacter.idle)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
		end
		
		--Actualiza el personaje
		_CCharacter.physic_controller:move(Vect3f(0.0, 0.0, 0.0), _CCharacter.elapsed_time)
	end
	
	function CPlayerDefenseState:OnExit(_CCharacter)
	end
	
	function CPlayerDefenseState:OnMessage(_CCharacter, _Msg)
	end
	
	function CPlayerDefenseState:__Finalize()
	end