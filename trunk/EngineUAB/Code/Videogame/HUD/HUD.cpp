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
	, m_iPlayerPreviousLife(100)
	, m_fThresholdDyingEffect(0.f)
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
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	l_pTextureManager->RemoveResource( m_pBar->GetName() );
	l_pTextureManager->RemoveResource( m_pMask->GetName() );
	l_pTextureManager->RemoveResource( m_pBackground->GetName() );

	TMapResources l_TextureMap = GetResourcesMap();
	TMapResources::iterator l_It = l_TextureMap.begin();
	TMapResources::iterator l_End = l_TextureMap.end();
	for(; l_It != l_End; ++l_It)
	{
		l_pTextureManager->RemoveResource( l_It->first );
	}

	CTemplatedVectorMapManager::Destroy();
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
		uint8 l_NumNodes = l_RootNode.GetNumChildren();
		for(uint8 j=0; j<l_NumNodes; ++j)
		{
			CXMLTreeNode l_Node = l_RootNode(j);
			std::string l_TypeHUD = l_Node.GetName();
			if( l_TypeHUD == "LifeBar" )
			{
				m_fThresholdDyingEffect = l_Node.GetFloatProperty("threshold_to_dying_effect", 0.f);

				uint8 l_TotalNodes = l_Node.GetNumChildren();
				for ( uint8 i = 0; i < l_TotalNodes; ++i )
				{
					std::string l_Type = l_Node(i).GetName();
					if( l_Type == "Background" )
					{
						m_BackgroundPosition = l_Node(i).GetVect2iProperty("position", v2iZERO);
						m_BackgroundSize = l_Node(i).GetVect2iProperty("size", v2iZERO);
						m_bBackgroundActive = l_Node(i).GetBoolProperty("active", false);
						std::string l_TextureName = l_Node(i).GetPszProperty("texture", "");

						m_pBackground = CORE->GetTextureManager()->GetTexture(l_TextureName);

						assert( m_pBackground );

						m_pBackground->SetName( l_TextureName );

					}
					else if( l_Type == "Bar" )
					{
						m_BarPosition = l_Node(i).GetVect2iProperty("position", v2iZERO);
						m_BarSize = l_Node(i).GetVect2iProperty("size", v2iZERO);
						m_bBarActive = l_Node(i).GetBoolProperty("active", false);
						std::string l_TextureName = l_Node(i).GetPszProperty("texture", "");

						m_pBar = CORE->GetTextureManager()->GetTexture(l_TextureName);

						assert( m_pBar );

						m_pBar->SetName( l_TextureName );
						m_BarRealSize = m_BarSize;
					}
					else if( l_Type == "Mask" )
					{
						m_MaskPosition = l_Node(i).GetVect2iProperty("position", v2iZERO);
						m_MaskSize = l_Node(i).GetVect2iProperty("size", v2iZERO);
						m_bMaskActive = l_Node(i).GetBoolProperty("active", false);
						std::string l_TextureName = l_Node(i).GetPszProperty("texture", "");

						m_pMask = CORE->GetTextureManager()->GetTexture(l_TextureName);

						assert( m_pMask );

						m_pMask->SetName( l_TextureName );
					}
				}
			}
			else if( l_TypeHUD == "Texture" )
			{
				std::string l_Name	= l_Node.GetPszProperty("name", "");
				std::string l_Path	= l_Node.GetPszProperty("file", "");
				Vect2i		l_Pos	= l_Node.GetVect2iProperty("position", v2iZERO);
				Vect2i		l_Size	= l_Node.GetVect2iProperty("size", v2iZERO);
				bool		l_Active= l_Node.GetBoolProperty("active", false);

				CTexture *l_pTexture = new CTexture();
				l_pTexture->Load(l_Path);

				CORE->GetTextureManager()->AddResource(l_Name, l_pTexture);

				STexture *l_pTextureInfo	= new STexture();
				l_pTextureInfo->pTexture	= l_pTexture;
				l_pTextureInfo->position	= l_Pos;
				l_pTextureInfo->size		= l_Size;
				l_pTextureInfo->bActive		= l_Active;

				CTemplatedVectorMapManager::AddResource(l_Name, l_pTextureInfo);
			}
		}

		
	}

	CalculatePositions();

	return true;
}

//------------------------------------------------
void CHud::Init( int _iPlayerLife )
{
	m_iPlayerLife			= _iPlayerLife;
	m_iPlayerPreviousLife	= _iPlayerLife;
}

