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
		self.target_run = CPlayerTargetRunState()
		self.target_attack = CPlayerTargetAttackState()
		
		self.animated_idle = CPlayerAnimationIdleState()
		self.animated_run = CPlayerAnimationRunState()
		self.animated_attack = CPlayerAnimationAttackState()
		
		self.animation_time = -1.0
	end

	function CPlayer:init() 
		get_game_process():create_player_camera(1.0, 10000.0, 4.0, 1.0, 1.3, 'Caperucita')
	
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
			self.pitch = angle_filter(self.pitch)
			local l_pi = math.pi
			if self.pitch > l_pi/12 then
				self.pitch = l_pi/12
			elseif self.pitch < -l_pi/6 then
				self.pitch = -l_pi/6
			end
			
			--Crea el salto del player
			--if action_2_input:do_action('PlayerJump') then
				--self.physic_controller:jump(70)
			--end
		end
		
		--Actualizamos los estados en caso de cambiar
		self.logic_fsm:update()
		self.graphic_fsm:update()
	end
	
	function CPlayer:detect_enemy()
		local character_manager = get_game_process():get_character_manager()
		local enemy = character_manager:search_target_enemy(20.0, math.pi / 4)
		character_manager.preview_target_enemy = enemy
		return enemy
	end