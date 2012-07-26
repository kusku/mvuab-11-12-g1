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
				
		self.animation_time = -1.0
		
		self.movement_zoom = 3.3
		self.static_zoom = 3.0
		self.velocity_adaptative_zoom = 1.5
	end

	function CPlayer:init() 
		get_game_process():create_player_camera(1.0, 10000.0, self.static_zoom, 0.7, 0.7, 'Caperucita')
		
		self:create_callbacks()
		self:create_states()
		
		l_gfsm = self.graphic_fsm 
		l_gfsm.current_state = self.animated_idle 
		l_gfsm:change_state(self.animated_idle)
		
		l_lfsm = self.logic_fsm
		l_lfsm.current_state = self.idle
		l_lfsm:change_state(self.idle)
		
		self.animated_model.yaw = (self.yaw * math.pi / 180.0) + 90.0
		self.animated_model.pitch = 0
		
		return true
	end
	
	function CPlayer:update(elapsed_time)
		self.elapsed_time = elapsed_time
		
		if not self.locked then
			local l_d = 0.0
			
			--Modifica el zoom según el estado del player
			local l_camera_player = get_game_process().player_camera
			local zoom = l_camera_player.zoom
			if self.logic_fsm.current_state.name ~= "player_idle" then
				if (self.movement_zoom - zoom) > 0.0001 then
					l_camera_player.zoom = zoom + self.velocity_adaptative_zoom * elapsed_time
				else
					l_camera_player.zoom = self.movement_zoom
				end
			else
				if (zoom - self.static_zoom) > 0.0001 then
					l_camera_player.zoom = zoom - self.velocity_adaptative_zoom * elapsed_time
				else
					l_camera_player.zoom = self.static_zoom
				end
			end
			
			
			--Calcula el pitch a partir del ratón
			l_d = core:get_action_to_input():do_action_mouse('PitchPlayer')
			self.pitch = self.pitch - l_d
			local l_pi = math.pi
			if self.pitch > l_pi/12 then
				self.pitch = l_pi/12
			elseif self.pitch < -l_pi/6 then
				self.pitch = -l_pi/6
			end
			
			--Mira si el player hace una defensa
			if core:get_action_to_input():do_action('DefensePlayer') then
				self.logic_fsm:change_state(self.defense)
				self.graphic_fsm:change_state(self.animated_defense)
			end
		end
		
		--Mirar si el personaje está muerto
		if self.properties.life <= 0 then
			change_to_game_over_gui_process()
		end
		
		--Actualizamos los estados en caso de cambiar
		self.logic_fsm:update()
		self.graphic_fsm:update()
		
		--Actualiza la posición del objeto 3D
		self.position = self.physic_controller.position
		self.position = Vect3f(self.position.x, self.position.y - self.physic_controller.height, self.position.z)
		--Actualiza la posición del modelo animado
		local v = Vect3f(self.position.x, self.position.y - self.physic_controller.height + self.properties.animation_offset, self.position.z)
		self.animated_model.position = v
		
		--Actualiza la cámara
		get_game_process().player_camera:update(elapsed_time)
	end
	
	function CPlayer:detect_enemy()
		local character_manager = get_game_process():get_character_manager()
		local enemy = character_manager:search_target_enemy(20.0, math.pi / 3)
		character_manager.preview_target_enemy = enemy
		return enemy
	end
	
	function CPlayer:hit_to_player()
		print_logger(0, "hit")
		self:rest_life( 10 + math.random(0, 10) )
	end
	
	function CPlayer:create_callbacks()
		local animation_callback_manager = get_game_process():get_animation_callback_manager()
		
		animation_callback_manager:create_callback("attack1", self.animated_model )
		animation_callback_manager:create_callback("attack2", self.animated_model )
		animation_callback_manager:create_callback("attack3", self.animated_model )
	end
	
	function CPlayer:create_states()
		self.idle = CPlayerIdleState("player_idle")
		self.run = CPlayerRunState("player_run")
		self.attack = CPlayerAttackState("player_attack")
		self.attack2 = CPlayerAttack2State("player_attack_2")
		self.attack3 = CPlayerAttack3State("player_attack_3")
		self.target_run = CPlayerTargetRunState("player_target_run")
		self.target_attack = CPlayerTargetAttackState("player_target_attack")
		self.target_attack2 = CPlayerTargetAttack2State("player_target_attack_2")
		self.target_attack3 = CPlayerTargetAttack3State("player_target_attack_3")
		self.defense = CPlayerDefenseState("player_defense")
		self.jump = CPlayerJumpState("player_jump")
		self.hit = CPlayerHitState("player_hit")
		
		self.animated_idle = CPlayerAnimationIdleState("player_animation_idle")
		self.animated_run = CPlayerAnimationRunState("player_animation_run")
		self.animated_attack = CPlayerAnimationAttackState("player_animation_attack")
		self.animated_attack2 = CPlayerAnimationAttack2State("player_animation_attack_2")
		self.animated_attack3 = CPlayerAnimationAttack3State("player_animation_attack_3")
		self.animated_defense = CPlayerAnimationDefenseState("player_animation_defense")
		self.animated_jump = CPlayerAnimationJumpState("player_animation_jump")
		self.animated_hit = CPlayerAnimationHitState("player_animation_hit")
	end