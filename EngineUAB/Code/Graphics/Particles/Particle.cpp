#include "Particle.h"
#include "Cameras\Camera.h" 
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableObjectTechnique.h"

#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"
#include <d3d9.h>
#include <string>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CParticle::CParticle(void)
	: m_vDir				( Vect3f(0.f,0.f,0.f) )
	, m_fAge				( 0.f )
	, m_fLifetime			( 10.f )
	, m_vColor				( CColor(1.f, 1.f, 1.f, 1.f ) )
	, m_iCurrentCoreIndex	( 0 )
	, m_bAlive				( true )
	, m_fCurrentCoreTime	( 0.f )
{}

CParticle::~CParticle(void)
{
	Destroy();
}


// -----------------------------------------
//			   MÈTODES PRINCIPALS
// -----------------------------------------
void CParticle::Destroy ( void )
{
	CBillboard::Destroy();
}

void CParticle::Render ( CRenderManager &_RM, const CColor &_Color )
{
	CBillboard::Render( _RM, _Color );
}

bool CParticle::Update ( float _ElapsedTime )
{
	m_fAge += _ElapsedTime;
	
	// Si la vida ja li ha finalitzat diem que s'ha mort
	if ( !IsALive() ) 
	{
		SetIsAlive( false );
		return false;
	}

	// En altra cas actualitzem tots els paràmetre de la partícular per la seva visualització
	Vect3f l_Pos = GetPosition();
	CBillboard::SetPosition( l_Pos + m_vDir * _ElapsedTime );
	CBillboard::Update( CORE->GetCamera(), 0.f );

	return true;
}
	
// -----------------------------------------
//				MÈTODES PRIVATS
// -----------------------------------------

// -----------------------------------------
//				MÈTODES PUBLICS
// -----------------------------------------

// -----------------------------------------
//				PROPIEDADES
// -----------------------------------------