//------------------------------------------------
void CHud::Update( float _fElapsedTime, int _iPlayerLife )
{
	float l_fTarget = (float)_iPlayerLife / (float)m_iPlayerLife;
	if( _iPlayerLife < m_iPlayerPreviousLife )
	{
		m_PerCentSize = m_PerCentSize - 0.5f * _fElapsedTime;
		if( m_PerCentSize <= l_fTarget )
		{
			m_PerCentSize = l_fTarget;
			m_iPlayerPreviousLife = _iPlayerLife;
		}
	}
	else if( _iPlayerLife > m_iPlayerPreviousLife )
	{
		m_PerCentSize = m_PerCentSize + 0.5f * _fElapsedTime;
		if( m_PerCentSize >= l_fTarget )
		{
			m_PerCentSize = l_fTarget;
			m_iPlayerPreviousLife = _iPlayerLife;
		}
	}

	m_BarRealSize.x = static_cast<int>( static_cast<float>(m_BarSize.x) * m_PerCentSize);

	if( m_PerCentSize <= m_fThresholdDyingEffect )
	{
		float l_fDying	= m_PerCentSize;
		l_fDying		= l_fDying / m_fThresholdDyingEffect;
		l_fDying		= 1.f - l_fDying;

		CORE->SetDyingAmount( l_fDying );
	}
	else
	{
		CORE->SetDyingAmount( 0.f );
	}
}

//------------------------------------------------
void CHud::Render( CRenderManager &RM )
{
	Mat44f mat;
	mat.SetIdentity();
	RM.SetTransform(mat);

	//Inicia la transparencia
	CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	//Pinta la barra de vida
	if( m_bBackgroundActive )
		RM.DrawTexturedQuad2D(m_BackgroundPosition, m_BackgroundSize.x, m_BackgroundSize.y,  UPPER_LEFT, m_pBackground );

	if( m_bBarActive )
		RM.DrawTexturedQuad2D(m_BarPosition, Vect2f(0.f, 0.f), Vect2f(m_PerCentSize, 1.f), m_BarRealSize.x, m_BarRealSize.y,  UPPER_LEFT, m_pBar );

	if( m_bMaskActive )
		RM.DrawTexturedQuad2D(m_MaskPosition, m_MaskSize.x, m_MaskSize.y,  UPPER_LEFT, m_pMask );

	//Renderiza otras texturas del HUD
	TVectorResources l_TextureVector = GetResourcesVector();
	for(uint16 i=0; i<l_TextureVector.size(); ++i)
	{
		STexture *l_pTextureInfo = l_TextureVector[i];
		if( l_pTextureInfo->bActive )
		{
			RM.DrawTexturedQuad2D(l_pTextureInfo->position, l_pTextureInfo->size.x, l_pTextureInfo->size.y,  UPPER_LEFT, l_pTextureInfo->pTexture );
		}
	}

	//Finaliza la transparencia
	CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

//------------------------------------------------
bool CHud::IsTextureActive( const std::string &_name )
{
	STexture *l_pTextureInfo = CTemplatedVectorMapManager::GetResource(_name);
	if(l_pTextureInfo != NULL)
	{
		return l_pTextureInfo->bActive;
	}

	return false;
}

//------------------------------------------------
void CHud::ActiveTexture( const std::string &_name, bool _bActive )
{
	STexture *l_pTextureInfo = CTemplatedVectorMapManager::GetResource(_name);
	if(l_pTextureInfo != NULL)
	{
		l_pTextureInfo->bActive = _bActive;
	}
}

//------------------------------------------------
void CHud::CalculatePositions()
{
	Vect2i l_ScreenSize = CORE->GetRenderManager()->GetScreenSize();

	//Calcula la posición de la barra de vida
	m_MaskPosition.x = static_cast<uint32>(m_MaskPosition.x * 0.01f * l_ScreenSize.x);
	m_MaskPosition.y = static_cast<uint32>(m_MaskPosition.y * 0.01f * l_ScreenSize.y);

	m_BarPosition.x = static_cast<uint32>(m_BarPosition.x * 0.01f * l_ScreenSize.x);
	m_BarPosition.y = static_cast<uint32>(m_BarPosition.y * 0.01f * l_ScreenSize.y);

	m_BackgroundPosition.x = static_cast<uint32>(m_BackgroundPosition.x * 0.01f * l_ScreenSize.x);
	m_BackgroundPosition.y = static_cast<uint32>(m_BackgroundPosition.y * 0.01f * l_ScreenSize.y);

	//Calcula la posición de los elementos de textura
	TVectorResources l_TextureInfoVector = GetResourcesVector();
	TVectorResources::iterator l_It = l_TextureInfoVector.begin();
	TVectorResources::iterator l_ItEnd = l_TextureInfoVector.end();
	for(; l_It != l_ItEnd; ++l_It)
	{
		Vect2i l_Position = (*l_It)->position;

		l_Position.x = static_cast<uint32>(l_Position.x * 0.01f * l_ScreenSize.x);
		l_Position.y = static_cast<uint32>(l_Position.y * 0.01f * l_ScreenSize.y);

		(*l_It)->position = l_Position;
	}
}