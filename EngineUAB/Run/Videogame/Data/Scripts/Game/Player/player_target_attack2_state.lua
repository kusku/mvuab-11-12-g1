class 'CPlayerTargetAttack2State' (CState)
	function CPlayerTargetAttack2State:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerTargetAttack2State:OnEnter(_CCharacter)
		self.animation_time = 0.0	
	end
	
	function CPlayerTargetAttack2State:Execute(_CCharacter)
		if self.animation_time > _CCharacter.animated_model:get_current_animation_duration("attack1") - 0.02 then
			if core:get_action_to_input():do_action('AttackPlayer') then
					_CCharacter.logic_fsm:change_state(_CCharacter.target_attack3)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack3)
			else
				if get_game_process():get_time_between_clicks() < 0.3 then
					_CCharacter.logic_fsm:change_state(_CCharacter.target_attack3)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack3)
				else
					_CCharacter.logic_fsm:change_state(_CCharacter.idle)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
				end
			end
		else
			self.animation_time = self.animation_time + _CCharacter.elapsed_time
		end
		
		--Movimiento del player hacia adelante
		local l_dir = Vect3f(0.0, 0.0, 0.0)
		if core:get_action_to_input():do_action('MovePlayerUp') then
			l_dir = Vect3f(math.cos(_CCharacter.yaw), 0.0, math.sin(_CCharacter.yaw))
		end
		
		--Le aplica la velocidad al movimiento
		l_dir = l_dir * 2.0 * _CCharacter.elapsed_time
		
		--Mueve el controller físico
		_CCharacter.physic_controller:move(l_dir, _CCharacter.elapsed_time)
		
		--Actualiza la posición del objeto 3D
		_CCharacter.position = _CCharacter.physic_controller.position
		_CCharacter.position = Vect3f(_CCharacter.position.x, _CCharacter.position.y - _CCharacter.physic_controller.height + 0.4, _CCharacter.position.z)
		
		--Actualiza la posición del modelo animado
		_CCharacter.animated_model.position = _CCharacter.position
	end
	
	function CPlayerTargetAttack2State:OnExit(_CCharacter)
	end
	
	function CPlayerTargetAttack2State:OnMessage(_CCharacter, _Msg)
	end
	
	function CPlayerTargetAttack2State:__Finalize()
	end
	