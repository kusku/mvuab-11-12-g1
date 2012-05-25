
#include <assert.h>

#include "ParticleSystem.h"
#include "Textures\Texture.h"
#include "Math\Vector2.h"
#include "Core.h"
#include "RenderManager.h"
#include "Utils\BoostRandomHelper.h"
#include "Cameras\Camera.h"
#include "Effects\EffectManager.h"
#include "Math\Color.h"
#include "Effects\Effect.h"
#include "Math\Matrix44.h"
#include "ParticleDef.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleSystem::CParticleSystem(const std::string& name, const TParticleSystemSettings* settings)
	: m_Name(name)
	, m_VertexBuffer(NULL)
	, m_IndexBuffer(NULL)
	, m_FirstActiveParticle(0)
	, m_FirstNewParticle(0)
	, m_FirstFreeParticle(0)
	, m_FirstRetiredParticle(0)
	, m_Particles(NULL)
	, m_Settings(settings)
	, m_CurrentTime(0.0f)
	, m_DrawCounter(0)
	, m_ParticeTexture(NULL)
	, m_ViewParam(NULL)
	, m_ProjectionParam(NULL)
	, m_ViewPortScaleParam(NULL)
	, m_CurrentTimeParam(NULL)
	, m_DurationParam(NULL)
	, m_DurationRandomnessParam(NULL)
	, m_GravityParam(NULL)
	, m_EndVelocityParam(NULL)
	, m_MinColorParam(NULL)
	, m_MaxColorParam(NULL)
	, m_RotateSpeedParam(NULL)
	, m_StartSizeParam(NULL)
	, m_EndSizeParam(NULL)
	, m_TextureParam(NULL)
	, m_Effect(NULL)
{
	assert(settings);
}

CParticleSystem::~CParticleSystem()
{
	CHECKED_RELEASE(m_VertexBuffer);
	CHECKED_RELEASE(m_IndexBuffer);
	CHECKED_DELETE_ARRAY(m_Particles);
	CHECKED_DELETE(m_ParticeTexture);
}

