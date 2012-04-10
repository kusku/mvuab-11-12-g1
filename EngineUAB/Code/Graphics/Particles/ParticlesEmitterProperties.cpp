#include "ParticlesEmitterProperties.h"
#include "XML\XMLTreeNode.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CParticlesEmitterProperties::CParticlesEmitterProperties( void )

	: m_vPosition1			( Vect3f(0.f,0.f,0.f) )
	, m_vPosition2			( Vect3f(0.f,0.f,0.f) )
	, m_vVelocity			( Vect3f(0.f,0.f,0.f) )
	, m_fAngle				( 0.f )
	, m_fMinEmitRate		( 0.f )
	, m_fMaxEmitRate		( 0.f )
	, m_Color1				( CColor(0.f,0.f,0.f,0.f) )
	, m_Color2				( CColor(0.f,0.f,0.f,0.f) )
	, m_fMinSize			( 0.f )
	, m_fMaxSize			( 0.f )
	, m_vSpawnDir1			( Vect3f(0.f,0.f,0.f) )
	, m_vSpawnDir2			( Vect3f(0.f,0.f,0.f) )
	, m_fNumNewPartsExcess	( 0.f )
	, m_bAlive				( true )
	, m_bGravity			( true )
	, m_sTexture			( "" )
	, m_fLifeTime1			( 0.f )
	, m_fLifeTime2			( 0.f )
	, m_fKeyTime			( 0.f )
{}

CParticlesEmitterProperties::CParticlesEmitterProperties( const CXMLTreeNode &_Node )
{
	//uint16 l_TotalNodes = _Node.GetNumChildren ();
	//for ( uint16 i = 0; i < l_TotalNodes; ++i )
	//{ 
	//	std::string l_Node = _Node(i).GetName();
	//	if ( l_Node == "color" )
	//	{
	//		m_Color1 = static_cast<CColor> (_Node(i).GetVect4fProperty( "color1", Vect4f(1.f,0.f,0.f,1.f), true ));
	//		m_Color2 = static_cast<CColor> (_Node(i).GetVect4fProperty( "color2", Vect4f(1.f,0.f,0.f,1.f), true ));

	//		uint16 l_TotalSubNodes = _Node(i).GetNumChildren ();
	//		for ( uint16 j = 0; j < l_TotalSubNodes; ++j )
	//		{
	//			std::string l_Node = _Node(i).GetName();
	//			if ( l_Node == "time" )
	//			{
	//			}
	//			else 
	//				l_ExistErrors = true;
	//		}
	//		
	//	}

	//	else if ( l_Node == "" )
	//	{
	//		m_fMinSize = _Node(i).GetFloatProperty ( "mins" );
	//		m_fMaxSize = _Node(i).GetFloatProperty ( "maxs" );
	//	}

	//	/*else if ( l_Node == "numText" )
	//		m_uiNumTexturas = _Node(i).GetIntProperty( "num" );
	//	
	//	else if ( l_Node == "timePerImage" )
	//		m_fTimePerImage = _Node(i).GetFloatProperty( "time" );

	//	else if ( l_Node == "loop" )
	//		m_bLoop = _Node(i).GetBoolProperty( "bucle" );

	//	else if ( l_Node == "textures" )
	//	{
	//		for ( int j = 0; j < m_uiNumTexturas; j++ )
	//		{
	//			std::string l_TextureName = _Node(i)(j).GetPszProperty( "filename" );
	//			CTexture * l_Text = new CTexture();
	//			if ( l_Text->Load( l_TextureName ) )
	//			{
	//				CORE->GetTextureManager()->AddResource( l_TextureName, l_Text );
	//				m_vTextures.push_back ( l_Text ); 
	//			}
	//			else
	//				LOGGER->AddNewLog( ELL_ERROR, "CBillboardAnimation::CBillboardAnimation-> Error loading texture file : %s", l_TextureName) ;
	//		}
	//	}		*/
	//}

	//m_vEmitterPosition	= Vect3f( 0.f,0.f,0.f);
	//m_fMinEmitRate		= 10;
	//m_fMaxEmitRate		= 100;
	//
	//m_fMinSize			= 5.f;
	//m_fMaxSize			= 105.f;;
	//m_vSpawnDir1		= Vect3f( 1.f,0.5f,0.f);
	//m_vSpawnDir2		= Vect3f( 0.f,0.5f,0.f);
	//m_fNumNewPartsExcess= 10.f;
	//
	//m_sTexture			= "Data/ParticlesTextures/boom/rlboom_1.tga";

}

