
#pragma once

#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H

#include "Base.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "Vertexs\VertexType.h"
#include <stdlib.h>
#include <string>
#include "Math\Vector3.h"

class CTexture;
class CEffectTechnique;
class CEffect;
struct TParticleSystemSettings;

class CParticleSystem
{
protected:
	std::string						m_Name;

	CTexture*						m_ParticleTexture;
	CEffectTechnique*				m_ParticleTechnique;
	CEffect*						m_Effect;
	D3DXHANDLE						m_ViewParam;
	D3DXHANDLE						m_ProjectionParam;
	D3DXHANDLE						m_PrevViewProjectionParam;
	D3DXHANDLE						m_InvertViewProjectionParam;
	D3DXHANDLE						m_CameraPositionParam;
	D3DXHANDLE						m_ViewPortScaleParam;
	D3DXHANDLE						m_CurrentTimeParam;
	D3DXHANDLE						m_DurationParam;
	D3DXHANDLE						m_DurationRandomnessParam;
	D3DXHANDLE						m_GravityParam;
	D3DXHANDLE						m_EndVelocityParam;
	D3DXHANDLE						m_MinColorParam;
	D3DXHANDLE						m_MaxColorParam;
	D3DXHANDLE						m_RotateSpeedParam;
	D3DXHANDLE						m_StartSizeParam;
	D3DXHANDLE						m_EndSizeParam;
	D3DXHANDLE						m_TextureParam;

	const TParticleSystemSettings*	m_Settings;

	TPARTICLE_VERTEX*				m_Particles;

	LPDIRECT3DVERTEXBUFFER9			m_VertexBuffer;
	LPDIRECT3DINDEXBUFFER9			m_IndexBuffer;

	uint32							m_FirstActiveParticle;
	uint32							m_FirstNewParticle;
	uint32							m_FirstFreeParticle;
	uint32							m_FirstRetiredParticle;

	float							m_CurrentTime;

	uint32							m_DrawCounter;

protected:
	void							RetireActiveParticles			();
	void							FreeRetiredParticles			();

	void							AddNewParticlesToVertexBuffer	();
	void							SetParamsParticleEffect			();

public:
	CParticleSystem(const std::string& name, const TParticleSystemSettings* settings);
	virtual ~CParticleSystem();

	void			Initialize			();
	void			Update				(float elapsedTime);
	void			Render				();
	void			ReloadEffect		();

	bool			AddParticle			(const Vect3f& position, const Vect3f& velocity);

	void			RefreshTexture		();	
};

#endif