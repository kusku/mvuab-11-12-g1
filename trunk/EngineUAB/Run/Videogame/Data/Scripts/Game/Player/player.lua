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
		self.attack2 = CPlayerAttack2State()
		self.attack3 = CPlayerAttack3State()
		self.target_run = CPlayerTargetRunState()
		self.target_attack = CPlayerTargetAttackState()
		self.target_attack2 = CPlayerTargetAttack2State()
		self.target_attack3 = CPlayerTargetAttack3State()
		self.defense = CPlayerDefenseState()
		self.jump = CPlayerJumpState()
		self.hit = CPlayerHitState()
		
		self.animated_idle = CPlayerAnimationIdleState()
		self.animated_run = CPlayerAnimationRunState()
		self.animated_attack = CPlayerAnimationAttackState()
		self.animated_attack2 = CPlayerAnimationAttack2State()
		self.animated_attack3 = CPlayerAnimationAttack3State()
		self.animated_defense = CPlayerAnimationDefenseState()
		self.animated_jump = CPlayerAnimationJumpState()
		self.animated_hit = CPlayerAnimationHitState()
		
		self.animation_time = -1.0
	end

	function CPlayer:init() 
		get_game_process():create_player_camera(1.0, 10000.0, 3.5, 1.0, 1.3, 'Caperucita')
		
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
			
			--Calcula el pitch a partir del ratón
			l_d = core:get_action_to_input():do_action_mouse('PitchPlayer')
			self.pitch = self.pitch + l_d
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
	end
	
	function CPlayer:detect_enemy()
		local character_manager = get_game_process():get_character_manager()
		local enemy = character_manager:search_target_enemy(20.0, math.pi / 4)
		character_manager.preview_target_enemy = enemy
		return enemy
	end
	
	function CPlayer:hit_to_player()
		print_logger(0, "hit")
		self:rest_life( 10 + math.random(0, 10) )
	end