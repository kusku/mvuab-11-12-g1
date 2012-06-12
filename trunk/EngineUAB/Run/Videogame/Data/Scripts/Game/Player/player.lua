--------------------------------------
-- PLAYER
--------------------------------------
class 'CPlayer' (CCharacter)
	function CPlayer:__init()
		CCharacter.__init(self)
		
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		--self.position = Vect3f(0.0, 0.0, 0.0)
		
		self.locked = false
		
		self.idle = CPlayerIdleState()
		self.run = CPlayerRunState()
		self.attack = CPlayerAttackState()
		
		self.animation_time = -1.0
	end

	function CPlayer:init() 
		get_game_process():create_player_camera(1.0, 10000.0, 10.5, 0.7, 1.0, 'Caperucita')
	
		l_gfsm = self.graphic_fsm 
		if (l_gfsm == nil) then
			print_logger(2, "CPlayer::init->Máquina de estado no construida.")
			return false
		else
			l_gfsm.current_state = self.idle 
			l_gfsm:change_state(self.idle)
			return true
		end
		
		self.animated_model.yaw = (self.yaw * math.pi / 180.0) + 90.0
		self.animated_model.pitch = 0
	end
	
	function CPlayer:update(elapsed_time)
		local l_dir = Vect3f(0.0, 0.0, 0.0)
		local l_move_player = false
		local l_attack_player = false
		local l_pos_anterior = self.physic_controller.position
	
		if not self.locked then
			local action_2_input = core:get_action_to_input()
			
			local l_d = 0.0
			local l_yaw = 0.0
			
			--Calcula el pitch a partir del ratón
			l_d = action_2_input:do_action_mouse('PitchPlayer')
			self.pitch = self.pitch + l_d
			if self.pitch > math.pi/10 then
				self.pitch = math.pi/10
			elseif self.pitch < -math.pi/4 then 
				self.pitch = -math.pi/4
			end
			
			--Calcula el yaw a partir del ratón
			l_d = action_2_input:do_action_mouse('YawPlayer')
			self.yaw = self.yaw + l_d
			if self.yaw > 2*math.pi then
				self.yaw = self.yaw - 2*math.pi
			elseif self.yaw < -2*math.pi then
				self.yaw = self.yaw + 2*math.pi
			end
			l_yaw = self.yaw
			
			--Mira los controles de movimiento
			if action_2_input:do_action('MovePlayerUp') then --El player se mueve hacia adelante
				if action_2_input:do_action('MovePlayerLeft') then
					l_yaw = l_yaw + math.pi/4
				elseif action_2_input:do_action('MovePlayerRight') then
					l_yaw = l_yaw - math.pi/4
				end
				l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
				l_move_player = true
				
			elseif action_2_input:do_action('MovePlayerDown') then --El player se mueve hacia atrás
				l_yaw = self.yaw - math.pi
				if action_2_input:do_action('MovePlayerLeft') then
					l_yaw = l_yaw - math.pi/4
				elseif action_2_input:do_action('MovePlayerRight') then
					l_yaw = l_yaw + math.pi/4
				end
				l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
				l_move_player = true
				
			elseif action_2_input:do_action('MovePlayerLeft') then --El player se mueve hacia la izquierda
				l_yaw = l_yaw + math.pi/2
				l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
				l_move_player = true
				
			elseif action_2_input:do_action('MovePlayerRight') then --El player se mueve hacia la derecha
				l_yaw = l_yaw - math.pi/2
				l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
				l_move_player = true
			end
			l_dir = l_dir * 10.0 * elapsed_time
			
			--Crea el salto del player
			if action_2_input:do_action('PlayerJump') then
				self.physic_controller:jump(70)
			end
			
			--Ataque del player
			if self.animation_time <= -1.0 then
				if action_2_input:do_action('AttackPlayer') then --Se pulsa el botón de atacar
					l_attack_player = true
					self.animation_time = 0.0
				end
			elseif self.animation_time > self.animated_model:get_current_animation_duration("attack") - 0.5 then --Finaliza el estado de atacar
				if action_2_input:do_action('AttackPlayer') then --MIra si se vuelve a pulsar para no cambiar a un estado que no se quiere ir
					l_attack_player = true
					self.animation_time = 0.0
				else
					l_attack_player = false	--Se cambia al estado correspondiente
					self.animation_time = -1.0
				end
			else
				l_attack_player = true --Controla el tiempo de la animación
				self.animation_time = self.animation_time + elapsed_time
			end
			
			--Establece los ángulos de rotación si se mueve el player
			if l_move_player then
				self.physic_controller.yaw = l_yaw
				self.animated_model.yaw = -(l_yaw * 180.0 / math.pi) + 90.0
			end
		end
		
		--Mueve el controller físico
		self.physic_controller:move(l_dir, elapsed_time)
		
		--Actualiza la posición del objeto 3D
		self.position = self.physic_controller.position
		self.position = Vect3f(self.position.x, self.position.y - self.physic_controller.height + 0.4, self.position.z)
		
		--Actualiza la posición del modelo animado
		self.animated_model.position = self.position
		
		--Analizamos el cambio de posición
		local l_pos_actual = self.physic_controller.position
		
		--Actualizamos los estados en caso de cambiar
		l_gfsm =	self.graphic_fsm 
		l_gfsm:update()
		if l_attack_player then
			l_gfsm:change_state(self.attack)
		elseif l_move_player then 
			l_gfsm:change_state(self.run)
		else
			l_gfsm:change_state(self.idle)
		end	
		
		----------------------
		--Finaliza el movimiento del player
		----------------------
		self:detect_enemy()
	end
	
	function CPlayer:detect_enemy()
		local character_manager = get_game_process():get_character_manager()
		local enemy = character_manager:is_player_near_enemy(10.0)
		character_manager.target_enemy = enemy
	end