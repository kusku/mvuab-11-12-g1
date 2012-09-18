-- ------------------------------------------------------------------------
--	Clase CWolf
-- ------------------------------------------------------------------------
class 'CWolf' (CCharacter)
	
	-- ------------------------------
	--		Load States Methods
	-- ------------------------------
	function CWolf:load_graphic_states()
		-- print_logger(0, "CWolf::load_graphic_states->Loading animated states...")
		
		-- Idle State --
		self.animation_idle_state = CWolfAnimatedIdleState("wolf_idle_animation")
		if self.animation_idle_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado IDLE del lobo")
		end		
		
		-- -- Walk State --
		self.animation_walk_state = CWolfAnimatedWalkState("wolf_walk_animation")
		if self.animation_walk_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado WALK del lobo")
		end
		
		-- Attack Run State --
		self.animation_run_state = CWolfAnimatedRunState("wolf_run_animation")
		if self.animation_run_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado PURSUIT del lobo")
		end
		
		-- -- Still Attack State --
		self.animation_still_attack_state = CWolfAnimatedStillAttackState("wolf_still_attack_animation")
		if self.animation_still_attack_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado STILL ATTACK del lobo")
		end
		
		-- -- Run Attack State --
		self.animation_run_attack_state = CWolfAnimatedStillAttackState("wolf_still_attack_animation")
		if self.animation_run_attack_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado RUN ATTACK del lobo")
		end
		
		-- -- Hit State --
		self.animation_hit_state = CWolfAnimatedHitState("wolf_hit_animation")
		if self.animation_hit_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado HIT del lobo")
		end
		
		-- Defense State --
		self.animation_defense_state = CWolfAnimatedDefenseState("wolf_defense_animation")
		if self.animation_defense_state == nil then
			print_logger(2, "CWolf::load_graphic_states->Error al cargar un estado animado DEFENSE del lobo")
		end
		
		-- print_logger(0, "CWolf::load_graphic_states->Animated states loaded")
	end 
	
	function CWolf:load_logic_states()
		-- print_logger(1, "CWolf::load_logic_states->Loading logic states...")
		
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
		
		-- Attack Still State --
		self.still_attack_state = CWolfStillAttackState("wolf_still_attack")
		if self.still_attack_state == nil then
			print_logger(2, "CWolf::load_logic_states->Error al cargar un estado STILL ATTACK del lobo")
		end
		
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
		
		-- print_logger(1, "CWolf::load_logic_states->Logic states loaded")
	end	
	
	-- ------------------------------
	--			Constructors
	-- ------------------------------	
	function CWolf:__init( _id )
		CCharacter.__init( self, _id )
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		self.locked = false
		
		-- self.character_manager = get_game_process():get_character_manager()
		-- self.player = get_game_process():get_character_manager():get_player()
		
		if ( get_game_process():get_character_manager():get_player() == nil ) then
			print_logger ( 2, "CWolf::__init()->player is nil" )
		end
		
		-- Cargamos estados
		self:load_logic_states()
		self:load_graphic_states()
	end
	
	function CWolf:__init( _id, _name )
		CCharacter.__init( self, _id, _name )
		-- self.yaw = 0.0
		-- self.pitch = -math.pi / 8
		--self.roll = 0.0
		--self.position = Vect3f(3.0, 0.0, 0.0)
		--self.position2 = self.position
		self.locked = false
		
		-- self.character_manager = get_game_process():get_character_manager()
		-- self.player = get_game_process():get_character_manager():get_player()
			
		if ( get_game_process():get_character_manager():get_player() == nil ) then
			print_logger ( 2, "CWolf::__init()->player is nil" )
		end
		
		-- Cargamos los estados		
		self:load_logic_states()
		self:load_graphic_states()
	end
	
	function CWolf:init()
		local l_IsOk = false
		
		-- self:get_animation_model():clear_cycle( self:get_animation_id("run"), 0.3 )
		-- self:get_animation_model():clear_cycle( self:get_animation_id("attack_still"), 0.3 )
		-- self:get_animation_model():clear_cycle( self:get_animation_id("idle"), 0.3 )
		-- self:get_animation_model():clear_cycle( self:get_animation_id("walk"), 0.3 )
		-- self:get_animation_model():clear_cycle( self:get_animation_id("hit"), 0.3 )
			
		if (self.graphic_fsm ~= nil) then
			self.graphic_fsm .current_state = self.animation_hit_state 
			self.graphic_fsm :change_state(self.animation_idle_state)
			l_IsOk = true
		end
		
		if (self.logic_fsm  ~= nil) then
			self.logic_fsm .current_state = self.hit_state
			self.logic_fsm :change_state(self.idle_state)
			l_IsOk = l_IsOk and true
		end
		
		self.behaviors:add_behaviour( CSeek() )
		self.behaviors:add_behaviour( CFlee(self.properties.panic_distance) )
		self.behaviors:add_behaviour( CPursuit() )
		self.behaviors:add_behaviour( CEvade(self.properties.panic_distance) )
		self.behaviors:add_behaviour( CArrive() )
		self.behaviors:add_behaviour( CWander() )
		self.behaviors:add_behaviour( CCollisionAvoidance(5) )
		
		-- -- Flocking --
		self.behaviors:add_behaviour( CSeparation() )
		self.behaviors:add_behaviour( CAlignment() )
		self.behaviors:add_behaviour( CCohesion() )
		
		self.behaviors:seek_off()
		self.behaviors:flee_off()
		self.behaviors:arrive_off()
		self.behaviors:pursuit_off()
		self.behaviors:evade_off()
		self.behaviors:wander_off()
		self.behaviors:separation_off()
		self.behaviors:alignment_off()
		self.behaviors:cohesion_off()
		self.behaviors:collision_avoidance_off()
		self.behaviors:obstacle_wall_avoidance_off()
		
		return l_IsOk
	end
	
	-- ------------------------------
	--			Main Methods
	-- ------------------------------
	function CWolf:update_ia(_elapsed_time)
	
	end
	
	function CWolf:update(_elapsed_time)
		-- print_logger(0, "CWolf::update()->Actualizando enemigo...")
		-- Almacenamos el elapsed time pq después será impossible de guardar
		self.elapsed_time = _elapsed_time
		
		--Actualizamos los estados en caso de cambiar
		self.logic_fsm:update()
		self.graphic_fsm:update()
	end
	
	function CWolf:bedead()
	
	end
	
	-- ------------------------------
	--			Destructor
	-- ------------------------------
	function CWolf:__Finalize()
	
	end
	
	-- ------------------------------
	--			Test Method
	-- ------------------------------
	
	