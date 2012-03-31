#pragma once

#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Utils\RecyclingArray.h"
#include "Particle.h"
#include <string>

class CParticleEmitter
{
public: 
	CParticleEmitter();
	~CParticleEmitter();

	void		Update		( float elapsedTime );

	void				SetPosition				( const Vect3f &position )			{ m_Position = position; }
	void				SetStartSpawnDir		( const Vect3f &spawn )				{ m_StartSpawnDir = spawn; }
	void				SetEndSpawnDir			( const Vect3f &spawn )				{ m_EndSpawnDir = spawn; }
	void				SetStartColor			( const CColor &color )				{ m_StartColor = color; }
	void				SetEndColor				( const CColor &color )				{ m_EndColor = color; }
	void				SetMinEmitRate			( float emit )						{ m_fMinEmitRate = emit; }
	void				SetMaxEmitRate			( float emit )						{ m_fMaxEmitRate = emit; }
	void				SetMinSize				( float size )						{ m_fMinSize = size; }
	void				SetMaxSize				( float size )						{ m_fMaxSize = size; }
	void				SetNumNewPartsExcess	( float parts )						{ m_fNumNewPartsExcess = parts; }
	void				SetTexture				( const std::string texture )		{ m_Texture = texture; }

	const Vect3f&		GetPosition				() const							{ return m_Position; }
	const Vect3f&		GetStartSpawnDir		() const							{ return m_StartSpawnDir; }
	const Vect3f&		GetEndSpawnDir			() const							{ return m_EndSpawnDir; }
	const CColor&		GetStartColor			() const							{ return m_StartColor; }
	const CColor&		GetEndColor				() const							{ return m_EndColor; }
	float				GetMinEmitRate			() const							{ return m_fMinEmitRate; }
	float				GetMaxEmitRate			() const							{ return m_fMaxEmitRate; }
	float				GetMinSize				() const							{ return m_fMinSize; }
	float				GetMaxSize				() const							{ return m_fMaxSize; }
	float				GetNumNewPartsExcess	() const							{ return m_fNumNewPartsExcess; }
	const std::string&	GetTexture				() const							{ return m_Texture; }

private:
	Vect3f			m_Position;
	Vect3f			m_StartSpawnDir;
	Vect3f			m_EndSpawnDir;
	CColor			m_StartColor;
	CColor			m_EndColor;
	float			m_fMinEmitRate;
	float			m_fMaxEmitRate;
	float			m_fMinSize;
	float			m_fMaxSize;
	float			m_fNumNewPartsExcess;
	std::string		m_Texture;

	CRecyclingArray<CParticle>	m_Particles;
};

#endif