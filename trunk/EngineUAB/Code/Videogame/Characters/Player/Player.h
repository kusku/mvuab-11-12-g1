#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Characters\Character.h"
#include "StatesMachine\State.h"
#include <map>
#include <string>

class CThPSCharacterCamera;

class CPlayer : public CCharacter
{
public:
	CPlayer();
	~CPlayer();

	bool			Init	();
	void			Update	( float _ElapsedTime );

	void			HitToPlayer		();
	CCharacter*		DetectEnemy		();

	void			SetTargetFixed	( bool fix )		{ m_bIsTargetFixed = fix; }
	bool			IsTargetFixed	() const			{ return m_bIsTargetFixed; }

private:
	void			CreateCallbacks	();
	void			CreateStates	();

private:
	bool			m_bIsTargetFixed;

	float			m_fMovementZoom;
	float			m_fStaticZoom;
	float			m_fVelocityAdaptativeZoom;
	float			m_fDownZoom;

	CThPSCharacterCamera *m_pCamera;
};

#endif //_PLAYER_H_