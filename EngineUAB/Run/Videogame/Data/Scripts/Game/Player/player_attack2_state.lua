class 'CPlayerAttack2State' (CState)
	function CPlayerAttack2State:__init() 
		CState.__init(self)
	end

	function CPlayerAttack2State:OnEnter(_CCharacter)
		core:get_debug_gui_manager().debug_render:set_state_name("Attack 2")
		print_logger(0, "State: Attack 2")
		
		self.animation_time = 0.0
	end
	
	function CPlayerAttack2State:Execute(_CCharacter)
		if self.animation_time > _CCharacter.animated_model:get_current_animation_duration("attack2") - 0.02 then
			if core:get_action_to_input():do_action('AttackPlayer') then
				_CCharacter.logic_fsm:change_state(_CCharacter.attack3)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack3)
			else
				if get_game_process():get_time_between_clicks() < 0.3 then
					_CCharacter.logic_fsm:change_state(_CCharacter.attack3)
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
	
	function CPlayerAttack2State:OnExit(_CCharacter)
		print_logger(0, "Exit State: Attack 2")
		local enemy = get_game_process():get_character_manager():search_target_enemy(8.0, math.pi/6)
		if enemy ~= nil then
			
			-- Test jordi missatges entre entitats
			print_logger(1, "Player ha atacat a un enemic nº : " ..enemy:get_id())
			_DispatchMgr:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), enemy:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
			
			-- TODO:: AQuest codi hauria d'anar en el OnMessage del llob!!
			-- enemy.properties.life = enemy.properties.life - 10
			-- if enemy.properties.life <= 0 then
				-- enemy.enable = false
			-- end
		end
	end
	
	function CPlayerAttack2State:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CPlayerAttackState:OnMessage")	
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
	
	function CPlayerAttack2State:__Finalize()
	end