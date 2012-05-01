 #include "ParticlesEmitterInstance.h"

#include "ParticlesSystemManager.h"
#include "ParticlesEmitterCore.h"
#include "ParticlesEmitterProperties.h"
#include "ParticleDef.h"
#include "Textures\TextureManager.h"
#include "Textures\Texture.h"

#include "Utils\TemplatedVectorMapManager.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CParticlesEmitterInstance::CParticlesEmitterInstance( CXMLTreeNode &_Node )
	: m_pCore						( NULL )
	, m_Particles					( _Node.GetIntProperty( "total_particulas", 1 ) )
	, m_bInstanceAlive				( false )	
{
	SetName( _Node.GetPszProperty( "name", "" ) );
	std::string l_NomCore = _Node.GetPszProperty( "core", "" );

	m_pCore = CORE->GetParticlesManager()->GetParticlesEmitterCore( l_NomCore );
	if ( !m_pCore )
		return;
	
	SetPosition1( _Node.GetVect3fProperty( "pos1", Vect3f(0.f,0.f,0.f) ) );
	SetPosition2( _Node.GetVect3fProperty( "pos2", Vect3f(0.f,0.f,0.f) ) );

	Init();
}

CParticlesEmitterInstance::~CParticlesEmitterInstance( void )
{
	Destroy();
}


// -----------------------------------------
//			   MÈTODES PRINCIPALS
// -----------------------------------------
void CParticlesEmitterInstance::Destroy ( void )
{
	m_Particles.Done();
	m_pCore = NULL;
}

void CParticlesEmitterInstance::Init ( void )
{
	for ( uint32 l_uiIndex = 0; l_uiIndex < m_Particles.GetTotalElements(); ++l_uiIndex )
	{
		CreateParticle();
	}
}

void CParticlesEmitterInstance::Render ( CRenderManager &_RM, const CColor &_Color )
{
	for ( uint32 l_uiIndex = 0; l_uiIndex < m_Particles.GetTotalElements(); ++l_uiIndex )
	{
		if ( !m_Particles.IsFree( l_uiIndex ) )
		{
			CParticle* l_Particle =  m_Particles.GetAt( l_uiIndex );
			if ( l_Particle->IsALive ( ) )
				l_Particle->Render ( _RM, _Color );
		}
	}
}

void CParticlesEmitterInstance::Update ( float _ElapsedTime )
{
	for ( uint32 l_uiIndex = 0; l_uiIndex < m_Particles.GetTotalElements(); ++l_uiIndex )
	{
		// Només si existeix la patícula dins l'array cal actualitzar. En altra cas serà lliure per crear una partícula nova
		if ( !m_Particles.IsFree( l_uiIndex ) )
		{
			CParticle* l_Particle =  m_Particles.GetAt( l_uiIndex );

			// Si el index que té la partícula és l'últim mantenim aquests paràmetres ja que el següent estat és en estat de desaparaixer s
			if ( l_Particle->GetCoreIndex() != m_pCore->GetResourcesVector().size() -1 )
			{
				float l_CurrentLife = l_Particle->GetAge() + _ElapsedTime;
	
				// versió 1
				//UpdateLocalParameters ( l_CurrentLife );
				
				// Pillo el índex del core de la partícula
				int l_CurrentCoreIndex = l_Particle->GetCoreIndex();
				int l_NewCoreIndex = l_CurrentCoreIndex;

				// Cerco els nous paràmetres segons el índex i basat en el temps
				l_CurrentLife = SearchTimeBasedAge ( l_CurrentLife, l_NewCoreIndex );

				// Si el core ha canviat assignarem els nous paràmetres
				if ( l_CurrentCoreIndex != l_NewCoreIndex )
				{
					l_Particle->SetCoreIndex( l_NewCoreIndex );
					UpdateValuesByIndex ( l_NewCoreIndex );

					// Aquí recalculo els nou valors que tindrá la partícular basat en els rangs proposats per el CORE actual segons el temps
					UpdateParticlesParameters ( l_Particle );
				}

				// versió 2
				// Actualitzo els valors locals segons el temps. Això permet cercar el CORE amb les propietats adequades en aquest temps
				//UpdateValuesByTime ( l_CurrentLife );
				
				// Aquí recalculo els nou valors que tindrá la partícular basat en els rangs proposats per el CORE actual segons el temps
				//UpdateParticlesParameters ( l_Particle );
			}
			
			//if ( l_Particle->IsALive ( ) )
			// A dins l'update ja mirem si és alive.
			if ( !l_Particle->Update( _ElapsedTime ) )
			{
				m_Particles.Free( l_uiIndex );
			}
		}
	}
	CreateParticles( _ElapsedTime );
}
	