void CParticleSystem::Initialize()
{
	uint32 length = 0;
	void* data = NULL;

	//Get Effect
	m_ParticleTechnique = CORE->GetEffectManager()->GetEffectTechnique("ParticleTechnique");
	assert(m_ParticleTechnique);

	//Get Parameters
	m_Effect = CORE->GetEffectManager()->GetEffect("ParticleEffect");

	m_Effect->GetParameterBySemantic("CAMERA_VIEW", m_ViewParam);
	m_Effect->GetParameterBySemantic("CAMERA_PROJECTION", m_ProjectionParam);
	m_Effect->GetParameterBySemantic("VIEWPORT_SCALE", m_ViewPortScaleParam);
	m_Effect->GetParameterBySemantic("CURRENT_TIME", m_CurrentTimeParam);
	m_Effect->GetParameterBySemantic("DURATION", m_DurationParam);
	m_Effect->GetParameterBySemantic("DURATION_RANDOMNESS", m_DurationRandomnessParam);
	m_Effect->GetParameterBySemantic("GRAVITY", m_GravityParam);
	m_Effect->GetParameterBySemantic("END_VELOCITY", m_EndVelocityParam);
	m_Effect->GetParameterBySemantic("MIN_COLOR", m_MinColorParam);
	m_Effect->GetParameterBySemantic("MAX_COLOR", m_MaxColorParam);
	m_Effect->GetParameterBySemantic("ROTATE_SPEED", m_RotateSpeedParam);
	m_Effect->GetParameterBySemantic("START_SIZE", m_StartSizeParam);
	m_Effect->GetParameterBySemantic("END_SIZE", m_EndSizeParam);
	m_Effect->GetParameterBySemantic("PARTICLE_TEXTURE", m_TextureParam);

	//Load Texture
	m_ParticeTexture = new CTexture();
	m_ParticeTexture->Load(m_Settings->m_TextureName);

	//Create Particles
	m_Particles = new TPARTICLE_VERTEX[m_Settings->m_MaxParticles * 4];
	memset(m_Particles, 0, sizeof(TPARTICLE_VERTEX) * m_Settings->m_MaxParticles * 4);

	for(uint32 i = 0; i < m_Settings->m_MaxParticles; ++i)
	{
		m_Particles[i * 4 + 0].m_Corner_X = -1;
		m_Particles[i * 4 + 0].m_Corner_Y = -1;

		m_Particles[i * 4 + 1].m_Corner_X = 1;
		m_Particles[i * 4 + 1].m_Corner_Y = -1;

		m_Particles[i * 4 + 2].m_Corner_X = 1;
		m_Particles[i * 4 + 2].m_Corner_Y = 1;

		m_Particles[i * 4 + 3].m_Corner_X = -1;
		m_Particles[i * 4 + 3].m_Corner_Y = 1;
	}

	LPDIRECT3DDEVICE9 dx9Device = CORE->GetRenderManager()->GetDevice();
	
	//Set VB to VRAM
	DWORD usageVB = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
	length = sizeof(TPARTICLE_VERTEX) * m_Settings->m_MaxParticles * 4;
	dx9Device->CreateVertexBuffer(length, usageVB, 0, D3DPOOL_DEFAULT, &m_VertexBuffer, NULL);
	m_VertexBuffer->Lock(0, length, &data, 0);
	memcpy(data, m_Particles, length);
	m_VertexBuffer->Unlock();

	//Create Indices
	uint16* indices = new uint16[m_Settings->m_MaxParticles * 6];
	memset(indices, 0, sizeof(uint16) * m_Settings->m_MaxParticles * 6);
	for (uint16 i = 0; i < m_Settings->m_MaxParticles; ++i)
	{
		indices[i * 6 + 0] = (i * 4 + 0);
		indices[i * 6 + 1] = (i * 4 + 1);
		indices[i * 6 + 2] = (i * 4 + 2);

		indices[i * 6 + 3] = (i * 4 + 0);
		indices[i * 6 + 4] = (i * 4 + 2);
		indices[i * 6 + 5] = (i * 4 + 3);
	}

	//Set IB to VRAMok
	length = sizeof(uint16) * m_Settings->m_MaxParticles * 6;
	dx9Device->CreateIndexBuffer(length, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IndexBuffer, NULL);
	m_IndexBuffer->Lock(0, length, &data, 0);
	memcpy(data, indices, length);
	m_IndexBuffer->Unlock();

	CHECKED_DELETE_ARRAY(indices);
}

void CParticleSystem::SetParamsParticleEffect()
{
	m_Effect->GetD3DEffect()->SetFloat(m_DurationParam, m_Settings->m_Duration);
	m_Effect->GetD3DEffect()->SetFloat(m_DurationRandomnessParam, m_Settings->m_DurationRandomness);
	m_Effect->GetD3DEffect()->SetFloatArray(m_GravityParam, (float*)&m_Settings->m_Gravity, 3);
	m_Effect->GetD3DEffect()->SetFloat(m_EndVelocityParam, m_Settings->m_EndVelocity);
	m_Effect->GetD3DEffect()->SetInt(m_MinColorParam, m_Settings->m_MinColor.GetUint32Argb());
	m_Effect->GetD3DEffect()->SetInt(m_MaxColorParam, m_Settings->m_MaxColor.GetUint32Argb());
	m_Effect->GetD3DEffect()->SetTexture(m_TextureParam, m_ParticeTexture->GetDXTexture());

	float startSize[] = { m_Settings->m_MinStartSize, m_Settings->m_MaxStartSize};
	float endSize[] = { m_Settings->m_MinEndSize, m_Settings->m_MaxEndSize};
	float rotateSpeed[] = { m_Settings->m_MinRotateSpeed, m_Settings->m_MaxRotateSpeed};

	m_Effect->GetD3DEffect()->SetFloatArray(m_RotateSpeedParam, rotateSpeed, 2);
	m_Effect->GetD3DEffect()->SetFloatArray(m_StartSizeParam, startSize, 2);
	m_Effect->GetD3DEffect()->SetFloatArray(m_EndSizeParam, endSize, 2);

	m_Effect->GetD3DEffect()->SetMatrix(m_ViewParam, &CORE->GetCamera()->GetViewMatrix().GetD3DXMatrix());
	m_Effect->GetD3DEffect()->SetMatrix(m_ProjectionParam, &CORE->GetCamera()->GetProjectionMatrix().GetD3DXMatrix());
	m_Effect->GetD3DEffect()->SetFloat(m_CurrentTimeParam, m_CurrentTime);

	Vect2f viewportScale(0.5f / CORE->GetCamera()->GetAspectRatio(), -0.5);
	m_Effect->GetD3DEffect()->SetFloatArray(m_ViewPortScaleParam, (float*)(&viewportScale), 2);
}

