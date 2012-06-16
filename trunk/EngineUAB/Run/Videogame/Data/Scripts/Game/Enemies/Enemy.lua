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
		
		-- Pursuit State --
		self.animation_pursuit_state = CWolfAnimatedPursuitState()
		if self.animation_pursuit_state == nil then
			print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado PURSUIT del lobo")
		else
			print_logger(0, "CEnemy::load_graphic_states->Creat estado animado PURSUIT del lobo")
		end
		
		-- Attack State --
		self.animation_attack_state = CWolfAnimatedAttackState()
		if self.animation_attack_state == nil then
			print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado ATTACK del lobo")
		else
			print_logger(0, "CEnemy::load_graphic_states->Creat estado animado ATTACK del lobo")
		end
		
		-- -- Attack Run State --
		-- self.animation_run_attack_state = CWolfAnimatedRunAttackState()
		-- if self.animation_run_attack_state == nil then
			-- print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado Run Attack del lobo")
		-- else
			-- print_logger(0, "CEnemy::load_graphic_states->Creat estado animado run del lobo")
		-- end
		
		-- -- Hit State --
		-- self.animation_hit_state = CWolfAnimatedHitState()
		-- if self.animation_hit_state == nil then
			-- print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado Hit del lobo")
		-- else
			-- print_logger(0, "CEnemy::load_graphic_states->Creat estado animado run del lobo")
		-- end
		
		-- -- Deffense State --
		-- self.animation_deffense_state = CWolfAnimatedDeffenseState()
		-- if self.animation_hit_state == nil then
			-- print_logger(2, "CEnemy::load_graphic_states->Error al cargar un estado animado deffense del lobo")
		-- else
			-- print_logger(0, "CEnemy::load_graphic_states->Creat estado animado run del lobo")
		-- end
		
		print_logger(0, "CEnemy::load_graphic_states->Animated states loaded")
	end 
	
	function CEnemy:load_logic_states()
		print_logger(0, "CEnemy::load_logic_states->Loading logic states...")
		
		-- Idle State --
		self.idle_state = CWolfIdleState()
		if self.idle_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado IDLE del lobo")
		else 
			print_logger(0, "CEnemy::load_logic_states->Creat estat IDLE del lobo")
		end		
		
		-- Seek State -- Ir directamente a un destino
		self.seek_state = CWolfSeekState()
		if self.seek_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado SEEK del lobo")
		else 
			print_logger(0, "CEnemy::load_logic_states->Creat estat SEEK del lobo")
		end	
		
		-- Flee State -- Huye directamente de un destino
		self.flee_state = CWolfFleeState()
		if self.flee_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado FLEE del lobo")
		else 
			print_logger(0, "CEnemy::load_logic_states->Creat estat FLEE del lobo")
		end	
		
		-- Pursuit State -- Persigue inteligentemente
		self.pursuit_state = CWolfPursuitState()
		if self.pursuit_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado PURSUIT del lobo")
		else 
			print_logger(0, "CEnemy::load_logic_states->Creat estat PURSUIT del lobo")
		end	
		
		-- Evade State -- Huye inteligentemente
		self.evade_state = CWolfEvadeState()
		if self.evade_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado EVADE del lobo")
		else 
			print_logger(0, "CEnemy::load_logic_states->Creat estat EVADE del lobo")
		end	
		
		-- Wander State -- Actua como un errante
		self.wander_state = CWolfWanderState()
		if self.wander_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado WANDER del lobo")
		else 
			print_logger(0, "CEnemy::load_logic_states->Creat estat WANDER del lobo")
		end	
		
		-- Path Follow State --	Sigue un camino
		self.path_follow_state = CWolfPathFollowState()
		if self.path_follow_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado PATH FOLLOW del lobo")
		else 
			print_logger(0, "CEnemy::load_logic_states->Creat estat PATH FOLLOW del lobo")
		end	
		
		-- Attack State --	Entra en estado de ataque al player
		self.attack_state = CWolfAttackState()
		if self.attack_state == nil then
			print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado ATTACK del lobo")
		else 
			print_logger(0, "CEnemy::load_logic_states->Creat estat ATTACK del lobo")
		end	
		
		-- -- Walk State --
		-- self.animation_walk_state = CWolfWalkState()
		-- if self.walk_state == nil then
			-- print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Walk del lobo")
		-- else
			-- print_logger(0, "CEnemy::load_logic_states->Creat estat Walk del lobo")
		-- end
		
		-- -- Run State --
		-- self.animation_run_state = CWolfRunState()
		-- if self.animation_run_state == nil then
			-- print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Run del lobo")
		-- else
			-- print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		-- end
		
		-- -- Attack Still State --
		-- self.animation_still_attack_state = CWolfStillAttackState()
		-- if self.animation_still_attack_state == nil then
			-- print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Still Attack del lobo")
		-- else
			-- print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		-- end
		
		-- -- Attack Run State --
		-- self.animation_run_attack_state = CWolfRunAttackState()
		-- if self.animation_run_attack_state == nil then
			-- print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Run Attack del lobo")
		-- else
			-- print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		-- end
		
		-- -- Hit State --
		-- self.animation_hit_state = CWolfHitState()
		-- if self.animation_hit_state == nil then
			-- print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado Hit del lobo")
		-- else
			-- print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		-- end
		
		-- -- Deffense State --
		-- self.animation_deffense_state = CWolfDeffenseState()
		-- if self.animation_hit_state == nil then
			-- print_logger(2, "CEnemy::load_logic_states->Error al cargar un estado deffense del lobo")
		-- else
			-- print_logger(0, "CEnemy::load_logic_states->Creat estat run del lobo")
		-- end
		-- print_logger(0, "CEnemy::load_logic_states->Logic states loaded")
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
		--self:load_graphic_states()
	end
	
	function CEnemy:__init( _id, _name )
		print_logger(0, "CEnemy:init()->Llamada al constructor __init( _id, _name)")
		CCharacter.__init( self, _id, _name )
		--self.yaw = 0.0
		self.pitch = -math.pi / 8
		--self.roll = 0.0
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
		--self:load_graphic_states()
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
			self:get_animation_model():clear_cycle( self:get_animation_id("run"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("attack_still"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("idle"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("walk"), 0.3 )
			l_lfsm.current_state = self.idle_state
			num = self:get_animation_id("idle")
			self:get_animation_model():blend_cycle( num, 0.3 )
			
			print_logger(0, "CEnemy:init()->Current state idle asignado del enemigo")
			l_IsOk = l_IsOk and true
		end
		
		self.enable = false
		self.fatigue = 2
		
		return l_IsOk
	end
	
	-- ------------------------------
	--			Main Methods
	-- ------------------------------
	function CEnemy:update(_elapsed_time)
		-- print_logger(0, "CEnemy::update()->Actualizando enemigo...")
		-- Almacenamos el elapsed time pq después será impossible de guardar
		self.elapsed_time = _elapsed_time
		-- print_logger (1, "CEnemy:Update()-> NAME : "..self.name)
		-- print_logger (1 , "CEnemy::Update()->POSITION :"..self.position.x.." "..self.position.y.." "..self.position.z)
		
		-- l_gfsm = self.graphic_fsm 
		-- if l_gfsm == nil then
			-- print_logger(2, "CEnemy:update()->No se ha podido obtener la máquina de estados gráfica.")
		-- else
			-- if (self.enable == true ) then
				-- l_gfsm:update()
				-- --print_logger(0, "CEnemy::update()->Actualizando gfsm...")			
			-- else
				-- --print_logger(0, "CEnemy::update()->Enable = false")
				-- l_gfsm:change_state(self.animation_idle_state)
			-- end 
		-- end
		
		-- Actualizamos parte de la IA
		-- self.behaviours.seek.target =  self.player.position
		-- self.steering_entity.velocity = self.behaviours:update( _elapsed_time, self.steering_entity )
		-- self.steering_entity.position = self.steering_entity.position + self.steering_entity.velocity
			
		local l_lfsm = self.logic_fsm 
		if l_lfsm == nil then
			print_logger(2, "CEnemy:update()->No se ha podido obtener la máquina de estados gráfica.")
		else
			if (self.enable == true ) then
				l_lfsm:update()
				--print_logger(0, "CEnemy::update()->Actualizando lfsm...")			
			else
				--print_logger(0, "CEnemy::update()->Enable = false")
				l_lfsm:change_state(self.idle_state)
				self:move_to( self.position, _elapsed_time )
			end 
		end
		
		if ( core == nil ) then
			print_logger(2, "CORE ES NULL")
		-- else
			-- print_logger(1, "CORE ES CORRECTE")
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
		local p = self.position
		--local l_Point = Vect2f(self.position.x, self.position.z)
		local l_PointA = self.locked
		if l_PointA == true then
			--print_logger(0, "IsPlayerDetected: TRUE")
		else
			--print_logger(0, "IsPlayerDetected: FALSE")
		end
		
		local point = self.position2
	end

	
	function CEnemy:is_fatigued()
		return self.fatigue <= 0
	end

	