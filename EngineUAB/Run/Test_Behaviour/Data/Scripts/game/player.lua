class 'CCaperucitaIdleState' (CState)
	function CCaperucitaIdleState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado idle de la caperucita")
	end

	function CCaperucitaIdleState:OnEnter(_CCharacter)
		print_logger(0, "CCaperucitaIdleState:Enter")
		if not ( _CCharacter == nil ) then
		-- Cal fer això
		-- CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
		-- int i = l_Core->GetCoreModel()->getCoreAnimationId ( "idle" );
		-- _pCharacter->GetAnimatedModel()->BlendCycle ( i, 0.3f );
		
			num = _CCharacter.core_animation_id
			print_logger(1, "he obtingut el id")
			model = _CCharacter:get_animation_model()
			if not ( _CCharacter == nil ) then
				print_logger(1, "he obtingut el model")
				--model:blend_cycle( num, 0.3 )
				--_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
			end 
		end
	end
	
	function CCaperucitaIdleState:Execute(_CCharacter)
		--print_logger(0, "CCaperucitaIdleState:Execute")
	end
	
	function CCaperucitaIdleState:OnExit(_CCharacter)
		print_logger(0, "CCaperucitaIdleState:Exit")
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
		print_logger(0, "CCaperucitaRunState:Enter")
	end
	
	function CCaperucitaRunState:Execute(_CCharacter)
		print_logger(0, "CCaperucitaRunState:Execute")
	end
	
	function CCaperucitaRunState:OnExit(_CCharacter)
		print_logger(0, "CCaperucitaRunState:Exit")
	end
	
	function CCaperucitaRunState:OnMessage(_CCharacter)
		print_logger(0, "CCaperucitaRunState:OnMessage")
	end
	
	function CCaperucitaRunState:__Finalize()
	
	end
	
class 'CPlayer' (CCharacter)
	Idle = CCaperucitaIdleState()
	if Idle == Nil then
		print_logger(2, "Error al cargar un estado idle")
	else 
		print_logger(0, "Creat estat idle")
	end
	
	Run = CCaperucitaRunState()
	if Run == Nil then
		print_logger(2, "Error al cargar un estado Run")
	else
		print_logger(0, "Creat estat run")
	end
	
	function CPlayer:__init() 
		CCharacter.__init(self,0)
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		self.position = Vect3f(0.0, 0.0, 0.0)
		--self.animated_model.pitch = -90.0
		self.locked = false
	end
	
	function CPlayer:init() 
		print_logger(0, "Entro en el Init del player")
		l_fsm =	self.get_graphic_fsm 
		if (l_fsm == nil) then
			print_logger(2, "maquina d'estats nil")
			return false
		else
			print_logger(2, "maquina d'estats inicialitzada")
			l_fsm.current_state = Idle 
			print_logger(2, "Current state assignat")
			return true
		end
	end
	
	function CPlayer:update(elapsed_time)
		if not self.locked then
			
			-- capturamos la posición para ver si hay un cambio de estado de la animación
			l_pos_anterior = Vect3f(self.physic_controller.position.x,self.physic_controller.position.y,self.physic_controller.position.z)
			--print_logger(0 , "posición anterior :"..l_pos_anterior.x.." "..l_pos_anterior.y.." "..l_pos_anterior.z)
			
			local action_2_input = core:get_action_to_input()
			
			local l_move_player = false
			local l_d = 0.0
			local l_dir = Vect3f(0.0, 0.0, 0.0)
			local l_position = Vect3f(0.0, 0.0, 0.0)
			
			local l_yaw = self.physic_controller.yaw
			l_d = action_2_input:do_action_mouse('YawPlayer')
				l_yaw = l_yaw + l_d
				if l_yaw > 2*math.pi then
					l_yaw = l_yaw - 2*math.pi
				elseif l_yaw < -2*math.pi then
					l_yaw = l_yaw + 2*math.pi
				end
			
			if action_2_input:do_action('MovePlayerUp') then
				if action_2_input:do_action('MovePlayerLeft') then
					l_dir = Vect3f(math.cos(l_yaw + math.pi/4), 0.0, math.sin( l_yaw + math.pi/4 ))
					l_position = l_position + l_dir
				elseif action_2_input:do_action('MovePlayerRight') then
					l_dir = Vect3f(math.cos(l_yaw - math.pi/4), 0.0, math.sin(l_yaw - math.pi/4))
					l_position = l_position + l_dir
				else
					l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
					l_position = l_position + l_dir
				end
				
				l_move_player = true
			elseif action_2_input:do_action('MovePlayerDown') then
				if action_2_input:do_action('MovePlayerLeft') then
					l_dir = Vect3f(math.cos(l_yaw - math.pi/4), 0.0, math.sin(l_yaw - math.pi/4))
					l_position = l_position - l_dir
				elseif action_2_input:do_action('MovePlayerRight') then
					l_dir = Vect3f(math.cos(l_yaw + math.pi/4), 0.0, math.sin(l_yaw + math.pi/4))
					l_position = l_position - l_dir
				else
					l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
					l_position = l_position - l_dir
				end
				
				l_move_player = true
			end
			
			l_position = l_position * 10.0 * elapsed_time
			
			self.physic_controller.yaw = l_yaw
			controller = self.physic_controller:move(l_position, elapsed_time)
			
			self.position = self.physic_controller.position
			self.position = Vect3f(self.position.x, self.position.y - self.physic_controller.height + 0.4, self.position.z)
			self.yaw = l_yaw
			
			self.animated_model.position = self.position
			if l_move_player then
				self.animated_model.yaw = -(l_yaw * 180.0 / math.pi) + 90.0
			end
			
			-- Comprobamos si cambiamos de estado
			--l_pos_actual = Vect3f(self.position.x,self.position.y,self.position.z)
			l_pos_actual = Vect3f(self.physic_controller.position.x,self.physic_controller.position.y,self.physic_controller.position.z)
			--print_logger(0 , "posición actual :"..l_pos_actual.x.." "..l_pos_actual.y.." "..l_pos_actual.z)
			
			--if l_pos_anterior ~= l_pos_actual then
			--if l_pos_anterior.x ~= l_pos_actual.x | l_pos_anterior.y ~= l_pos_actual.y | l_pos_anterior.z ~= l_pos_actual.z then
				--print_logger (0, "No iguals" )
			
			
			l_Fsm =	self.get_graphic_fsm 
			l_Fsm:update()
			if l_Fsm == Nil then
				print_logger(2, "error obtenir FSM")
			else
				--print_logger(0, "OK obtenir FSM")
				if not (l_pos_anterior == l_pos_actual) then 
					l_Fsm:change_state(Run)
					--print_logger(2, "poso a corre")
				else
					l_Fsm:change_state(Idle)
					--print_logger(0, "poso a repos")
				end
			end 
			
		else
			self.physic_controller:move(Vect3f(0.0,0.0,0.0), elapsed_time)
			self.position = self.physic_controller.position
			self.animated_model.position = self.position
		end
	end