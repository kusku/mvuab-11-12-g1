#pragma once

#ifndef __STATES_DEFS_H__
#define __STATES_DEFS_H__


// ********* ESTADOS ANIMADOS RABBIT *********** 
#define RABBIT_IDLE_STATE					"conejo_idle"
#define RABBIT_IDLE2_STATE					"conejo_idle2"
#define RABBIT_RUN_STATE					"conejo_run"
#define RABBIT_DEATH_STATE					"conejo_death"
#define RABBIT_DEFENSE_STATE				"conejo_defense"
#define RABBIT_HIT_STATE					"conejo_hit"
#define RABBIT_JUMP_STATE					"conejo_jump"
#define RABBIT_RIGHT_LATERAL_STATE			"conejo_right_lateral"
#define RABBIT_LEFT_LATERAL_STATE			"conejo_left_lateral"
#define	RABBIT_ATTACK_STATE					"Conejo_attack"
#define RABBIT_STILL_ATTACK_STATE			"conejo_attack_1"
#define RABBIT_RUN_ATTACK_STATE				"conejo_attack_2"
#define RABBIT_WALK_STATE					"conejo_walk"

// ********* ESTADOS ANIMADOS RABBIT *********** 
#define WOLF_IDLE_STATE						"lobo_idle"
#define WOLF_IDLE2_STATE					"lobo_idle2"
#define WOLF_RUN_STATE						"lobo_run"
#define WOLF_DEATH_STATE					"lobo_death"
#define WOLF_DEFENSE_STATE					"lobo_defense"
#define WOLF_HIT_STATE						"lobo_hit"
#define WOLF_JUMP_STATE						"lobo_jump"
#define WOLF_RIGHT_LATERAL_STATE			"lobo_right_lateral"
#define WOLF_LEFT_LATERAL_STATE				"lobo_left_lateral"
#define WOLF_STILL_ATTACK_STATE				"lobo_attack_still"
#define WOLF_STILL_ATTACK_COMBO_STATE		"lobo_attack_still_combo"
#define WOLF_RUN_ATTACK_STATE				"lobo_attack_run_all"
#define WOLF_ONLY_RUN_ATTACK_STATE			"lobo_attack_run_1"
#define WOLF_ONLY_IMPACT_RUN_ATTACK_STATE	"lobo_attack_run_2"
#define WOLF_WALK_STATE						"lobo_walk"
#define WOLF_HOWL_STATE						"lobo_howl"

// ********* ESTADOS ANIMADOS DEER *********** 
#define DEER_IDLE_STATE						"ciervo_idle"
#define DEER_IDLE2_STATE					"ciervo_idle2"
#define DEER_RUN_STATE						"ciervo_run"
#define DEER_DEATH_STATE					"ciervo_death"
#define DEER_DEFENSE_STATE					"ciervo_defense"
#define DEER_HIT_STATE						"ciervo_hit"
#define DEER_JUMP_STATE						"ciervo_jump"
#define DEER_RIGHT_LATERAL_STATE			"ciervo_right_lateral"
#define DEER_LEFT_LATERAL_STATE				"ciervo_left_lateral"
#define DEER_STILL_ATTACK_STATE				"ciervo_attack_1"
#define DEER_RUN_ATTACK_STATE				"ciervo_attack_2"
#define DEER_WALK_STATE						"ciervo_walk"

struct sDireccion
{
	Vect3f	Direccion;		// o paso la dirección
	float	Speed;			// o paso la velocidad 
	float	ElapsedTime;
};

#endif __STATES_DEFS_H__