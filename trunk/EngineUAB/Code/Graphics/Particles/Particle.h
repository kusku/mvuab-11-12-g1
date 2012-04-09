#pragma once

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Billboard\Billboard.h"
#include "Math\Vector3.h"

class CParticle : public CBillboard
{
public:
	CParticle();
	CParticle( float lifeTime, const Vect3f& direction );
	~CParticle();

	bool			Update				( float elapsedTime );

	void			SetLifeTime			( float time )			{ m_fLifeTime = time; }
	void			SetDirection		( const Vect3f &dir )	{ m_Direction = dir; }

	float			GetLifeTime			() const				{ return m_fLifeTime; }
	float			GetAge				() const				{ return m_fAge; }
	const Vect3f&	GetDirection		() const				{ return m_Direction; }

private:
	float		m_fLifeTime;
	float		m_fAge;
	Vect3f		m_Direction;
};


#endif