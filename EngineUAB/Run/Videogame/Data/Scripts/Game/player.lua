class 'CCaperucitaIdleState' (CState)
	function CCaperucitaIdleState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado idle de la caperucita")
	end

	function CCaperucitaIdleState:OnEnter(_CCharacter)
		--print_logger(0, "CCaperucitaIdleState:Enter")
		
		-- Cal fer això
		-- CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
		-- int i = l_Core->GetCoreModel()->getCoreAnimationId ( "idle" );
		-- _pCharacter->GetAnimatedModel()->BlendCycle ( i, 0.3f );
		
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CCaperucitaIdleState:Execute(_CCharacter)
		--print_logger(0, "CCaperucitaIdleState:Execute")
	end
	
	function CCaperucitaIdleState:OnExit(_CCharacter)
		--print_logger(0, "CCaperucitaIdleState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CCaperucitaIdleState:OnMessage(_CCharacter)
		print_logger(0, "CCaperucitaIdleState:OnMessage")	
	end
	
	function CCaperucitaIdleState:__Finalize()
	
	end
	
class 'CCaperucitaRunState' (CState)
	function CCaperucitaRunState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CCaperucitaRunState:OnEnter(_CCharacter)
		--print_logger(0, "CCaperucitaRunState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CCaperucitaRunState:Execute(_CCharacter)
		--print_logger(0, "CCaperucitaRunState:Execute")
	end
	
	function CCaperucitaRunState:OnExit(_CCharacter)
		--print_logger(0, "CCaperucitaRunState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CCaperucitaRunState:OnMessage(_CCharacter)
		print_logger(0, "CCaperucitaRunState:OnMessage")
	end
	
	function CCaperucitaRunState:__Finalize()
	
	end

--------------------------------------
-- PLAYER
--------------------------------------
class 'CPlayer' (CCharacter)
	function CPlayer:__init()
		CCharacter.__init(self)
		
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		self.position = Vect3f(0.0, 0.0, 0.0)
		
		self.locked = false
		
		--self.animated_model.yaw = (self.yaw *math.pi / 180.0) + 90.0
		--self.animated_model.pitch = 0
		
		self.idle = CCaperucitaIdleState()
		self.run = CCaperucitaRunState()
	end

	function CPlayer:init() 
		l_fsm =	self.get_graphic_fsm 
		if (l_fsm == nil) then
			print_logger(2, "CPlayer::init->Máquina de estado no construida.")
			return false
		else
			l_fsm.current_state = self.idle 
			return true
		end
		
		self.animated_model.yaw = (self.yaw * math.pi / 180.0) + 90.0
		self.animated_model.pitch = 0
	end
	
	function CPlayer:update(elapsed_time)
		local l_dir = Vect3f(0.0, 0.0, 0.0)
		local l_move_player = false
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
		l_Fsm =	self.get_graphic_fsm 
		l_Fsm:update()
		if l_Fsm ~= Nil then
			if l_move_player then 
				l_Fsm:change_state(self.run)
			else
				l_Fsm:change_state(self.idle)
			end
		end 
	end