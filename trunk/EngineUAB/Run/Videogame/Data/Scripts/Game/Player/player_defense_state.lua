class 'CPlayerDefenseState' (CState)
	function CPlayerDefenseState:__init(name) 
		CState.__init(self, name)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerDefenseState:OnEnter(_CCharacter)
		if core:is_debug_mode() then
			core:get_debug_gui_manager().debug_render:set_state_name("Defense")
		end
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
		return false
	end
	
	function CPlayerDefenseState:__Finalize()
	end