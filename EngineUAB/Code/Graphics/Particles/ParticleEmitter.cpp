#include "ParticleEmitter.h"
#include "RenderManager.h"
#include "Utils\Types.h"
#include <time.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitter::CParticleEmitter()
	: m_Particles(50)
	, m_Position(Vect3f(0.f, 0.f, 0.f))
	, m_StartSpawnDir(Vect3f(1.f, 0.f, 0.f))
	, m_EndSpawnDir(Vect3f(0.f, 1.f, 0.f))
	, m_StartColor(colWHITE)
	, m_EndColor(colBLACK)
	, m_fMinEmitRate(0.1f)
	, m_fMaxEmitRate(10.f)
	, m_fMinSize(0.1f)
	, m_fMaxSize(1.f)
	, m_fNumNewPartsExcess(1.f)
	, m_Texture("")
{
	m_RandomNumber.SetSeed( static_cast<uint32>(time(NULL)) );
}

CParticleEmitter::~CParticleEmitter()
{
}

void CParticleEmitter::Update(float elapsedTime)
{
	float fEmitRateThisFrame = m_RandomNumber( m_fMaxEmitRate, m_fMinEmitRate );
	int iNumNewParts = static_cast<int>( fEmitRateThisFrame * elapsedTime );
	m_fNumNewPartsExcess += (float)(fEmitRateThisFrame * elapsedTime)-iNumNewParts;

	if (m_fNumNewPartsExcess > 1.0f)
	{
		iNumNewParts += (int)m_fNumNewPartsExcess;
		m_fNumNewPartsExcess -= (int)m_fNumNewPartsExcess;
	}

	for (int q=0; q < iNumNewParts; ++q)
	{
		// Si hay espacio para una nueva partícula:
		CParticle *l_pParticle = m_Particles.New();

		// determine a random vector between dir1 and dir2
		float fRandX = m_RandomNumber( m_StartSpawnDir.x, m_EndSpawnDir.x );
		float fRandY = m_RandomNumber( m_StartSpawnDir.y, m_EndSpawnDir.y );
		float fRandZ = m_RandomNumber( m_StartSpawnDir.z, m_EndSpawnDir.z );

		l_pParticle->SetDirection( Vect3f(fRandX, fRandY, fRandZ) );
		l_pParticle->SetPosition( m_Position );

		float fRandR = m_RandomNumber( m_StartColor.GetRed(), m_EndColor.GetRed() );
		float fRandG = m_RandomNumber( m_StartColor.GetGreen(), m_EndColor.GetGreen() );
		float fRandB = m_RandomNumber( m_StartColor.GetBlue(), m_EndColor.GetBlue() );
		float fRandA = m_RandomNumber( m_StartColor.GetAlpha(), m_EndColor.GetAlpha() );

		l_pParticle->SetColor( CColor(fRandR, fRandG, fRandB, fRandA) );	
	}
}

void CParticleEmitter::Render(CRenderManager &RM)
{
	uint32 l_TotalElems = m_Particles.GetTotalElements();

	for(uint32 i=0; i<l_TotalElems; ++i)
	{
		if( !m_Particles.IsFree(i) )
		{
			CParticle *l_pParticle = m_Particles.GetAt(i);
			l_pParticle->Render(RM);
		}
	}
}