#pragma once
#ifndef __CLASS_PROPERTIES_H__
#define __CLASS_PROPERTIES_H__

#include <string>

#include "Utils\Active.h"
#include "Utils\Named.h"
#include "Math\Vector3.h"
#include "Object3D.h"

class CProperties : public CObject3D, public CActive
{
public:
	//--- Init and End protocols ----------------------------------------
						CProperties				( const std::string &_Name, int _Life = 0, int _Strong = 0, float _Speed = 0.f , 
												float _HeightController = 1.f, float _WidthController = 1.f, float _Slope  = 45.f, 
												float _SkinWidth  = 0.1f, float _StepOffset = 0.5, 
												const Vect3f &_Direction = Vect3f(0.f,0.f,0.f), const Vect3f &_Position = NULL );

						CProperties				( void );
						~CProperties			( void );

	//---- Main Methods --------------------------------------------------
	//---- Methods -------------------------------------------------------
	//---- Properties ( get & Set )---------------------------------------

	// -------------------- Popiedades Generales -----------------------------------------------------------------
	inline void					SetCore					( const std::string &_CoreName )					{ m_Core = _CoreName; }
	inline const std::string	GetCore					( void ) const										{ return m_Core; }

	inline void					SetAnimationInstance	( const std::string &_AnimationInstanceName )		{ m_AnimationInstanceName = _AnimationInstanceName; }
	inline std::string			GetAnimationInstance	( void ) const										{ return m_AnimationInstanceName; }

	inline void					SetLife					( int _Life )										{ m_Life = m_CurrentLife = _Life; }
	inline const int			GetLife					( void ) const										{ return m_Life; }

	inline void					SetCurrentLife			( int _Life )										{ m_CurrentLife = _Life; }
	inline const int			GetCurrentLife			( void ) const										{ return m_CurrentLife; }

	inline void					SetTimeToCure			( float _Time )										{ m_TimeToCure = _Time; }
	inline const float			GetTimeToCure			( void ) const										{ return m_TimeToCure; }

	inline void					SetWaitToCure			( float _Time	)									{ m_WaitToCure = _Time; }
	inline const float			GetWaitToCure			( void ) const										{ return m_WaitToCure; }

	inline void					SetCureVelocity			( int _Velocity	)									{ m_CureVelocity = _Velocity; }
	inline const int			GetCureVelocity			( void ) const										{ return m_CureVelocity; }

	inline void					SetStrong				( int _Strong )										{ m_Strong = _Strong; }
	inline  int					GetStrong				( void ) const										{ return m_Strong; }

	inline void					SetMass					( float _Mass )										{ m_Mass = _Mass; }
	inline const float			GetMass					( void ) const										{ return m_Mass; }

	inline void					SetRespawnPosition		( const Vect3f &_RespawnPosition )					{ m_RespawnPosition = _RespawnPosition; }
	inline const Vect3f			GetRespawnPosition		( void ) const										{ return m_RespawnPosition; }

	inline void					SetLocked				( bool _Locked )									{ m_Locked = _Locked; }
	inline const bool			GetLocked				( void ) const										{ return m_Locked; }


	// -------------------- Popiedades de detección para estados --------------------------------------------------------------
	inline void					SetDetectionDistance		( float _DetectionDistance )						{ m_DetectionDistance = _DetectionDistance; }
	inline float				GetDetectionDistance		( void ) const										{ return m_DetectionDistance; }
	
	inline void					SetChaseDistance			( float _ChaseDistance )							{ m_ChaseDistance = _ChaseDistance; }
	inline float				GetChaseDistance			( void ) const										{ return m_ChaseDistance; }

	inline void					SetFollowPathDistance		( float _FollowPathDistance )						{ m_FollowPathDistance = _FollowPathDistance; }
	inline float				GetFollowPathDistance		( void ) const										{ return m_FollowPathDistance; }

	inline void					SetPanicDistance			( float _PanicDistance )							{ m_PanicDistance = _PanicDistance; }
	inline float				GetPanicDistance			( void ) const										{ return m_PanicDistance; }