// -----------------------------------------
//				MÈTODES PRIVATS
// -----------------------------------------
// crea noves partícules a Temps 0
void CParticlesEmitterInstance::CreateParticle ( void )
{
	// Actualitzo els valors locals segons el temps. Això permet cercar el CORE amb les propietats adequades en aquest temps
	UpdateValuesByTime ( TIME_CERO );

	// Si hay espacio para una nueva partícula:
	CParticle *l_Part = m_Particles.New();
	if ( l_Part )
		AssignInitialParameters ( l_Part );
}

// Comprova que hi hagi espai i crea noves partícules
void CParticlesEmitterInstance::CreateParticles ( float _ElapsedTime )
{
	float fEmitRateThisFrame = RandomFloat( m_fMinEmitRate, m_fMaxEmitRate );
	int iNumNewParts = static_cast<int> ( fEmitRateThisFrame * _ElapsedTime );
	m_fNumNewPartsExcess += (float) ( fEmitRateThisFrame * _ElapsedTime ) - iNumNewParts;
	if ( m_fNumNewPartsExcess > 1.0f )
	{
		iNumNewParts += (int) m_fNumNewPartsExcess;
		m_fNumNewPartsExcess -= (int) m_fNumNewPartsExcess;
	}

	for ( int q = 0; q < iNumNewParts; ++q )
	{
		// Si hay espacio para una nueva partícula la creamos
		CreateParticle();
	}
}

// Calcula els nous valors i els assigna a la partícula passada
void CParticlesEmitterInstance::AssignInitialParameters ( CParticle *l_Particle )
{
	// Calculem el valor de cada una de les variables que tindrá l'emissor
	float		l_fCurrentLifeTime			= RandomFloat( m_fLifeTime1, m_fLifeTime2 );
	Vect3f		l_vCurrentVelocity			= m_vVelocity;
	float		l_fCurrentAngle				= m_fAngle;
	float		l_fCurrentEmitRate			= RandomFloat( m_fMinEmitRate, m_fMaxEmitRate );		
	float		l_fCurrentSize				= RandomFloat( m_fMinSize, m_fMaxSize );
	bool		l_bCurrentAlive				= true;
	std::string	l_sCurrentTexture			= m_sTexture;
	float		l_fCurrentTime				= TIME_CERO;
		
	// Determino la nova posició, direcció, color
	Vect3f		l_vCurrentPosition			= RandomVector ( m_vPosition1, m_vPosition2 );
	Vect3f		l_vCurrentSpawnDir			= RandomVector ( m_vSpawnDir1, m_vSpawnDir2 );
	CColor		l_CCurrentColor				= RandomColor  ( m_Color1, m_Color2 );
		
	// Calculem la direcció de la partícula segons la velocitat
	//l_vCurrentSpawnDir = l_vCurrentSpawnDir.Normalize(); // + l_vCurrentVelocity;

	// Assignem a cada partícula els valor calculats
	l_Particle->SetDirection( l_vCurrentSpawnDir );
	l_Particle->SetPosition	( l_vCurrentPosition );
	l_Particle->SetColor	( l_CCurrentColor );
	l_Particle->SetVelocity	( l_vCurrentVelocity );
	//l_Particle->setAngle	( l_fCurrentAngle );
	l_Particle->SetLifeTime ( l_fCurrentLifeTime );
	l_Particle->SetAge		( l_fCurrentTime );
	l_Particle->SetIsAlive  ( l_bCurrentAlive );
	l_Particle->SetTexture  ( m_Texture );
	l_Particle->SetWidth	( l_fCurrentSize );
	l_Particle->SetHeight	( l_fCurrentSize );

	assert( m_Texture != NULL );
}

