class 'CPlayerRunState' (CState)
	function CPlayerRunState:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerRunState:OnEnter(_CCharacter)
	end
	
	function CPlayerRunState:Execute(_CCharacter)
		local l_d = 0.0
		local l_yaw = 0.0
		local l_dir = Vect3f(0.0, 0.0, 0.0)
		local l_move_player = false
		local enemy_detected = nil
		
		--Miramos si hay un target a poder fijarse
		if not _CCharacter.is_target_fixed then
			enemy_detected = _CCharacter:detect_enemy()
		end
		
		--Actualización del yaw a partir del movimiento del mouse
		l_d = self.action_2_input:do_action_mouse('YawPlayer')
		_CCharacter.yaw = _CCharacter.yaw + l_d
		_CCharacter.yaw = angle_filter(_CCharacter.yaw)
		l_yaw = _CCharacter.yaw
	
		--Mira los controles de movimiento
		if self.action_2_input:do_action('MovePlayerUp') then --El player se mueve hacia adelante
			if self.action_2_input:do_action('MovePlayerLeft') then
				l_yaw = l_yaw + math.pi/4
			elseif self.action_2_input:do_action('MovePlayerRight') then
				l_yaw = l_yaw - math.pi/4
			end
			l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
			l_move_player = true
			
		elseif self.action_2_input:do_action('MovePlayerDown') then --El player se mueve hacia atrás
			l_yaw = l_yaw - math.pi
			if self.action_2_input:do_action('MovePlayerLeft') then
				l_yaw = l_yaw - math.pi/4
			elseif self.action_2_input:do_action('MovePlayerRight') then
				l_yaw = l_yaw + math.pi/4
			end
			l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
			l_move_player = true
			
		elseif self.action_2_input:do_action('MovePlayerLeft') then --El player se mueve hacia la izquierda
			l_yaw = l_yaw + math.pi/2
			l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
			l_move_player = true
			
		elseif self.action_2_input:do_action('MovePlayerRight') then --El player se mueve hacia la derecha
			l_yaw = l_yaw - math.pi/2
			l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
			l_move_player = true
		end
			
		--Le aplica la velocidad al movimiento
		l_dir = l_dir * 10.0 * _CCharacter.elapsed_time
		
		--Mueve el controller físico
		_CCharacter.physic_controller:move(l_dir, _CCharacter.elapsed_time)
		
		--Actualiza la posición del objeto 3D
		_CCharacter.position = _CCharacter.physic_controller.position
		_CCharacter.position = Vect3f(_CCharacter.position.x, _CCharacter.position.y - _CCharacter.physic_controller.height + 0.4, _CCharacter.position.z)
		
		--Actualiza la posición del modelo animado
		_CCharacter.animated_model.position = _CCharacter.position
		
		if l_move_player then
			--Pone de forma correcta los ángulos
			_CCharacter.physic_controller.yaw = l_yaw
			_CCharacter.animated_model.yaw = -(l_yaw * 180.0 / math.pi) + 90.0
		else
			--Cambia de estado a idle
			_CCharacter.logic_fsm:change_state(_CCharacter.idle)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
		end
	end
	
	function CPlayerRunState:OnExit(_CCharacter)
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