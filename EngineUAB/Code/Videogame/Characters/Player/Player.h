#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Characters\Character.h"
#include "StatesMachine\State.h"
#include <map>
#include <string>

//--- Foward Declarations ---//
class CThPSCharacterCamera;
class CCharacter;
class CGameProcess;
//---------------------------//

class CPlayer : public CCharacter
{
public:
	// ------------- Constructors i Destructors --------------
	CPlayer();
	~CPlayer();

	//----Main Functions --------------------------------------
	bool			Init	();
	void			Update	( float _ElapsedTime );
	void			UpdateCamera( float _ElapsedTime, bool _bIsUpdated );

	//----Methods ---------------------------------------------
	void			HitToPlayer		( int _Strong );
	CCharacter*		DetectEnemy		();

	void			SetTargetFixed	( bool fix )		{ m_bIsTargetFixed = fix; }
	bool			IsTargetFixed	() const			{ return m_bIsTargetFixed; }
	void			BeDead			();

	void			ResetTimeDamage	()					{ m_fTimeWithoutDamage = 0.f; m_fTimeToIncreaseLife = 0.f; }
	
	bool			CallHitState	( CCharacter* _pCharacter, const STelegram& _Message );

	float			CalculateAttackYaw( float _fDetectionDistance, float _fDetectionAngle, CCharacter *_pTarget );

private: //----Private Methods --------------------------------------
	void			CreateCallbacks			();
	void			CreateStates			();
	bool			CalculateAngleMovement	( float &_fAngle );
	
	
private: //----Members ---------------------------------------------
	bool			m_bIsTargetFixed;

	// Camera
	float			m_fMovementZoom;
	float			m_fStaticZoom;
	float			m_fVelocityAdaptativeZoom;
	float			m_fDownZoom;

	// Life
	float			m_fTimeWithoutDamage;
	float			m_fTimeToIncreaseLife;

	// Detection
	float			m_fDistanceToDetectEnemy;
	float			m_fVisibilityAngle;

	CThPSCharacterCamera	*m_pCamera;
	CGameProcess			*m_pProcess;
};

#endif //_PLAYER_H_