	inline void					SetTiredDistance			( float _TiredDistance )							{ m_TiredDistance = _TiredDistance; }
	inline float				GetTiredDistance			( void ) const										{ return m_TiredDistance; }

	inline void					SetImpactDistance			( float _ImpactDistance )							{ m_ImpactDistance = _ImpactDistance; }
	inline float				GetImpactDistance			( void ) const										{ return m_ImpactDistance; }
	
	inline void					SetAttackDistance			( float _AttackDistance )							{ m_AttackDistance = _AttackDistance; }
	inline float				GetAttackDistance			( void ) const										{ return m_AttackDistance; }
	
	inline void					SetAproximationDistance		( float _AproximationDistance )						{ m_AproximationDistance = _AproximationDistance; }
	inline float				GetAproximationDistance		( void ) const										{ return m_AproximationDistance; }
	
	inline void					SetPreparedAttackDistance	( float _PreparedAttackDistance )					{ m_PreparedAttackDistance = _PreparedAttackDistance; }
	inline float				GetPreparedAttackDistance	( void ) const										{ return m_PreparedAttackDistance; }

	// -------------------- Popiedades relacionadas con el controler o parte física o de animación ----------------------------
	inline void					SetHeightController		( float _Height )									{ m_HeightController = _Height; }
	inline const float			GetHeightController		( void ) const										{ return m_HeightController; }

	inline void					SetWidthController		( float _Width )									{ m_WidthController = _Width; }
	inline const float			GetWidthController		( void ) const										{ return m_WidthController; }

	inline void					SetSlopeController		( float _Slope )									{ m_Slope = _Slope; }
	inline const float			GetSlopeController		( void ) const										{ return m_Slope; }

	inline void					SetSkinWidthController	( float _SkinWidth )								{ m_SkinWidth = _SkinWidth; }
	inline const float			GetSkinWidthController	( void ) const										{ return m_SkinWidth; }

	inline void					SetStepOffsetController	( float _StepOffset )								{ m_StepOffset = _StepOffset; }
	inline const float			GetStepOffsetController	( void ) const										{ return m_StepOffset; }

	inline void					SetAnimationOffset		( float _AnimationOffset )							{ m_AnimationOffset = _AnimationOffset; }
	inline const float			GetAnimationOffset		( void ) const										{ return m_AnimationOffset; }

	// -------------------- Popiedades relacionadas con IA i FISICA ---------------------------------------------------------
	inline void					SetBoundingRadious		( float _BoundingRadius )							{ m_BoundingRadius = _BoundingRadius; }
	inline const float			GetBoundingRadious		( void ) const										{ return m_BoundingRadius; }

	inline void					SetVelocity				( const Vect3f &_Velocity )							{ m_Velocity = _Velocity; }
	inline const Vect3f			GetVelocity				( void ) const										{ return m_Velocity; }

	inline void					SetMaxRotationSpeed		( float _MaxRotationSpeed )							{ m_MaxRotationSpeed = _MaxRotationSpeed; }
	inline const float			GetMaxRotationSpeed		( void ) const										{ return m_MaxRotationSpeed; }

	inline void					SetSpeed				( float _Speed )									{ m_Speed = _Speed; }
	inline const float			GetSpeed				( void ) const										{ return m_Speed; }

	inline void					SetMaxSpeed				( float _MaxSpeed )									{ m_MaxSpeed = _MaxSpeed; }
	inline const float			GetMaxSpeed				( void ) const										{ return m_MaxSpeed; }

	inline void					SetMinSpeed				( float _MinSpeed )									{ m_MinSpeed = _MinSpeed; }
	inline const float			GetMinSpeed				( void ) const										{ return m_MinSpeed; }

	inline void					SetMinAcceleration		( float _MinAcceleration )							{ m_MinAcceleration = _MinAcceleration; }
	inline const float			GetMinAcceleration		( void ) const										{ return m_MinAcceleration; }

	inline void					SetMaxAcceleration		( float _MaxAcceleration )							{ m_MaxAcceleration  = _MaxAcceleration ; }
	inline const float			GetMaxAcceleration		( void ) const										{ return m_MaxAcceleration ; }