void CParticleSystem::Update(float elapsedTime)
{
	m_CurrentTime += elapsedTime;

	RetireActiveParticles();
	FreeRetiredParticles();

	if (m_FirstActiveParticle == m_FirstFreeParticle)
	{
		m_CurrentTime = 0.0f;
	}

	if (m_FirstRetiredParticle == m_FirstActiveParticle)
	{
		m_DrawCounter = 0;
	}
}

void CParticleSystem::Render()
{
	LPDIRECT3DDEVICE9 dx9Device = CORE->GetRenderManager()->GetDevice();

	if (m_FirstNewParticle != m_FirstFreeParticle)
	{
		AddNewParticlesToVertexBuffer();
	}

	if (m_FirstActiveParticle != m_FirstFreeParticle)
	{
		CORE->GetRenderManager()->SetGraphicBlendState(m_Settings->m_BlendState);

		dx9Device->SetRenderState(D3DRS_ZWRITEENABLE, false);

		SetParamsParticleEffect();
		
		dx9Device->SetStreamSource(0, m_VertexBuffer, 0, sizeof(TPARTICLE_VERTEX));
		dx9Device->SetVertexDeclaration(TPARTICLE_VERTEX::GetVertexDeclaration());
		dx9Device->SetIndices(m_IndexBuffer);

		UINT passes = 0;
		m_Effect->GetD3DEffect()->Begin(&passes, 0);

		for(UINT i = 0; i < passes; ++i)
		{
			m_Effect->GetD3DEffect()->BeginPass(i);

			if (m_FirstActiveParticle < m_FirstFreeParticle)
			{
				dx9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,  
					m_FirstActiveParticle * 4, (m_FirstFreeParticle - m_FirstActiveParticle) * 4,
					m_FirstActiveParticle * 6, (m_FirstFreeParticle - m_FirstActiveParticle) * 2);
			}
			else
			{
				dx9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,  
					m_FirstActiveParticle * 4, (m_Settings->m_MaxParticles - m_FirstActiveParticle) * 4,
					m_FirstActiveParticle * 6, (m_Settings->m_MaxParticles - m_FirstActiveParticle) * 2);

				if (m_FirstFreeParticle > 0)
				{
					dx9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,  
						0, m_FirstFreeParticle * 4,
						0, m_FirstFreeParticle * 2);
				}
			}

			m_Effect->GetD3DEffect()->EndPass();
		}

		m_Effect->GetD3DEffect()->End();

		dx9Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

		CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::DefaultState);
	}

	m_DrawCounter++;
}

void CParticleSystem::RetireActiveParticles()
{
	float particleDuration = m_Settings->m_Duration;

	while (m_FirstActiveParticle != m_FirstNewParticle)
	{
		float particleAge = m_CurrentTime - m_Particles[m_FirstActiveParticle * 4].m_Time;

		if (particleAge < particleDuration)
		{
			break;
		}

		m_Particles[m_FirstActiveParticle * 4].m_Time = (float)m_DrawCounter;

		m_FirstActiveParticle++;

		if (m_FirstActiveParticle >= m_Settings->m_MaxParticles)
		{
			m_FirstActiveParticle = 0;
		}
	}
}

void CParticleSystem::FreeRetiredParticles()
{
	while (m_FirstRetiredParticle != m_FirstActiveParticle)
	{
		int age = m_DrawCounter - (int32)m_Particles[m_FirstRetiredParticle * 4].m_Time;

		if (age < 3)
		{
			break;
		}

		m_FirstRetiredParticle++;

		if(m_FirstRetiredParticle >= m_Settings->m_MaxParticles)
		{
			m_FirstRetiredParticle = 0;
		}
	}
}

