
-- Enum para mensajes 
SEND_MSG_IMMEDIATELY = 0
NO_ADDITIONAL_INFO = nil

Msg_Idle 	= 0		-- Repòs
Msg_Ready 	= 1		-- Llest
Msg_Sleep	= 2		-- Dormir
Msg_Attack	= 3 	-- Atacar
Msg_RunAway = 4 	-- Fugir
Msg_Patrol 	= 5		-- Patrullar
Msg_Pusuit 	= 6		-- Cercar
Msg_Roam 	= 7		-- Vagar
Msg_Evade	= 8		-- Evadir
Msg_Chase	= 9 	-- Persecució
	
function init_game_data()
	core:load_triggers()
	core:reload_steering_behaviors_settings()
	_soundM:play_event('Stop_All_Game')
	_soundM:play_event('Play_MUS_Level_1')
	_soundM:play_event('Waterfall', 'Play_EFX_Environment')
end

function presentation_init()
	local l_process = get_game_process()
	local l_rail_manager = core:get_rail_manager()
	
	l_rail_manager:set_rail('rail_01')
	l_rail_manager:set_object_to_rail( l_process:get_rail_object() )
	l_rail_manager:get_current_rail().velocity = 5.0
	l_rail_manager:start()
end

function presentation_control()
	local l_process = get_game_process()
	local l_rail_manager = core:get_rail_manager()
	
	if( l_rail_manager:is_rail_finished() ) then
		l_process:add_rail_counter()
		local l_rail = l_process:get_rail_counter()
		
		if( l_rail == 1 ) then
			l_rail_manager:set_rail('rail_02')
			l_rail_manager:set_object_to_rail( l_process:get_rail_object() )
			l_rail_manager:get_current_rail().velocity = 5.0
			l_rail_manager:start()
		elseif( l_rail == 2 ) then
			l_rail_manager:set_rail('rail_03')
			l_rail_manager:set_object_to_rail( l_process:get_rail_object() )
			l_rail_manager:get_current_rail().velocity = 4.0
			l_rail_manager:start()
		elseif( l_rail == 3 ) then
			l_rail_manager:set_rail('rail_04')
			l_rail_manager:set_object_to_rail( l_process:get_rail_object() )
			l_rail_manager:get_current_rail().velocity = 4.0
			l_rail_manager:start()
		elseif( l_rail == 4 ) then
			l_rail_manager:set_rail('rail_05')
			l_rail_manager:set_object_to_rail( l_process:get_rail_object() )
			l_rail_manager:get_current_rail().velocity = 3.5
			l_rail_manager:start()
		else
			l_process:set_intro_finish(true)	
		end
	end
end