// Calcula els nous valors i els assigna a la partícula passada
void CParticlesEmitterInstance::UpdateParticlesParameters ( CParticle *l_Particle )
{
	// Calculem el valor de cada una de les variables que tindrá l'emissor
	Vect3f		l_vCurrentVelocity			= m_vVelocity;
	float		l_fCurrentAngle				= m_fAngle;
	float		l_fCurrentEmitRate			= RandomFloat( m_fMinEmitRate, m_fMaxEmitRate );		
	float		l_fCurrentSize				= RandomFloat( m_fMinSize, m_fMaxSize );
		
	// Determino la nova posició, direcció, color
	Vect3f		l_vCurrentPosition			= RandomVector ( m_vPosition1, m_vPosition2 );
	Vect3f		l_vCurrentSpawnDir			= RandomVector( m_vSpawnDir1, m_vSpawnDir2 );
	CColor		l_CCurrentColor				= RandomColor( m_Color1, m_Color2 );
		
	// Calculem la direcció de la partícula segons la velocitat
	//l_vCurrentSpawnDir = l_vCurrentSpawnDir.Normalize(); // + l_vCurrentVelocity;

	// Assignem a cada partícula els valor calculats
	l_Particle->SetDirection( l_vCurrentSpawnDir );
	l_Particle->SetPosition	( l_vCurrentPosition );
	l_Particle->SetColor	( l_CCurrentColor );
	//l_Particle->setVelocit !! TODO
	//l_Particle->setAngle		!! TODO
	//l_Particle->SetLifeTime ( l_fCurrentLifeTime );
	//l_Particle->SetAge		( l_fCurrentTime );
	//l_Particle->SetIsAlive  ( l_bCurrentAlive );
	l_Particle->SetTexture  ( m_Texture );
	l_Particle->SetWidth	( l_fCurrentSize );
	l_Particle->SetHeight	( l_fCurrentSize );
}

// Cerca el core de propietats adequat per el temps passat. S'agafa el temps exacte o el temps anterior que es mantingui dins del rang de temps
CParticlesEmitterProperties* CParticlesEmitterInstance::SearchTimeBasedAge ( float &_CurrentLife ) //, int &_CurrentIndex )
{
	CParticlesEmitterProperties * l_Properties = m_pCore->GetResource ( m_pCore->ConvertFloatToString( _CurrentLife ) );
	if ( l_Properties == NULL) 
	{
		l_Properties = m_pCore->GetPreviousCore ( m_pCore->ConvertFloatToString( _CurrentLife ) );
	}

	return l_Properties;
}

// Cerca el core de propietats però bàsicament usa el índex ja que cada partícula sempre tindrá i + 1 com a següent CORE basat en el temps al tenir ordenat el vector de CORES
float CParticlesEmitterInstance::SearchTimeBasedAge ( float _CurrentLife, int &_CurrentIndex )
{
	float l_NewCoreTime = _CurrentLife;
	float l_OldCoreTime = _CurrentLife;

	for ( size_t i = _CurrentIndex; i < m_pCore->GetResourcesVector().size(); i++ )
	{
		size_t j = i + 1;
		// Comprovo que no estigui ja en l'últim element i retorno el mateix valor d'entrada
		if ( j == m_pCore->GetResourcesVector().size() ) 
			return l_OldCoreTime;

		l_NewCoreTime = m_pCore->GetResourceById( j )->GetKeyTime();
		if ( l_OldCoreTime < l_NewCoreTime )
			return l_OldCoreTime;
		else
		{
			_CurrentIndex = i + 1;
			l_OldCoreTime = l_NewCoreTime;
		}
			
	}
	return l_OldCoreTime;
}

void CParticlesEmitterInstance::UpdateLocalParameters ( float _CurrentLife )
{
	//int l_CurrentCoreIndex = l_Particle->GetCoreIndex();
	//int l_NewCoreIndex = l_CurrentCoreIndex;

	//// Cerco els nous paràmetres 
	//l_NewCoreIndex = SearchTimeBasedAge ( l_CurrentLife, l_NewCoreIndex );

	//if ( l_CurrentCoreIndex != l_NewCoreIndex )
	//{
	//	l_Particle->SetCoreIndex( l_NewCoreIndex );
	//	UpdateValuesByIndex ( l_NewCoreIndex );
	//				
	//}
}

