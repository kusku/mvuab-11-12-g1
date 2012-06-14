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
		self.is_target_fixed = false
		
		self.idle = CPlayerIdleState()
		self.run = CPlayerRunState()
		self.attack = CPlayerAttackState()
		
		self.animation_time = -1.0
	end

	function CPlayer:init() 
		get_game_process():create_player_camera(1.0, 10000.0, 4.0, 1.0, 1.3, 'Caperucita')
	
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
		local l_target_rotation = false
		local l_pos_anterior = self.physic_controller.position
	
		if not self.locked then
			local l_d = 0.0
			local l_yaw = 0.0
			local enemy_detected = nil
			local action_2_input = core:get_action_to_input()
			
			--Mira si hay un enemigo cerca y lo marca como target
			if not self.is_target_fixed then
				enemy_detected = self:detect_enemy()
			end
			
			--Asigna o no un enemigo como target
			if action_2_input:do_action('PlayerTarget') then
				if not self.is_target_fixed then
					if enemy_detected ~= nil then
						self.is_target_fixed = true
					end
				else
					self.is_target_fixed = false
				end
			end
			
			--Calcula el pitch a partir del ratón
			l_d = action_2_input:do_action_mouse('PitchPlayer')
			self.pitch = self.pitch + l_d
			if self.pitch > math.pi/12 then
				self.pitch = math.pi/12
			elseif self.pitch < -math.pi/6 then 
				self.pitch = -math.pi/6
			end
			
			if not self.is_target_fixed then --Mira si no hay un target asignado
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
			else
				--Está bloqueado sobre un target
				l_target_rotation = true
				
				local l_player_dir = get_game_process().player_camera:get_direction()
				local l_enemy_dir = get_game_process():get_character_manager().target_enemy.position - self.position
				l_player_dir.y = 0.0
				l_enemy_dir.y = 0.0
				l_enemy_dir:normalize(1.0)
				l_player_dir:normalize(1.0)
				l_d = l_enemy_dir:dot(l_player_dir) --Calcula el ángulo entre donde mira el personaje y la dirección hacia el enemigo
				l_d = math.acos(l_d)
				if (math.deg(l_d) > 0.1 or math.deg(l_d) < -0.1)  then
					if( l_d < 0.0 ) then
						print_logger(0,"més petit")
					end
					self.yaw = self.yaw - l_d
					if self.yaw > 2*math.pi then
						self.yaw = self.yaw - 2*math.pi
					elseif self.yaw < -2*math.pi then
						self.yaw = self.yaw + 2*math.pi
					end
					l_move_player = true
				end
			
				l_yaw = self.yaw
				--Se mueve el personaje con el target asignado
				if action_2_input:do_action('MovePlayerUp') then
					l_dir = l_enemy_dir
					l_move_player = true
					
				elseif action_2_input:do_action('MovePlayerDown') then
					l_dir = Vect3f(-l_enemy_dir.x, -l_enemy_dir.y, -l_enemy_dir.z)
					l_move_player = true
				
				elseif action_2_input:do_action('MovePlayerLeft') then
					--l_dir = l_dir:cross(Vect3f(0.0, 1.0, 0.0))
					local l_vector_yaw = l_yaw + math.pi/2
					l_dir = Vect3f(math.cos(l_vector_yaw), 0.0, math.sin(l_vector_yaw))
					l_move_player = true
					
				elseif action_2_input:do_action('MovePlayerRight') then
					local l_vector_yaw = l_yaw -  math.pi/2
					l_dir = Vect3f(math.cos(l_vector_yaw), 0.0, math.sin(l_vector_yaw))
					l_move_player = true
				
				else
					l_dir = Vect3f(0.0, 0.0, 0.0)
				end
				
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
			elseif self.animation_time > self.animated_model:get_current_animation_duration("attack1")-0.05 then --Finaliza el estado de atacar
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
			if l_move_player or l_target_rotation then
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
		l_gfsm = self.graphic_fsm 
		l_gfsm:update()
		if l_attack_player then
			l_gfsm:change_state(self.attack)
		elseif l_move_player then 
			l_gfsm:change_state(self.run)
		else
			l_gfsm:change_state(self.idle)
		end	
	end
	
	function CPlayer:detect_enemy()
		local character_manager = get_game_process():get_character_manager()
		local enemy = character_manager:search_target_enemy(20.0, math.pi / 4)
		character_manager.target_enemy = enemy
		return enemy
	end