	inline void					SetAccelerationAttack1	( float _Acceleration )								{ m_AccelerationAttack1 = _Acceleration; }
	inline const float			GetAccelerationAttack1	( void ) const										{ return m_AccelerationAttack1; }

	inline void					SetAccelerationAttack2	( float _Acceleration )								{ m_AccelerationAttack2 = _Acceleration; }
	inline const float			GetAccelerationAttack2	( void ) const										{ return m_AccelerationAttack2; }

	inline void					SetAccelerationAttack3	( float _Acceleration )								{ m_AccelerationAttack3 = _Acceleration; }
	inline const float			GetAccelerationAttack3	( void ) const										{ return m_AccelerationAttack3; }

	inline void					SetMaxForce				( float _MaxForce )									{ m_MaxForce = _MaxForce; }
	inline const float			GetMaxForce				( void ) const										{ return m_MaxForce; }

	inline void					SetAttackAngle			( float _Angle )									{ m_AttackAngle = _Angle; }
	inline const float			GetAttackAngle			( void ) const										{ return m_AttackAngle; }

	//---- Members -------------------------------------------------------
private:
	std::string			m_Core;
	std::string			m_AnimationInstanceName;
	
	// Propiedades generales del personaje
	float				m_Mass;
	int					m_Life;
	int					m_CurrentLife;
	float				m_TimeToCure;
	float				m_WaitToCure;
	int					m_CureVelocity;
	int					m_Strong;
	Vect3f				m_RespawnPosition;
	
	bool				m_Locked;					// Dice que paralizo el caracter

	// Propiedades de IA + Física
	Vect3f				m_Velocity;
	float				m_BoundingRadius;
	float				m_Speed;					// Velocidad actual del personaje
	float				m_MinSpeed;					// Velocidad en modo caminar
	float				m_MaxSpeed;					// Velocidad máxima del personaje
	float				m_MinAcceleration;			// Aceleración mínima del personaje.		--> Aceleración = Fuerza / Masa
	float				m_MaxAcceleration;			// Aceleración máxima del personaje.		--> Aceleración = Fuerza / Masa
	float				m_AccelerationAttack1;		// Aceleración para el ataque 1.
	float				m_AccelerationAttack2;		// Aceleración para el ataque 2.
	float				m_AccelerationAttack3;		// Aceleración para el ataque 3.
	float				m_MaxRotationSpeed;			// Rotación máxima de giro del personaje
	float				m_MaxForce;					// Fuerza máxima aplicable al personaje.	--> Fuerza = Masa * Aceleración 
	float				m_AttackAngle;				// Ángulo para la detección de enemigos en grados.

	// Propiedades de distancias 
	float				m_DetectionDistance;		// distancia de detección de otros caràcteres
	float				m_ChaseDistance;			// distancia de persecución 
	float				m_FollowPathDistance;		// distancia de persecución a través de waypoints
	float				m_ImpactDistance;			// distancia de mínima de impacto donde indica que el enemigo está justo encima del player 
	float				m_AttackDistance;			// distancia de donde ya puede atacar
	float				m_AproximationDistance;		// distancia de aproximación de ataque
	float				m_PreparedAttackDistance;	// distancia en que evalua y se prepara para aproximarse y poder atacar
	float				m_PanicDistance;			// distancia que recorre en estado de pànico. Este para el estado de evade o flee
	float				m_TiredDistance;			// distancia de alejamiento del enemigo respecto al player cuando está cansado. como flee pero de cara al player

	// Propiedades del controller y animacion
	float				m_HeightController;			// altura del controller
	float				m_WidthController;			// anchura del controller
	float				m_Slope;					// Slope del controller
	float				m_SkinWidth;				// parámetro que da el ancho de una piel que envuelve el controler con la que hace colision. Recomendado 0.1f
	float				m_StepOffset;				// cantidad para subir un escalon
	float				m_AnimationOffset;			// Offset para restar la coordenada Y de la animación 

};

#endif __CLASS_PROPERTIES_H__