CParticlesEmitterProperties::CParticlesEmitterProperties ( const CParticlesEmitterProperties *_pParticlesEmitterProperties )
{
	m_bAlive				= _pParticlesEmitterProperties->IsALive();
	m_Color1				= _pParticlesEmitterProperties->GetColor1();
	m_Color2				= _pParticlesEmitterProperties->GetColor2();
	m_fMaxEmitRate			= _pParticlesEmitterProperties->GetMaxRate();
	m_fMinEmitRate			= _pParticlesEmitterProperties->GetMinRate();
	m_fMaxSize				= _pParticlesEmitterProperties->GetMaxSize();
	m_fMinSize				= _pParticlesEmitterProperties->GetMinSize();
	m_fNumNewPartsExcess	= _pParticlesEmitterProperties->GetNumExcess();
	m_sTexture				= _pParticlesEmitterProperties->GetTextureName();
	m_vPosition1			= _pParticlesEmitterProperties->GetPosition1();
	m_vPosition2			= _pParticlesEmitterProperties->GetPosition2();
	m_vSpawnDir1			= _pParticlesEmitterProperties->GetSpawnDirection1();
	m_vSpawnDir2			= _pParticlesEmitterProperties->GetSpawnDirection2();
	m_vVelocity				= _pParticlesEmitterProperties->GetVelocity();
	m_fAngle				= _pParticlesEmitterProperties->GetAngle();
	m_bGravity				= _pParticlesEmitterProperties->GetGravity();
	m_fLifeTime1			= _pParticlesEmitterProperties->GetLifeTime1();
	m_fLifeTime2			= _pParticlesEmitterProperties->GetLifeTime2();
	m_fKeyTime				= _pParticlesEmitterProperties->GetKeyTime();
}

CParticlesEmitterProperties::~CParticlesEmitterProperties(void)
{
}


// -----------------------------------------
//			   MÈTODES PRINCIPALS
// -----------------------------------------

// -----------------------------------------
//				MÈTODES PRIVATS
// -----------------------------------------

// -----------------------------------------
//				 PROPIETATS 
// -----------------------------------------
TEmitterAtTime CParticlesEmitterProperties::GetProperties ( void )
{
	TEmitterAtTime l_PropietatsEmissor;
	l_PropietatsEmissor.m_fLifeTime1			= GetLifeTime1		();
	l_PropietatsEmissor.m_fLifeTime2			= GetLifeTime2		();
	l_PropietatsEmissor.m_bAlive				= IsALive			();
	l_PropietatsEmissor.m_Color1				= GetColor1			();
	l_PropietatsEmissor.m_Color2				= GetColor2			();
	l_PropietatsEmissor.m_fMaxEmitRate			= GetMaxRate		();
	l_PropietatsEmissor.m_fMinEmitRate			= GetMinRate		();
	l_PropietatsEmissor.m_fMaxSize				= GetMaxSize		();
	l_PropietatsEmissor.m_fMinSize				= GetMinSize		();
	l_PropietatsEmissor.m_fNumNewPartsExcess	= GetNumExcess		();
	l_PropietatsEmissor.m_sTexture				= GetTextureName	();
	l_PropietatsEmissor.m_vSpawnDir1			= GetSpawnDirection1();
	l_PropietatsEmissor.m_vSpawnDir2			= GetSpawnDirection2();
	l_PropietatsEmissor.m_vPosition1			= GetPosition1		();
	l_PropietatsEmissor.m_vPosition2			= GetPosition2		();
	l_PropietatsEmissor.m_vVelocity				= GetVelocity		();
	l_PropietatsEmissor.m_fAngle				= GetAngle			();
	l_PropietatsEmissor.m_bGravity				= GetGravity		();
	l_PropietatsEmissor.m_fKeyTime				= GetKeyTime		();

	return l_PropietatsEmissor;
}