// Actualitzem les variables de la classe a partir del core passat. Triarà les propietats = al temps o bé l'anterior CORE
bool CParticlesEmitterInstance::UpdateValuesByTime ( float _Time )
{
	// Obtinc els paràmetres del CORE i les assigno a les variables de la Instancia
	CParticlesEmitterProperties * l_pParticlesEmitterProperties = SearchTimeBasedAge ( _Time );
	
	if ( l_pParticlesEmitterProperties == NULL )
		return false;

	m_bAlive				= l_pParticlesEmitterProperties->IsALive();
	m_Color1				= l_pParticlesEmitterProperties->GetColor1();
	m_Color2				= l_pParticlesEmitterProperties->GetColor2();
	m_fMaxEmitRate			= l_pParticlesEmitterProperties->GetMaxRate();
	m_fMinEmitRate			= l_pParticlesEmitterProperties->GetMinRate();
	m_fMaxSize				= l_pParticlesEmitterProperties->GetMaxSize();
	m_fMinSize				= l_pParticlesEmitterProperties->GetMinSize();
	m_fNumNewPartsExcess	= l_pParticlesEmitterProperties->GetNumExcess();
	m_sTexture				= l_pParticlesEmitterProperties->GetTextureName();
	m_vSpawnDir1			= l_pParticlesEmitterProperties->GetSpawnDirection1();
	m_vSpawnDir2			= l_pParticlesEmitterProperties->GetSpawnDirection2();
	m_vVelocity				= l_pParticlesEmitterProperties->GetVelocity();
	m_fAngle				= l_pParticlesEmitterProperties->GetAngle();
	m_bGravity				= l_pParticlesEmitterProperties->GetGravity();
	m_fLifeTime1			= l_pParticlesEmitterProperties->GetLifeTime1();
	m_fLifeTime2			= l_pParticlesEmitterProperties->GetLifeTime2();
	m_fKeyTime				= l_pParticlesEmitterProperties->GetKeyTime();

	// Obtinc els paràmetres del CORE i les assigno a les variables de la Instancia
	//TEmitterAtTime _ParticlesEmitterProperties = m_pCore->GetPropertiesAtTime( _Time );

	//m_bAlive				= _ParticlesEmitterProperties.m_bAlive;
	//m_Color1				= _ParticlesEmitterProperties.m_Color1;
	//m_Color2				= _ParticlesEmitterProperties.m_Color2;
	//m_fMaxEmitRate			= _ParticlesEmitterProperties.m_fMaxEmitRate;
	//m_fMinEmitRate			= _ParticlesEmitterProperties.m_fMinEmitRate;
	//m_fMaxSize				= _ParticlesEmitterProperties.m_fMaxSize;
	//m_fMinSize				= _ParticlesEmitterProperties.m_fMinSize;
	//m_fNumNewPartsExcess	= _ParticlesEmitterProperties.m_fNumNewPartsExcess;
	//m_vSpawnDir1			= _ParticlesEmitterProperties.m_vSpawnDir1;
	//m_vSpawnDir2			= _ParticlesEmitterProperties.m_vSpawnDir2;
	//m_vVelocity				= _ParticlesEmitterProperties.m_vVelocity;
	//m_fAngle				= _ParticlesEmitterProperties.m_fAngle;
	//m_bGravity				= _ParticlesEmitterProperties.m_bGravity;
	//m_fLifeTime1			= _ParticlesEmitterProperties.m_fLifeTime1;
	//m_fLifeTime2			= _ParticlesEmitterProperties.m_fLifeTime2;
	//m_sTexture				= _ParticlesEmitterProperties.m_sTexture;
	//m_fKeyTime				= _ParticlesEmitterProperties.m_fKeyTime;

	// Carreguem la textura
	CTextureManager *l_TextureManager = CORE->GetTextureManager();
	m_Texture = l_TextureManager->GetTexture ( m_sTexture );
	if ( m_Texture == NULL )
	{
		m_Texture = new CTexture();
		m_Texture->SetName ( m_sTexture );
		if ( m_Texture->Load( m_sTexture ) )
			l_TextureManager->AddResource( m_sTexture, m_Texture );
		else
		{
			CHECKED_DELETE ( m_Texture );
			//LOGGER->AddNewLog( ELL_ERROR, "CParticlesEmitterInstance::Init-> Error loading texture file : %s", m_sTexture) ;
		}
	}

	return true;
}

