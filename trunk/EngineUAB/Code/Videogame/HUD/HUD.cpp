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
	, m_WolfBarPosition(v2iZERO)
	, m_WolfBarSize(v2iZERO)
	, m_WolfBarRealSize(v2iZERO)
	, m_WolfMaskPosition(v2iZERO)
	, m_WolfMaskSize(v2iZERO)
	, m_WolfBackgroundPosition(v2iZERO)
	, m_WolfBackgroundSize(v2iZERO)
	, m_bWolfBarActive(true)
	, m_bWolfBackgroundActive(true)
	, m_bWolfMaskActive(true)
	, m_pWolfBar(NULL)
	, m_pWolfMask(NULL)
	, m_pWolfBackground(NULL)
	, m_WolfPerCentSize(1.f)
	, m_iWolfLife(100)
	, m_iWolfPreviousLife(100)
	, m_iCurrentWolfLife(100)
	, m_bWolfActive(false)
	, m_bPlayerActive(false)
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
			if( l_TypeHUD == "PlayerLifeBar" )
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
			else if( l_TypeHUD == "WolfLifeBar" )
			{
				uint8 l_TotalNodes = l_Node.GetNumChildren();
				for ( uint8 i = 0; i < l_TotalNodes; ++i )
				{
					std::string l_Type = l_Node(i).GetName();
					if( l_Type == "Background" )
					{
						m_WolfBackgroundPosition = l_Node(i).GetVect2iProperty("position", v2iZERO);
						m_WolfBackgroundSize = l_Node(i).GetVect2iProperty("size", v2iZERO);
						m_bWolfBackgroundActive = l_Node(i).GetBoolProperty("active", false);
						std::string l_TextureName = l_Node(i).GetPszProperty("texture", "");

						m_pWolfBackground = CORE->GetTextureManager()->GetTexture(l_TextureName);

						assert( m_pWolfBackground );

						m_pWolfBackground->SetName( l_TextureName );

					}
					else if( l_Type == "Bar" )
					{
						m_WolfBarPosition = l_Node(i).GetVect2iProperty("position", v2iZERO);
						m_WolfBarSize = l_Node(i).GetVect2iProperty("size", v2iZERO);
						m_bWolfBarActive = l_Node(i).GetBoolProperty("active", false);
						std::string l_TextureName = l_Node(i).GetPszProperty("texture", "");

						m_pWolfBar = CORE->GetTextureManager()->GetTexture(l_TextureName);

						assert( m_pWolfBar );

						m_pWolfBar->SetName( l_TextureName );
						m_WolfBarRealSize = m_WolfBarSize;
					}
					else if( l_Type == "Mask" )
					{
						m_WolfMaskPosition = l_Node(i).GetVect2iProperty("position", v2iZERO);
						m_WolfMaskSize = l_Node(i).GetVect2iProperty("size", v2iZERO);
						m_bWolfMaskActive = l_Node(i).GetBoolProperty("active", false);
						std::string l_TextureName = l_Node(i).GetPszProperty("texture", "");

						m_pWolfMask = CORE->GetTextureManager()->GetTexture(l_TextureName);

						assert( m_pWolfMask );

						m_pWolfMask->SetName( l_TextureName );
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
void CHud::InitWolf( int _iWolfLife )
{
	m_iWolfLife				= _iWolfLife;
	m_iWolfPreviousLife		= _iWolfLife;
}

//------------------------------------------------
void CHud::Update( float _fElapsedTime, int _iPlayerLife )
{
	// Player Life Bar
	//-------------------------------------
	if( m_bPlayerActive )
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
	}

	// Wolf Life Bar
	//------------------------------------------
	if( m_bWolfActive )
	{
		float l_fTarget = (float)m_iCurrentWolfLife / (float)m_iWolfLife;
		if( m_iCurrentWolfLife < m_iWolfPreviousLife )
		{
			m_WolfPerCentSize = m_WolfPerCentSize - 0.5f * _fElapsedTime;
			if( m_WolfPerCentSize <= l_fTarget )
			{
				m_WolfPerCentSize = l_fTarget;
				m_iWolfPreviousLife = m_iCurrentWolfLife;
			}
		}
		else if( m_iCurrentWolfLife > m_iPlayerPreviousLife )
		{
			m_WolfPerCentSize = m_WolfPerCentSize + 0.5f * _fElapsedTime;
			if( m_WolfPerCentSize >= l_fTarget )
			{
				m_WolfPerCentSize = l_fTarget;
				m_iWolfPreviousLife = m_iCurrentWolfLife;
			}
		}

		m_WolfBarRealSize.x = static_cast<int>( static_cast<float>(m_WolfBarSize.x) * m_WolfPerCentSize);
	}


	//Efecto de blanco y negro
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

	LPDIRECT3DDEVICE9 l_Device = RM.GetDevice();

	//Inicia la transparencia
	l_Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	l_Device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	l_Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	//Pinta la barra de vida del player
	if( m_bPlayerActive )
	{
		if( m_bBackgroundActive )
			RM.DrawTexturedQuad2D(m_BackgroundPosition, m_BackgroundSize.x, m_BackgroundSize.y,  UPPER_LEFT, m_pBackground );

		if( m_bBarActive )
			RM.DrawTexturedQuad2D(m_BarPosition, v2fZERO, Vect2f(m_PerCentSize, 1.f), m_BarRealSize.x, m_BarRealSize.y,  UPPER_LEFT, m_pBar );

		if( m_bMaskActive )
			RM.DrawTexturedQuad2D(m_MaskPosition, m_MaskSize.x, m_MaskSize.y,  UPPER_LEFT, m_pMask );
	}

	//Pinta la barra de vida del lobo
	if( m_bWolfActive )
	{
		if( m_bWolfBackgroundActive )
			RM.DrawTexturedQuad2D(m_WolfBackgroundPosition, m_WolfBackgroundSize.x, m_WolfBackgroundSize.y,  UPPER_LEFT, m_pWolfBackground );

		if( m_bWolfBarActive )
			RM.DrawTexturedQuad2D(m_WolfBarPosition, v2fZERO, Vect2f(m_WolfPerCentSize, 1.f), m_WolfBarRealSize.x, m_WolfBarRealSize.y,  UPPER_LEFT, m_pWolfBar );

		if( m_bWolfMaskActive )
			RM.DrawTexturedQuad2D(m_WolfMaskPosition, m_WolfMaskSize.x, m_WolfMaskSize.y,  UPPER_LEFT, m_pWolfMask );
	}

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
	l_Device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
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

	// Player Life Bar
	//--------------------------

	//Calcula la posición de la barra de vida
	m_MaskPosition.x = static_cast<uint32>(m_MaskPosition.x * 0.01f * l_ScreenSize.x);
	m_MaskPosition.y = static_cast<uint32>(m_MaskPosition.y * 0.01f * l_ScreenSize.y);

	m_BarPosition.x = static_cast<uint32>(m_BarPosition.x * 0.01f * l_ScreenSize.x);
	m_BarPosition.y = static_cast<uint32>(m_BarPosition.y * 0.01f * l_ScreenSize.y);

	m_BackgroundPosition.x = static_cast<uint32>(m_BackgroundPosition.x * 0.01f * l_ScreenSize.x);
	m_BackgroundPosition.y = static_cast<uint32>(m_BackgroundPosition.y * 0.01f * l_ScreenSize.y);

	// Wolf Life Bar
	//--------------------------
	//Calcula la posición de la barra de vida
	m_WolfMaskPosition.x = static_cast<uint32>(m_WolfMaskPosition.x * 0.01f * l_ScreenSize.x);
	m_WolfMaskPosition.y = static_cast<uint32>(m_WolfMaskPosition.y * 0.01f * l_ScreenSize.y);

	m_WolfBarPosition.x = static_cast<uint32>(m_WolfBarPosition.x * 0.01f * l_ScreenSize.x);
	m_WolfBarPosition.y = static_cast<uint32>(m_WolfBarPosition.y * 0.01f * l_ScreenSize.y);

	m_WolfBackgroundPosition.x = static_cast<uint32>(m_WolfBackgroundPosition.x * 0.01f * l_ScreenSize.x);
	m_WolfBackgroundPosition.y = static_cast<uint32>(m_WolfBackgroundPosition.y * 0.01f * l_ScreenSize.y);

	// Texturas de información
	//-------------------------

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