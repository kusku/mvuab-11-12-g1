#include "BillboardAnimation.h"
#include "Billboard.h"
#include "Cameras\Camera.h" 
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"
#include <sstream>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CBillboardAnimation::CBillboardAnimation( float _Time, const std::vector<CTexture *> & _vBillboardAnimation )
	: m_fTimePerImage	( _Time )	
	, m_fCurrentTime	( 0.f )
	, m_uiImage			( 0 )
	, m_vColor			( Vect4f(0.f,0.f,0.f,0.f) )
	, m_fSize			( 0.f )
	, m_uiNumTexturas	( 0 )
	, m_bLoop			( false )
{}

CBillboardAnimation::CBillboardAnimation( const CBillboardAnimation *_BillboardAnimation )
{
	CBillboard::SetVisible( _BillboardAnimation->GetVisible() );
	m_bLoop			= _BillboardAnimation->GetLoop();
	m_fTimePerImage = _BillboardAnimation->GetTimePerImage();
	m_fCurrentTime  = _BillboardAnimation->GetUpdatedTime();
	m_uiImage		= _BillboardAnimation->GetImageIndex();
	m_vColor		= _BillboardAnimation->GetColor();
	m_fSize			= _BillboardAnimation->GetSize();
	m_uiNumTexturas	= _BillboardAnimation->GetTotalTextures();
	m_vTextures		= _BillboardAnimation->GetTexturesVector();

	return;
}

CBillboardAnimation::CBillboardAnimation ( CXMLTreeNode &_Node )
	: m_fTimePerImage	( 0.f )	
	, m_fCurrentTime	( 0.f )
	, m_uiImage			( 0 )
	, m_vColor			( Vect4f(0.f,0.f,0.f,0.f) )
	, m_fSize			( 0.f )
	, m_uiNumTexturas	( 0 )
	, m_bLoop			( false )
{
	SetName ( _Node.GetPszProperty ( "name", "" ) );
					
	uint16 l_TotalAnimationNodes = _Node.GetNumChildren ();
	for ( uint16 i = 0; i < l_TotalAnimationNodes; ++i )
	{ 
		std::string l_Node = _Node(i).GetName();
		if ( l_Node == "color" )
			m_vColor = _Node(i).GetVect4fProperty( "col", Vect4f(0.f,0.f,0.f,0.f), true );				

		else if ( l_Node == "size" )
			m_fSize = _Node(i).GetFloatProperty ( "siz" );

		else if ( l_Node == "numText" )
			m_uiNumTexturas = _Node(i).GetIntProperty( "num" );
		
		else if ( l_Node == "timePerImage" )
			m_fTimePerImage = _Node(i).GetFloatProperty( "time" );

		else if ( l_Node == "loop" )
			m_bLoop = _Node(i).GetBoolProperty( "bucle" );

		else if ( l_Node == "textures" )
		{
			for ( int j = 0; j < m_uiNumTexturas; j++ )
			{
				std::string l_TextureName = _Node(i)(j).GetPszProperty( "filename" );
				CTexture * l_Text = new CTexture();
				if ( l_Text->Load( l_TextureName ) )
				{
					CORE->GetTextureManager()->AddResource( l_TextureName, l_Text );
					m_vTextures.push_back ( l_Text ); 
				}
				else
					LOGGER->AddNewLog( ELL_ERROR, "CBillboardAnimation::CBillboardAnimation-> Error loading texture file : %s", l_TextureName) ;
			}
		}		
	}
}

CBillboardAnimation::~CBillboardAnimation( void )
{
	Destroy();
}

// -----------------------------------------
//			   MÈTODES PRINCIPALS
// -----------------------------------------
void CBillboardAnimation::Destroy ( void )
{
	std::vector<CTexture *>::iterator l_It = m_vTextures.begin();
	std::vector<CTexture *>::iterator l_End = m_vTextures.end();

	for ( ; l_It < l_End; ++l_It )
	{
		(*l_It) = NULL;
	}
	m_vTextures.clear();
}

void CBillboardAnimation::Update ( float _ElapsedTime )
{
	m_fCurrentTime += _ElapsedTime;

	if ( m_vTextures.size() < 1 )
		return;

	if ( m_fCurrentTime >= m_fTimePerImage )
	{
		m_uiImage += 1;
		if ( m_uiImage > m_uiNumTexturas -1 )
			m_uiImage = 0;
	}

	CBillboard::SetTexture( m_vTextures[m_uiImage] );
	CBillboard::Update( CORE->GetCamera(), 0.f );
}

void CBillboardAnimation::Render ( CRenderManager &_RM, const CColor &_Color )
{
	CBillboard::Render( _RM, _Color );
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
