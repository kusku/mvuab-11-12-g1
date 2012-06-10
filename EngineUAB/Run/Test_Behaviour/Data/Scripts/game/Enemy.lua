-- ------------------------------------------------------------------------
--	Clase CEnemy
-- ------------------------------------------------------------------------

class 'CEnemy' (CCharacter)
	
	function CEnemy:load_graphic_states()
		print_logger(0, "Loading animated states...")
		
		-- Idle State --
		self.animation_idle_state = CWolfAnimatedIdleState()
		if self.animation_idle_state == nil then
			print_logger(2, "Error al cargar un estado animado idle del lobo")
		else 
			print_logger(0, "Creat estado animado idle del lobo")
		end		
		
		-- Walk State --
		self.animation_walk_state = CWolfAnimatedWalkState()
		if self.animation_walk_state == nil then
			print_logger(2, "Error al cargar un estado animado Walk del lobo")
		else
			print_logger(0, "Creat estado animado Walk del lobo")
		end
		
		-- Run State --
		self.animation_run_state = CWolfAnimatedRunState()
		if self.animation_run_state == nil then
			print_logger(2, "Error al cargar un estado animado Run del lobo")
		else
			print_logger(0, "Creat estado animado run del lobo")
		end
		
		-- Attack Still State --
		self.animation_still_attack_state = CWolfAnimatedStillAttackState()
		if self.animation_still_attack_state == nil then
			print_logger(2, "Error al cargar un estado animado Still Attack del lobo")
		else
			print_logger(0, "Creat estado animado run del lobo")
		end
		
		-- Attack Run State --
		self.animation_run_attack_state = CWolfAnimatedRunAttackState()
		if self.animation_run_attack_state == nil then
			print_logger(2, "Error al cargar un estado animado Run Attack del lobo")
		else
			print_logger(0, "Creat estado animado run del lobo")
		end
		
		-- Hit State --
		self.animation_hit_state = CWolfAnimatedHitState()
		if self.animation_hit_state == nil then
			print_logger(2, "Error al cargar un estado animado Hit del lobo")
		else
			print_logger(0, "Creat estado animado run del lobo")
		end
		
		-- Deffense State --
		self.animation_deffense_state = CWolfAnimatedDeffenseState()
		if self.animation_hit_state == nil then
			print_logger(2, "Error al cargar un estado animado deffense del lobo")
		else
			print_logger(0, "Creat estado animado run del lobo")
		end
		
		print_logger(0, "Animated states loaded")
	end 
	
	function CEnemy:load_logic_states()
		print_logger(0, "Loading logic states...")
		
		-- Idle State --
		self.idle_state = CWolfIdleState()
		-- self.idle_state = nil
		if self.idle_state == nil then
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
		print_logger(0, "Logic states loaded")
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
		--self.position2 = self.position
		self.locked = false
		self.character_manager = get_game_process():get_character_manager()
		
		-- Cargamos estados
		self:load_logic_states()
		self:load_graphic_states()
	end
	
	function CEnemy:__init( _id, _name )
		print_logger(0, "Inicio el enemic __INIT()")
		CCharacter.__init( self, _id, _name )
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		--self.position = Vect3f(3.0, 0.0, 0.0)
		--self.position2 = self.position
		self.locked = false
		self.character_manager = get_game_process():get_character_manager()
		self.player = get_game_process():get_character_manager():get_player()
		
		print_logger(0 , "posición en el __init() :"..self.position.x.." "..self.position.y.." "..self.position.z)
		
		if ( self.player == nil ) then
			print_logger ( 2, "player is nil" )
		else 
			print_logger ( 0, "player existe" )
		end
		
		
		-- Cargamos los estados		
		self:load_logic_states()
		self:load_graphic_states()
	end
	
	function CEnemy:init() 
		print_logger(0, "Entro en el Init() del enemigo")
		l_gfsm = self.graphic_fsm 
		if (l_gfsm == nil) then
			print_logger(2, "máquina de estatos nil del enemigo")
			return false
		else
			print_logger(0, "maquina de estatos gráfica inicialitzada del enemigo")
			l_gfsm.current_state = self.animation_idle_state 
			print_logger(0, "Current state animated idle asignado del enemigo")
			return true
		end
		
		l_lfsm = self.logic_fsm 
		if (l_lfsm == nil) then
			print_logger(2, "máquina de estatos nil del enemic")
			return false
		else
			print_logger(0, "maquina de estatos logica inicialitzada del enemic")
			l_lfsm.current_state = self.idle_state 
			print_logger(0, "Current state idle asignado del enemic")
			return true
		end
	end
	
	function CEnemy:is_player_near()
		--if not ( self.player == nil ) then
			--local l_playerpos = self.player.position
			--l_pointA = Vect2f(l_playerpos.x, l_playerpos.z)
			--print_logger(0 , "posición en el __init() :"..self.position.x.." "..self.position.y.." "..self.position.z)
			print_logger (0, " is player near ")
			--local l_pointB = self.position2
			--local l = self.properties
		--end
		
		return true
	end
		
	function CEnemy:is_player_chased()
		print_logger (0, " is player chased ")
		
		-- Mirar los waypoints para ver si la distancia de la suma de todos los waypoints és menor que self.properties.distance_chase
		
		return true
	end
	
	function CEnemy:is_player_detected()
		local l_Point = self.position2
		print_logger (0, " is player detected MALOOOO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
			
		--l_pointB = Vect2f (self.position2.x, self.position2.z)
		--local l_Point = self.position2
		
		--l = self:is_player_too_near()		NO FUNCIONA!!
		if ( CEnemy:is_player_near() and CEnemy:is_player_chased() ) then
			print_logger(0, "valor : es true")
			return true
		else
			print_logger(0, "valor : es false")
			return false
		end
	end
	
	-- ------------------------------
	--			Update
	-- ------------------------------
	function CEnemy:update(elapsed_time)
		-- Almacenamos el elapsed time pq después será impossible de guardar
		self.elapsed_time = elapsed_time
		
		self.position2 = self.position
		local l_pointB = self.position
		
		l_fsm =	self.graphic_fsm
		if l_fsm == nil then
			print_logger(2, "error obteniendo el graphic fsm")
		else
			l_fsm:update()				
		end 
	end

	-- ------------------------------
	--			Destructor
	-- ------------------------------
	function CEnemy:__Finalize()
	
	end
	

