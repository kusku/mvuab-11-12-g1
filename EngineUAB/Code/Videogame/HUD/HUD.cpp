#include "HUD.h"
#include "RenderManager.h"
#include "Textures\TextureManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"
#include "Math\Matrix44.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

//------------------------------------------------
CHud::CHud()
	: m_BarPosition(v2iZERO)
	, m_BarSize(v2iZERO)
	, m_BarRealSize(v2iZERO)
	, m_MaskPosition(v2iZERO)
	, m_MaskSize(v2iZERO)
	, m_BackgroundPosition(v2iZERO)
	, m_BackgroundSize(v2iZERO)
	, m_bBarActive(true)
	, m_bBackgroundActive(true)
	, m_bMaskActive(true)
	, m_pBar(NULL)
	, m_pMask(NULL)
	, m_pBackground(NULL)
	, m_PerCentSize(1.f)
	, m_iPlayerLife(100)
	, m_Filename("")
{

}

//------------------------------------------------
CHud::~CHud()
{
	CleanUp();
}

//------------------------------------------------
void CHud::CleanUp()
{
	CORE->GetTextureManager()->RemoveResource( m_pBar->GetName() );
	CORE->GetTextureManager()->RemoveResource( m_pMask->GetName() );
	CORE->GetTextureManager()->RemoveResource( m_pBackground->GetName() );
}

//------------------------------------------------
bool CHud::Load( const std::string &_Filename )
{
	m_Filename = _Filename;
	return LoadFile();
}

//------------------------------------------------
bool CHud::Reload()
{
	CleanUp();
	return LoadFile();
}

//------------------------------------------------
bool CHud::LoadFile()
{
	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_Filename.c_str ( ) ) )
	{
		std::string msg_error = "CHud::LoadFile->Error al leer el fichero de HUD: " + m_Filename;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	CXMLTreeNode l_RootNode = newFile ["HUD"];
	if ( l_RootNode.Exists() )
	{
		uint8 l_TotalNodes = l_RootNode.GetNumChildren();
		for ( uint8 i = 0; i < l_TotalNodes; ++i )
		{
			std::string l_Type = l_RootNode(i).GetName();
			if( l_Type == "Background" )
			{
				m_BackgroundPosition = l_RootNode(i).GetVect2iProperty("position", v2iZERO);
				m_BackgroundSize = l_RootNode(i).GetVect2iProperty("size", v2iZERO);
				m_bBackgroundActive = l_RootNode(i).GetBoolProperty("active", false);
				std::string l_TextureName = l_RootNode(i).GetPszProperty("texture", "");

				m_pBackground = CORE->GetTextureManager()->GetTexture(l_TextureName);

				assert( m_pBackground );

				m_pBackground->SetName( l_TextureName );

			}
			else if( l_Type == "Bar" )
			{
				m_BarPosition = l_RootNode(i).GetVect2iProperty("position", v2iZERO);
				m_BarSize = l_RootNode(i).GetVect2iProperty("size", v2iZERO);
				m_bBarActive = l_RootNode(i).GetBoolProperty("active", false);
				std::string l_TextureName = l_RootNode(i).GetPszProperty("texture", "");

				m_pBar = CORE->GetTextureManager()->GetTexture(l_TextureName);

				assert( m_pBar );

				m_pBar->SetName( l_TextureName );
				m_BarRealSize = m_BarSize;
			}
			else if( l_Type == "Mask" )
			{
				m_MaskPosition = l_RootNode(i).GetVect2iProperty("position", v2iZERO);
				m_MaskSize = l_RootNode(i).GetVect2iProperty("size", v2iZERO);
				m_bMaskActive = l_RootNode(i).GetBoolProperty("active", false);
				std::string l_TextureName = l_RootNode(i).GetPszProperty("texture", "");

				m_pMask = CORE->GetTextureManager()->GetTexture(l_TextureName);

				assert( m_pMask );

				m_pMask->SetName( l_TextureName );
			}
		}
	}

	return true;
}

//------------------------------------------------
void CHud::Init( int _iPlayerLife )
{
	m_iPlayerLife = _iPlayerLife;
}

//------------------------------------------------
void CHud::Update( float _fElapsedTime, int _iPlayerLife )
{
	m_PerCentSize = (float)_iPlayerLife / (float)m_iPlayerLife;

	m_BarRealSize.x = static_cast<int>( static_cast<float>(m_BarSize.x) * m_PerCentSize);
}

//------------------------------------------------
void CHud::Render( CRenderManager &RM )
{
	Mat44f mat;
	mat.SetIdentity();
	RM.SetTransform(mat);

	CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	if( m_bBackgroundActive )
		RM.DrawTexturedQuad2D(m_BackgroundPosition, m_BackgroundSize.x, m_BackgroundSize.y,  UPPER_LEFT, m_pBackground );

	if( m_bBarActive )
		RM.DrawTexturedQuad2D(m_BarPosition, Vect2f(0.f, 0.f), Vect2f(m_PerCentSize, 1.f), m_BarRealSize.x, m_BarRealSize.y,  UPPER_LEFT, m_pBar );

	if( m_bMaskActive )
		RM.DrawTexturedQuad2D(m_MaskPosition, m_MaskSize.x, m_MaskSize.y,  UPPER_LEFT, m_pMask );

	CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}