// Actualitzem les variables de la classe a partir del core passat basat en l'índex. Triarà les propietats = al índex o bé se següent CORE
void CParticlesEmitterInstance::UpdateValuesByIndex ( size_t _Index )
{
	// Obtinc els paràmetres del CORE i les assigno a les variables de la Instancia
	TEmitterAtTime _ParticlesEmitterProperties = m_pCore->GetPropertiesAtIndex( _Index );

	m_bAlive				= _ParticlesEmitterProperties.m_bAlive;
	m_Color1				= _ParticlesEmitterProperties.m_Color1;
	m_Color2				= _ParticlesEmitterProperties.m_Color2;
	m_fMaxEmitRate			= _ParticlesEmitterProperties.m_fMaxEmitRate;
	m_fMinEmitRate			= _ParticlesEmitterProperties.m_fMinEmitRate;
	m_fMaxSize				= _ParticlesEmitterProperties.m_fMaxSize;
	m_fMinSize				= _ParticlesEmitterProperties.m_fMinSize;
	m_fNumNewPartsExcess	= _ParticlesEmitterProperties.m_fNumNewPartsExcess;
	m_vSpawnDir1			= _ParticlesEmitterProperties.m_vSpawnDir1;
	m_vSpawnDir2			= _ParticlesEmitterProperties.m_vSpawnDir2;
	m_vVelocity				= _ParticlesEmitterProperties.m_vVelocity;
	m_fAngle				= _ParticlesEmitterProperties.m_fAngle;
	m_bGravity				= _ParticlesEmitterProperties.m_bGravity;
	m_fLifeTime1			= _ParticlesEmitterProperties.m_fLifeTime1;
	m_fLifeTime2			= _ParticlesEmitterProperties.m_fLifeTime2;
	m_sTexture				= _ParticlesEmitterProperties.m_sTexture;
	m_fKeyTime				= _ParticlesEmitterProperties.m_fKeyTime;

	// Carreguem la textura
	CTextureManager *l_TextureManager = CORE->GetTextureManager();
	m_Texture = l_TextureManager->GetTexture( m_sTexture );
	if ( m_Texture == NULL )
	{
		m_Texture = new CTexture();
		m_Texture->SetName ( m_sTexture );
		if ( m_Texture->Load( m_sTexture ) )
			l_TextureManager->AddResource( m_sTexture, m_Texture );
		else
		{
			CHECKED_DELETE ( m_Texture );
			//LOGGER->AddNewLog( ELL_ERROR, "CParticlesEmitterInstance::Init-> Error loading texture file : %s", m_sTexture) ;
		}
	}
}

Vect3f CParticlesEmitterInstance::RandomVector( const Vect3f &_Vect1, const Vect3f &_Vect2 )
{
	// determine a random vector between _vect1 and _vect2
	float fRandX = m_Randomize.getRandFloat( _Vect1.x, _Vect2.x );
	float fRandY = m_Randomize.getRandFloat( _Vect1.y, _Vect2.y );
	float fRandZ = m_Randomize.getRandFloat( _Vect1.z, _Vect2.z );
	
	return ( Vect3f ( fRandX, fRandY, fRandZ ) );
}

CColor CParticlesEmitterInstance::RandomColor( const CColor &_Color1, const CColor &_Color2 )
{
	float fRandR = m_Randomize.getRandFloat( _Color1.GetRed(), _Color2.GetRed() );
	float fRandG = m_Randomize.getRandFloat( _Color1.GetGreen(), _Color2.GetGreen() );
	float fRandB = m_Randomize.getRandFloat( _Color1.GetBlue(), _Color2.GetBlue() );
	float fRandA = m_Randomize.getRandFloat( _Color1.GetAlpha(), _Color2.GetAlpha() );
	return ( CColor( fRandR, fRandG, fRandB, fRandA ) );
}

float CParticlesEmitterInstance::RandomFloat( const float &_Value1, const float &_Value2 )
{
	return ( m_Randomize.getRandFloat( _Value1, _Value2 ) );
}


// -----------------------------------------
//				MÈTODES PUBLICS
// -----------------------------------------

// -----------------------------------------
//				PROPIEDADES
// -----------------------------------------
