class 'CPlayerIdleState' (CState)
	function CPlayerIdleState:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerIdleState:OnEnter(_CCharacter)
	end
	
	function CPlayerIdleState:Execute(_CCharacter)
		if self.action_2_input:do_action('PlayerTarget') then
			--Se asigna un target
			if not _CCharacter.is_target_fixed then
				if _CCharacter:detect_enemy() ~= nil then
					_CCharacter.is_target_fixed = true
				end
			else
				_CCharacter.is_target_fixed = false
				get_game_process():get_character_manager().target_enemy = nil
			end
		end
	
		if self.action_2_input:do_action('AttackPlayer') then
			--El jugador ataca
			if _CCharacter.is_target_fixed then
				_CCharacter.logic_fsm:change_state(_CCharacter.target_attack)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
			else
				_CCharacter.logic_fsm:change_state(_CCharacter.attack)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
			end
			
		elseif 	self.action_2_input:do_action('MovePlayerUp') or 
				self.action_2_input:do_action('MovePlayerDown') or 
				self.action_2_input:do_action('MovePlayerLeft') or
				self.action_2_input:do_action('MovePlayerRight') then
			if _CCharacter.is_target_fixed then
				_CCharacter.logic_fsm:change_state(_CCharacter.target_run)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_run)
			else
				_CCharacter.logic_fsm:change_state(_CCharacter.run)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_run)
			end
		end
		
		--Actualización del yaw a partir del movimiento del mouse
		if not _CCharacter.is_target_fixed then
			l_d = self.action_2_input:do_action_mouse('YawPlayer')
			_CCharacter.yaw = _CCharacter.yaw + l_d
			_CCharacter.yaw = angle_filter(_CCharacter.yaw)
		end
		
		--Actualiza el personaje
		_CCharacter.physic_controller:move(Vect3f(0.0, 0.0, 0.0), _CCharacter.elapsed_time)
		
		--Actualiza la posición del objeto 3D
		_CCharacter.position = _CCharacter.physic_controller.position
		_CCharacter.position = Vect3f(_CCharacter.position.x, _CCharacter.position.y - _CCharacter.physic_controller.height + 0.4, _CCharacter.position.z)
		
		--Actualiza la posición del modelo animado
		_CCharacter.animated_model.position = _CCharacter.position
	end
	
	function CPlayerIdleState:OnExit(_CCharacter)
	end
	
	function CPlayerIdleState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CPlayerIdleState:OnMessage")	
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
	
	function CPlayerIdleState:__Finalize()
	end