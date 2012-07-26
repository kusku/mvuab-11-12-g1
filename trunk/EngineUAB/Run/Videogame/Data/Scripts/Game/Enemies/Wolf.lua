-- ------------------------------------------------------------------------
--	Clase CWolf
-- ------------------------------------------------------------------------
class 'CWolf' (CCharacter)
	
	-- ------------------------------
	--		Load States Methods
	-- ------------------------------
	function CWolf:load_graphic_states()
		
		-- Idle State --
		self.animation_idle_state = CWolfAnimatedIdleState("wolf_animation_idle")
		if self.animation_idle_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado idle del lobo")
		end		
		
		-- Walk State --
		self.animation_walk_state = CWolfAnimatedWalkState("wolf_animation_walk")
		if self.animation_walk_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado Walk del lobo")
		end
		
		-- Pursuit State --
		self.animation_pursuit_state = CWolfAnimatedPursuitState("wolf_animation_pursiut")
		if self.animation_pursuit_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado PURSUIT del lobo")
		end
		
		-- Attack State --
		self.animation_attack_state = CWolfAnimatedAttackState("wolf_animation_attack")
		if self.animation_attack_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado ATTACK del lobo")
		end
		
		-- -- Attack Run State --
		-- self.animation_run_attack_state = CWolfAnimatedRunAttackState()
		-- if self.animation_run_attack_state == nil then
			-- print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado Run Attack del lobo")
		-- else
			-- print_logger(0, "CWolf::load_graphic_states->Creat estado animado run del lobo")
		-- end
		
		-- -- Hit State --
		-- self.animation_hit_state = CWolfAnimatedHitState()
		-- if self.animation_hit_state == nil then
			-- print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado Hit del lobo")
		-- else
			-- print_logger(0, "CWolf::load_graphic_states->Creat estado animado run del lobo")
		-- end
		
		-- -- Deffense State --
		-- self.animation_deffense_state = CWolfAnimatedDeffenseState()
		-- if self.animation_hit_state == nil then
			-- print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado deffense del lobo")
		-- else
			-- print_logger(0, "CWolf::load_graphic_states->Creat estado animado run del lobo")
		-- end
		
		print_logger(0, "CWolf::load_graphic_states->Animated states loaded")
	end 
	
	function CWolf:load_logic_states()
		
		-- Idle State --
		self.idle_state = CWolfIdleState("wolf_idle")
		if self.idle_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado IDLE del lobo")
		end		
		
		-- Seek State -- Ir directamente a un destino
		self.seek_state = CWolfSeekState("wolf_seek")
		if self.seek_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado SEEK del lobo")
		end	
		
		-- Flee State -- Huye directamente de un destino
		self.flee_state = CWolfFleeState("wolf_flee")
		if self.flee_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado FLEE del lobo")
		end	
		
		-- Pursuit State -- Persigue inteligentemente
		self.pursuit_state = CWolfPursuitState("wolf_pursuit")
		if self.pursuit_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado PURSUIT del lobo")
		end	
		
		-- Evade State -- Huye inteligentemente
		self.evade_state = CWolfEvadeState("wolf_evade")
		if self.evade_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado EVADE del lobo")
		end	
		
		-- Wander State -- Actua como un errante
		self.wander_state = CWolfWanderState("wolf_wander")
		if self.wander_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado WANDER del lobo")
		end	
		
		-- Path Follow State --	Sigue un camino
		self.path_follow_state = CWolfPathFollowState("wolf_path_follow")
		if self.path_follow_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado PATH FOLLOW del lobo")
		end	
		
		-- Attack State --	Entra en estado de ataque al player
		self.attack_state = CWolfAttackState("wolf_attack")
		if self.attack_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado ATTACK del lobo")
		end	
		
		-- -- Walk State --
		-- self.animation_walk_state = CWolfWalkState()
		-- if self.walk_state == nil then
			-- print_logger(2, "CWolf::load_logic_states->Error al cargar un estado Walk del lobo")
		-- else
			-- print_logger(0, "CWolf::load_logic_states->Creat estat Walk del lobo")
		-- end
		
		-- -- Run State --
		-- self.animation_run_state = CWolfRunState()
		-- if self.animation_run_state == nil then
			-- print_logger(2, "CWolf::load_logic_states->Error al cargar un estado Run del lobo")
		-- else
			-- print_logger(0, "CWolf::load_logic_states->Creat estat run del lobo")
		-- end
		
		-- -- Attack Still State --
		-- self.animation_still_attack_state = CWolfStillAttackState()
		-- if self.animation_still_attack_state == nil then
			-- print_logger(2, "CWolf::load_logic_states->Error al cargar un estado Still Attack del lobo")
		-- else
			-- print_logger(0, "CWolf::load_logic_states->Creat estat run del lobo")
		-- end
		
		-- -- Attack Run State --
		-- self.animation_run_attack_state = CWolfRunAttackState()
		-- if self.animation_run_attack_state == nil then
			-- print_logger(2, "CWolf::load_logic_states->Error al cargar un estado Run Attack del lobo")
		-- else
			-- print_logger(0, "CWolf::load_logic_states->Creat estat run del lobo")
		-- end
		
		-- Hit State --
		self.hit_state = CWolfHitState("wolf_hit")
		if self.hit_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado HIT del lobo")
		end
		
		-- Deffense State --
		self.deffense_state = CWolfDeffenseState("wolf_defense")
		if self.deffense_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado DEFFENSE del lobo")
		end
		
		print_logger(0, "CWolf::load_logic_states->Logic states loaded")
	end	
	
	-- ------------------------------
	--			Constructors
	-- ------------------------------	
	function CWolf:__init( _id )
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
			print_logger ( 2, "CWolf::__init()->player is nil" )
		end
		
		-- Cargamos estados
		self:load_logic_states()
		--self:load_graphic_states()
	end
	
	function CWolf:__init( _id, _name )
		CCharacter.__init( self, _id, _name )
		--self.yaw = 0.0
		self.pitch = -math.pi / 8
		--self.roll = 0.0
		--self.position = Vect3f(3.0, 0.0, 0.0)
		--self.position2 = self.position
		self.locked = false
		
		self.character_manager = get_game_process():get_character_manager()
		self.player = get_game_process():get_character_manager():get_player()
			
		if ( self.player == nil ) then
			print_logger ( 2, "CWolf::__init()->player is nil" )
		end
		
		-- Cargamos los estados		
		self:load_logic_states()
		--self:load_graphic_states()
	end
	
	function CWolf:init()
		local l_IsOk = false
		
		l_gfsm = self.graphic_fsm 
		if (l_gfsm == nil) then
			print_logger(2, "CWolf:init()->No se ha podido obtener la máquina de estados gráfica.")
			l_IsOk = false
		else
			l_gfsm.current_state = self.animation_idle_state 
			l_IsOk = true
		end
		
		l_lfsm = self.logic_fsm 
		if (l_lfsm == nil) then
			print_logger(2, "CWolf:init()->No se ha podido obtener la máquina de estados lógica.")
			l_IsOk = false
		else
			self:get_animation_model():clear_cycle( self:get_animation_id("run"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("attack_still"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("idle"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("walk"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("hit"), 0.3 )
			l_lfsm.current_state = self.idle_state
			num = self:get_animation_id("idle")
			self:get_animation_model():blend_cycle( num, 0.3 )
			
			l_IsOk = l_IsOk and true
		end
		
		--self.enable = false
		self.fatigue = 2
		
		return l_IsOk
	end
	
	-- ------------------------------
	--			Main Methods
	-- ------------------------------
	function CWolf:update(_elapsed_time)
		-- print_logger(0, "CWolf::update()->Actualizando enemigo...")
		-- Almacenamos el elapsed time pq después será impossible de guardar
		self.elapsed_time = _elapsed_time
		
		-- print_logger (1, "CWolf:Update()-> NAME : "..self.name)
		-- print_logger (1 , "CWolf::Update()->POSITION :"..self.position.x.." "..self.position.y.." "..self.position.z)
		
		-- l_gfsm = self.graphic_fsm 
		-- if l_gfsm == nil then
			-- print_logger(2, "CWolf:update()->No se ha podido obtener la máquina de estados gráfica.")
		-- else
			-- if (self.enable == true ) then
				-- l_gfsm:update()
				-- --print_logger(0, "CWolf::update()->Actualizando gfsm...")			
			-- else
				-- --print_logger(0, "CWolf::update()->Enable = false")
				-- l_gfsm:change_state(self.animation_idle_state)
			-- end 
		-- end
		
		-- Actualizamos parte de la IA
		-- self.behaviours.seek.target =  self.player.position
		-- self.steering_entity.velocity = self.behaviours:update( _elapsed_time, self.steering_entity )
		-- self.steering_entity.position = self.steering_entity.position + self.steering_entity.velocity
			
		local l_lfsm = self.logic_fsm 
		if l_lfsm == nil then
			print_logger(2, "CWolf:update()->No se ha podido obtener la máquina de estados gráfica.")
		else
			if (self.enable == true ) then
				l_lfsm:update()
				--print_logger(0, "CWolf::update()->Actualizando lfsm...")			
			else
				--print_logger(0, "CWolf::update()->Enable = false")
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
	function CWolf:__Finalize()
	
	end
	
	-- ------------------------------
	--			Test Method
	-- ------------------------------
	function CWolf:is_player_detected()
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

	
	function CWolf:is_fatigued()
		return self.fatigue <= 0
	end

	