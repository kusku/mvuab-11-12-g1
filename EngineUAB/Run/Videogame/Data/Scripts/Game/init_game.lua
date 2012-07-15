
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
end