bool CParticleSystem::AddParticle(const Vect3f& position, const Vect3f& velocity)
{
	uint32 nextFreeParticle = m_FirstFreeParticle + 1;

	if (nextFreeParticle >= m_Settings->m_MaxParticles)
	{
		nextFreeParticle = 0;
	}

	if (nextFreeParticle == m_FirstRetiredParticle)
	{
		return false;
	}

	Vect3f newVelocity = velocity * m_Settings->m_EmitterVelocitySensitivity;

	float horizontalVelocity = mathUtils::Lerp<float>(m_Settings->m_MinHorizontalVelocity, m_Settings->m_MaxHorizontalVelocity, BoostRandomHelper::GetNextFloat());

	float horizontalAngle = BoostRandomHelper::GetNextFloat() * D3DX_PI;

	newVelocity.x += horizontalVelocity * mathUtils::Cos<float>(horizontalAngle);
	newVelocity.z += horizontalVelocity * mathUtils::Sin<float>(horizontalAngle);

	newVelocity.y += mathUtils::Lerp<float>(m_Settings->m_MinVerticalVelocity, m_Settings->m_MaxVerticalVelocity, BoostRandomHelper::GetNextFloat());

	CColor randomValues(
		BoostRandomHelper::GetNextFloat(),
		BoostRandomHelper::GetNextFloat(),
		BoostRandomHelper::GetNextFloat(),
		BoostRandomHelper::GetNextFloat()
		);

	for (uint32 i = 0; i < 4; ++i)
	{
		//Set position
		m_Particles[m_FirstFreeParticle * 4 + i].m_x = position.x;
		m_Particles[m_FirstFreeParticle * 4 + i].m_y = position.y;
		m_Particles[m_FirstFreeParticle * 4 + i].m_z = position.z;

		//Set Velocity
		m_Particles[m_FirstFreeParticle * 4 + i].m_v_x = newVelocity.x;
		m_Particles[m_FirstFreeParticle * 4 + i].m_v_y = newVelocity.y;
		m_Particles[m_FirstFreeParticle * 4 + i].m_v_z = newVelocity.z;

		//Set Random
		m_Particles[m_FirstFreeParticle * 4 + i].m_RandomColorRGBA = randomValues.GetUint32Argb();

		//Set Time
		m_Particles[m_FirstFreeParticle * 4 + i].m_Time = m_CurrentTime;
	}

	m_FirstFreeParticle = nextFreeParticle;

	return true;
}

void CParticleSystem::AddNewParticlesToVertexBuffer()
{
	TPARTICLE_VERTEX* data = NULL;

	if (m_FirstNewParticle < m_FirstFreeParticle)
	{
		m_VertexBuffer->Lock(0, m_Settings->m_MaxParticles * 4, (void**)&data, D3DLOCK_NOOVERWRITE);
		memcpy(data + (m_FirstNewParticle * 4), m_Particles + (m_FirstNewParticle * 4), sizeof(TPARTICLE_VERTEX) * ((m_FirstFreeParticle - m_FirstNewParticle) * 4));
		m_VertexBuffer->Unlock();
	}
	else
	{

		m_VertexBuffer->Lock(0, m_Settings->m_MaxParticles * 4, (void**)&data, D3DLOCK_NOOVERWRITE);
		memcpy(data + (m_FirstNewParticle * 4), m_Particles + (m_FirstNewParticle * 4), sizeof(TPARTICLE_VERTEX) * ((m_Settings->m_MaxParticles - m_FirstNewParticle) * 4));
		

		if (m_FirstFreeParticle > 0)
		{
			memcpy(data + (0), m_Particles + (0), sizeof(TPARTICLE_VERTEX) * (m_FirstNewParticle * 4));
		}

		m_VertexBuffer->Unlock();
	}

	// Move the particles we just uploaded from the new to the active queue.
	m_FirstNewParticle = m_FirstFreeParticle;
}