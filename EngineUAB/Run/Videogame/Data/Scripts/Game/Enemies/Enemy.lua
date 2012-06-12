-- ------------------------------------------------------------------------
--	Clase CEnemy
-- ------------------------------------------------------------------------
class 'CEnemy' (CCharacter)
	
	-- ------------------------------
	--		Load States Methods
	-- ------------------------------
	function CEnemy:load_graphic_states()
		print_logger(0, "CEnemy::load_graphic_states->Loading animated states...")
		
		-- Idle State --
		self.animation_idle_state = CWolfAnimatedIdleState()
		if self.animation_idle_state == nil then
			print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado idle del lobo")
		else 
			print_logger(0, "CEnemy::load_graphic_states->CEnemy::load_graphic_states->Creat estado animado idle del lobo")
		end		
		
		-- Walk State --
		self.animation_walk_state = CWolfAnimatedWalkState()
		if self.animation_walk_state == nil then
			print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado Walk del lobo")
		else
			print_logger(0, "CEnemy::load_graphic_states->Creat estado animado Walk del lobo")
		end
		
		-- Run State --
		self.animation_run_state = CWolfAnimatedRunState()
		if self.animation_run_state == nil then
			print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado Run del lobo")
		else
			print_logger(0, "CEnemy::load_graphic_states->Creat estado animado run del lobo")
		end
		
		-- Attack Still State --
		self.animation_still_attack_state = CWolfAnimatedStillAttackState()
		if self.animation_still_attack_state == nil then
			print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado Still Attack del lobo")
		else
			print_logger(0, "CEnemy::load_graphic_states->Creat estado animado run del lobo")
		end
		
		-- Attack Run State --
		self.animation_run_attack_state = CWolfAnimatedRunAttackState()
		if self.animation_run_attack_state == nil then
			print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado Run Attack del lobo")
		else
			print_logger(0, "CEnemy::load_graphic_states->Creat estado animado run del lobo")
		end
		
		-- Hit State --
		self.animation_hit_state = CWolfAnimatedHitState()
		if self.animation_hit_state == nil then
			print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado Hit del lobo")
		else
			print_logger(0, "CEnemy::load_graphic_states->Creat estado animado run del lobo")
		end
		
		-- Deffense State --
		self.animation_deffense_state = CWolfAnimatedDeffenseState()
		if self.animation_hit_state == nil then
			print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado deffense del lobo")
		else
			print_logger(0, "CEnemy::load_graphic_states->Creat estado animado run del lobo")
		end
		
		print_logger(0, "CEnemy::load_graphic_states->Animated states loaded")
	end 
	
	function CEnemy:load_logic_states()
		print_logger(0, "CEnemy::load_logic_states->Loading logic states...")
		
		-- Idle State --
		self.idle_state = CWolfIdleState()
		-- self.idle_state = nil
		if self.idle_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado idle del lobo")
		else 
			print_logger(0, "CEnemy::load_logic_states->Creat estat idle del lobo")
		end		
		
		-- Walk State --
		self.animation_walk_state = CWolfWalkState()
		if self.animation_walk_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Walk del lobo")
		else
			print_logger(0, "CEnemy::load_logic_states->Creat estat Walk del lobo")
		end
		
		-- Run State --
		self.animation_run_state = CWolfRunState()
		if self.animation_run_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Run del lobo")
		else
			print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		end
		
		-- Attack Still State --
		self.animation_still_attack_state = CWolfStillAttackState()
		if self.animation_still_attack_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Still Attack del lobo")
		else
			print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		end
		
		-- Attack Run State --
		self.animation_run_attack_state = CWolfRunAttackState()
		if self.animation_run_attack_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Run Attack del lobo")
		else
			print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		end
		
		-- Hit State --
		self.animation_hit_state = CWolfHitState()
		if self.animation_hit_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Hit del lobo")
		else
			print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		end
		
		-- Deffense State --
		self.animation_deffense_state = CWolfDeffenseState()
		if self.animation_hit_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado deffense del lobo")
		else
			print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		end
		print_logger(0, "CEnemy::load_logic_states->Logic states loaded")
	end	
	
	-- ------------------------------
	--			Constructors
	-- ------------------------------	
	function CEnemy:__init( _id )
		print_logger(0, "CEnemy::__init->Llamada al constructor __init( _id )")
		CCharacter.__init( self, _id )
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		--self.position = Vect3f(0.0, 0.0, 0.0)
		--self.position2 = self.position
		self.locked = false
		
		self.character_manager = get_game_process():get_character_manager()
		self.player = get_game_process():get_character_manager():get_player()
		
		if ( self.player == nil ) then
			print_logger ( 2, "CEnemy::__init()->player is nil" )
		else 
			print_logger ( 0, "CEnemy::__init()->player existe" )
		end
		
		-- Cargamos estados
		self:load_logic_states()
		self:load_graphic_states()
	end
	
	function CEnemy:__init( _id, _name )
		print_logger(0, "CEnemy:init()->Llamada al constructor __init( _id, _name)")
		CCharacter.__init( self, _id, _name )
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		--self.position = Vect3f(3.0, 0.0, 0.0)
		--self.position2 = self.position
		self.locked = false
		
		print_logger(0 , "CEnemy::__init()->posición :"..self.position.x.." "..self.position.y.." "..self.position.z)
		
		self.character_manager = get_game_process():get_character_manager()
		self.player = get_game_process():get_character_manager():get_player()
		
		print_logger(0 , "CEnemy::__init()->posición :"..self.position.x.." "..self.position.y.." "..self.position.z)
		
		if ( self.player == nil ) then
			print_logger ( 2, "CEnemy::__init()->player is nil" )
		else 
			print_logger ( 0, "CEnemy::__init()->player existe" )
		end
		
		-- Cargamos los estados		
		self:load_logic_states()
		self:load_graphic_states()
	end
	
	function CEnemy:init()
		print_logger(0, "CEnemy::init()->Inicializamos estados del enemigo")
		local l_IsOk = false
		
		l_gfsm = self.graphic_fsm 
		if (l_gfsm == nil) then
			print_logger(2, "CEnemy:init()->No se ha podido obtener la máquina de estados gráfica.")
			l_IsOk = false
		else
			print_logger(0, "CEnemy:init()->Máquina de estados gráfica inicializada.")
			l_gfsm.current_state = self.animation_idle_state 
			print_logger(0, "CEnemy:init()->Current state animated idle asignado del enemigo")
			l_IsOk = true
		end
		
		l_lfsm = self.logic_fsm 
		if (l_lfsm == nil) then
			print_logger(2, "CEnemy:init()->No se ha podido obtener la máquina de estados lógica.")
			l_IsOk = false
		else
			print_logger(0, "CEnemy:init()->Máquina de estados lógica inicializada.")
			l_lfsm.current_state = self.idle_state
			print_logger(0, "CEnemy:init()->Current state idle asignado del enemigo")
			l_IsOk = l_IsOk and true
		end
		
		self.enable = false
		
		return l_IsOk
	end
	
	-- ------------------------------
	--			Main Methods
	-- ------------------------------
	function CEnemy:update(_elapsed_time)
		-- print_logger(0, "CEnemy::update()->Actualizando enemigo...")
		-- Almacenamos el elapsed time pq después será impossible de guardar
		self.elapsed_time = _elapsed_time
		
		self.enable = false
		
		l_gfsm = self.graphic_fsm 
		if l_gfsm == nil then
			print_logger(2, "CEnemy:update()->No se ha podido obtener la máquina de estados gráfica.")
		else
			if (self.enable == true ) then
				l_gfsm:update()
				--print_logger(0, "CEnemy::update()->Actualizando gfsm...")			
			else
				--print_logger(0, "CEnemy::update()->Enable = false")
				self.graphic_fsm:change_state(self.animation_idle_state)
				self:move_to( self.position, _elapsed_time )
			end 
		end
		
		-- self.position2 = self.position
		-- local l_pointB = self.position
	end
	
	-- ------------------------------
	--			Destructor
	-- ------------------------------
	function CEnemy:__Finalize()
	
	end
	
	-- ------------------------------
	--			Test Method
	-- ------------------------------
	function CEnemy:is_player_detected()
		--local l_Point = Vect2f(self.position.x, self.position.z)
		local l_PointA = self.locked
		if l_PointA == true then
			--print_logger(0, "IsPlayerDetected: TRUE")
		else
			--print_logger(0, "IsPlayerDetected: FALSE")
		end
		
		local point = self.position2
	end
