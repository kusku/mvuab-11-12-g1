-- ------------------------------------------------------------------------
--	Clase CEnemy
-- ------------------------------------------------------------------------

class 'CEnemy' (CCharacter)
	
	function CEnemy:load_states()
		-- Idle State --
		self.animation_idle_state = CWolfIdleState()
		-- self.animation_idle_state = nil
		if self.animation_idle_state == nil then
			print_logger(2, "Error al cargar un estado idle del lobo")
		else 
			print_logger(0, "Creat estat idle del lobo")
		end		
		
		-- Walk State --
		self.animation_walk_state = CWolfWalkState()
		if self.animation_walk_state == nil then
			print_logger(2, "Error al cargar un estado Walk del lobo")
		else
			print_logger(0, "Creat estat Walk del lobo")
		end
		
		-- Run State --
		self.animation_run_state = CWolfRunState()
		if self.animation_run_state == nil then
			print_logger(2, "Error al cargar un estado Run del lobo")
		else
			print_logger(0, "Creat estat run del lobo")
		end
		
		-- Attack Still State --
		self.animation_still_attack_state = CWolfStillAttackState()
		if self.animation_still_attack_state == nil then
			print_logger(2, "Error al cargar un estado Still Attack del lobo")
		else
			print_logger(0, "Creat estat run del lobo")
		end
		
		-- Attack Run State --
		self.animation_run_attack_state = CWolfRunAttackState()
		if self.animation_run_attack_state == nil then
			print_logger(2, "Error al cargar un estado Run Attack del lobo")
		else
			print_logger(0, "Creat estat run del lobo")
		end
		
		-- Hit State --
		self.animation_hit_state = CWolfHitState()
		if self.animation_hit_state == nil then
			print_logger(2, "Error al cargar un estado Hit del lobo")
		else
			print_logger(0, "Creat estat run del lobo")
		end
		
		-- Deffense State --
		self.animation_deffense_state = CWolfDeffenseState()
		if self.animation_hit_state == nil then
			print_logger(2, "Error al cargar un estado deffense del lobo")
		else
			print_logger(0, "Creat estat run del lobo")
		end
	end 
	
	-- ------------------------------
	--			Constructor
	-- ------------------------------
	function CEnemy:__init( _id )
		print_logger(0, "Inicio el enemic __INIT()")
		CCharacter.__init( self, _id )
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		--self.position = Vect3f(0.0, 0.0, 0.0)
		--self.position = Vect3f(0.0, 0.0, 0.0)
		--self.position2 = self.position 
		self.locked = false
		self.character_manager = get_game_process():get_character_manager()
		
		-- Cargamos estados
		self.load_states()
	end
	
	function CEnemy:__init( _id, _name )
		print_logger(0, "Inicio el enemic __INIT()")
		CCharacter.__init( self, _id, _name )
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		self.position = Vect3f(0.0, 0.0, 0.0)
		self.locked = false
		self.character_manager = get_game_process():get_character_manager()
		self.player = get_game_process():get_character_manager():get_player()
		
		if ( self.player == nil ) then
			print_logger ( 2, "player is nil" )
		else 
			print_logger ( 0, "player existe" )
		end
		
		-- Cargamos los estados		
		self:load_states()
	end
	
	function CEnemy:init() 
		print_logger(0, "Entro en el Init() del enemic")
		l_fsm =	self.graphic_fsm 
		if (l_fsm == nil) then
			print_logger(2, "maquina d'estats nil del enemic")
			return false
		else
			print_logger(0, "maquina d'estats inicialitzada del enemic")
			l_fsm.current_state = self.animation_idle_state 
			print_logger(0, "Current state idle assignat del enemic")
			return true
		end
	end
	
	-- function CEnemy:is_player_too_near()
		-- print_logger (0, " is player near ")
		-- return true
	-- end
	
	function CEnemy:is_player_detected()
	
		print_logger (0, " is player detected ")
		return true
		
		-- --l_pointB = Vect2f (self.position2.x, self.position2.z)
		
		-- --l = self:is_player_too_near()		NO FUNCIONA!!
		-- -- l = CEnemy:is_player_too_near()
		
		-- -- if (l == true) then
			-- -- print_logger(0, "valor : es true")
		-- -- else
			-- -- print_logger(0, "valor : es false")
		-- -- end
		
		
		-- --CCharacter:IsPlayerDetected(self)
		-- --is_player_detected()
		-- --l = is_player_detected()
		-- -- -- local l_pepe = self.position
		-- -- -- local l = "posicio enemic"..l_pepe.x.." Y: "..l_pepe.y
			-- -- -- print_logger (0, l)
		-- -- --local l_Player = self.player
		-- -- if ( get_game_process():get_character_manager():get_player() == nil ) then
			-- -- print_logger(2, "player no trobat" )
			-- -- return false
		-- -- else 
			-- -- --print_logger(0, "player pillat" )
			-- -- l_playerpos = get_game_process():get_character_manager():get_player().position
			-- -- --print_logger(0, "X: "..l_playerpos.x.." Y: "..l_playerpos.y.." Z: "..l_playerpos.z)
			-- -- l_pointA = Vect2f(l_playerpos.x, l_playerpos.z)
			-- -- --local l_pointB = Vect3f(0.0,0.0,0.0)
			-- -- --l_pointB = self.position
			-- -- --l_pointC.x = self.position.x
			-- -- --self.physic_controller.position.x, self.physic_controller.position.z)
			-- -- --l_pointB = self.position
			-- -- --l_pos = self.properties.detection_distance
			-- -- --print_logger(0, "X: "..self.properties.detection_distance)
			-- -- -- si la distancia entre el player i el enemigo es menor de un valor empieza el ataque
			-- -- -- if ( l_pointA:sq_distance(l_pointB) < self.properties.detection_distance ) then
			-- -- -- 		self.graphic_fsm:change_state(self.animation_attack_state)
			-- -- -- end
			-- -- return true
		-- -- end
		
	end
	
	-- ------------------------------
	--			Update
	-- ------------------------------
	function CEnemy:update(elapsed_time)
		l_fsm =	self.graphic_fsm
		if l_fsm == nil then
			print_logger(2, "error obtenir fsm")
		else
			self.elapsed_time = elapsed_time
			l_fsm:update()				
		end 
			
		-- print_logger(0 , "Updating enemy...")
		
		-- -- if( self.physic_controller == NULL || self.animated_model == NULL  || _elapsed_time == 0)
		-- -- {
			-- -- return
		-- -- }
	
		-- --l = self.PlayerIsDetected() 
		-- if ( self.player_is_detected() ) then 
			-- print_logger ( 0 , "player is detected ")
			-- -- -- if ( player_is_too_away() ) then
				-- -- -- -- Acercandose para atacar
			-- -- -- else 
				-- -- -- -- Intercambio de estados de Atacando
			-- -- -- end
			-- -- -- Persiguiendo 
		-- -- --else
			-- -- -- Intercambio de Estados de esperando i patrullando
		-- end 
		
		-- --if not self.locked then
			
			-- -- -- capturamos la posición para ver si hay un cambio de estado de la animación
			-- -- l_pos_anterior = Vect3f(self.physic_controller.position.x,self.physic_controller.position.y,self.physic_controller.position.z)
			-- -- --print_logger(0 , "posición anterior :"..l_pos_anterior.x.." "..l_pos_anterior.y.." "..l_pos_anterior.z)
			
			-- -- local action_2_input = core:get_action_to_input()
			
			-- -- local l_move_player = false
			-- -- local l_d = 0.0
			-- -- local l_dir = Vect3f(0.0, 0.0, 0.0)
			-- -- local l_position = Vect3f(0.0, 0.0, 0.0)
			
			-- -- local l_yaw = self.physic_controller.yaw
			-- -- l_d = action_2_input:do_action_mouse('YawPlayer')
				-- -- l_yaw = l_yaw + l_d
				-- -- if l_yaw > 2*math.pi then
					-- -- l_yaw = l_yaw - 2*math.pi
				-- -- elseif l_yaw < -2*math.pi then
					-- -- l_yaw = l_yaw + 2*math.pi
				-- -- end
			
			-- -- if action_2_input:do_action('MovePlayerUp') then
				-- -- if action_2_input:do_action('MovePlayerLeft') then
					-- -- l_dir = Vect3f(math.cos(l_yaw + math.pi/4), 0.0, math.sin( l_yaw + math.pi/4 ))
					-- -- l_position = l_position + l_dir
				-- -- elseif action_2_input:do_action('MovePlayerRight') then
					-- -- l_dir = Vect3f(math.cos(l_yaw - math.pi/4), 0.0, math.sin(l_yaw - math.pi/4))
					-- -- l_position = l_position + l_dir
				-- -- else
					-- -- l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
					-- -- l_position = l_position + l_dir
				-- -- end
				
				-- -- l_move_player = true
			-- -- elseif action_2_input:do_action('MovePlayerDown') then
				-- -- if action_2_input:do_action('MovePlayerLeft') then
					-- -- l_dir = Vect3f(math.cos(l_yaw - math.pi/4), 0.0, math.sin(l_yaw - math.pi/4))
					-- -- l_position = l_position - l_dir
				-- -- elseif action_2_input:do_action('MovePlayerRight') then
					-- -- l_dir = Vect3f(math.cos(l_yaw + math.pi/4), 0.0, math.sin(l_yaw + math.pi/4))
					-- -- l_position = l_position - l_dir
				-- -- else
					-- -- l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
					-- -- l_position = l_position - l_dir
				-- -- end
				
				-- -- l_move_player = true
			-- -- end
			
			-- -- l_position = l_position * 10.0 * elapsed_time
			
			-- -- self.physic_controller.yaw = l_yaw
			-- -- controller = self.physic_controller:move(l_position, elapsed_time)
			
			-- -- self.position = self.physic_controller.position
			-- -- self.position = Vect3f(self.position.x, self.position.y - self.physic_controller.height + 0.4, self.position.z)
			-- -- self.yaw = l_yaw
			
			-- -- self.animated_model.position = self.position
			-- -- if l_move_player then
				-- -- self.animated_model.yaw = -(l_yaw * 180.0 / math.pi) + 90.0
			-- -- end
			
			-- -- -- Comprobamos si cambiamos de estado
			-- -- --l_pos_actual = Vect3f(self.position.x,self.position.y,self.position.z)
			-- -- l_pos_actual = Vect3f(self.physic_controller.position.x,self.physic_controller.position.y,self.physic_controller.position.z)
			-- -- --print_logger(0 , "posición actual :"..l_pos_actual.x.." "..l_pos_actual.y.." "..l_pos_actual.z)
			
			-- l_fsm =	self.graphic_fsm
			
			-- if l_fsm == nil then
				-- print_logger(2, "error obtenir fsm")
			-- else
				-- l_fsm:update()
				-- --print_logger(0, "ok obtenir fsm")
				-- -- if not (l_pos_anterior == l_pos_actual) then 
					-- l_fsm:change_state(self.animation_idle_state)
					-- --print_logger(2, "poso a corre")
				-- -- else
					-- -- l_fsm:change_state(idle)
					-- -- --print_logger(0, "poso a repos")
				-- -- end
			-- end 
			
		-- -- else
			-- self.physic_controller:move(Vect3f(0.0,0.0,0.0), elapsed_time)
			-- self.position = self.physic_controller.position
			-- self.animated_model.position = self.position
		-- -- end
	end

	-- ------------------------------
	--			Destructor
	-- ------------------------------
	function CEnemy:__Finalize()
	
	end
	

