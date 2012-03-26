#include "ModifierManager.h"
#include "Fonts\FontManager.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Utils\BaseUtils.h"
#include "Base.h"
#include "Core.h"
#include "LogRender\LogRender.h"
#include "DebugGUIManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CModifierManager::CModifierManager()
	: m_NowIndexInVector(0)
	, m_FileName("")
	, m_IsChild(false)
	, m_SizeRectangle(Vect2i(0, 0))
{
	SetActive(true);
}


CModifierManager::~CModifierManager()
{
	CleanUp();
}

void CModifierManager::CleanUp()
{
	Destroy();
}

void CModifierManager::Render(CRenderManager &RM, CFontManager &FM, CColor Color)
{
	if( !CORE->GetDebugGUIManager()->GetLogRender()->GetVisible() )
	{
		uint32 dx = 10;
		uint32 dy = 35;

		RenderQuad(RM, dx, dy);

		if( GetActive() )
		{
			if( !m_IsChild )
			{
				TVectorResources l_Resources = GetResourcesVector();
				uint16 l_ResourcesCount = l_Resources.size();
				for(uint16 i=0; i<l_ResourcesCount; ++i)
				{
					if( m_NowIndexInVector != i )
					{
						dy += FM.DrawDefaultText(dx, dy, Color, l_Resources[i]->GetName().c_str() );
					}
					else
					{
						dy += FM.DrawDefaultText(dx, dy, Color, "> %s", l_Resources[i]->GetName().c_str() );
					}
				}
			}
			else
			{
				TVectorResources l_Resources = GetResourcesVector();
				dy = l_Resources[m_NowIndexInVector]->Render(FM, l_Resources[m_NowIndexInVector]->GetName(), dx, dy, Color);
			}

			dy += FM.DrawDefaultText(dx, dy, Color, "__________________");
			dy += FM.DrawDefaultText(dx, dy, Color, "LSHIFT + F2 to hide");
		}
		else
		{
			dy += FM.DrawDefaultText(dx, dy, Color, "Press LSHIFT + F2 to view the Modifiers");
		}
	}
}

void CModifierManager::RenderQuad(CRenderManager &RM, const uint16 dx, const uint16 dy)
{
	CColor backgroundColor = CColor(0.35f, 0.35f, 0.35f);
		backgroundColor.SetAlpha(0.8f);
		CColor edgesColor = colBLACK;
		edgesColor.SetAlpha(0.7f);
		
		Vect2i screen = RM.GetScreenSize();
		RM.DrawRectangle2D(Vect2i(dx, dy), m_SizeRectangle.x, m_SizeRectangle.y, backgroundColor, 2, 2, edgesColor);
}

bool CModifierManager::Load(const std::string &FileName)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CModifierManager::Load->Cargando los modificadores de efectos.");

	m_FileName = FileName;
	return LoadFile();
}

bool CModifierManager::Reload()
{
	CleanUp();
	return LoadFile();
}

bool CModifierManager::LoadFile()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CModifierManager::LoadFile->Error al intentar leer el archivo de modifiers: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_Node = newFile["modifiers"];
	if( l_Node.Exists() )
	{
		uint16 l_Count = l_Node.GetNumChildren();
		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_Node(i).GetName();
			if( l_Type == "modifier" )
			{
				CModifier *l_Modifier = new CModifier( l_Node(i) );
				if( l_Modifier != NULL )
				{
					AddResource( l_Modifier->GetName(), l_Modifier );
				}
			}
		}
	}
	else
	{
		return false;
	}

	AnalizeSizeInfo();
	return true;
}
	
void CModifierManager::MoveToNextModifier()
{
	if( !CORE->GetDebugGUIManager()->GetLogRender()->GetVisible() )
	{
		if( m_IsChild )
		{
			TVectorResources l_Resources = GetResourcesVector();
			l_Resources[m_NowIndexInVector]->MoveToNextParam();
		}
		else
		{
			uint16 l_Position = m_NowIndexInVector + 1;
			if( l_Position < GetResourcesVector().size() )
			{
				m_NowIndexInVector = l_Position;
			}
		}
	}
}

void CModifierManager::MoveToPreviousModifier()
{
	if( !CORE->GetDebugGUIManager()->GetLogRender()->GetVisible() )
	{
		if( m_IsChild )
		{
			TVectorResources l_Resources = GetResourcesVector();
			l_Resources[m_NowIndexInVector]->MoveToPreviousParam();
		}
		else
		{
			if( m_NowIndexInVector > 0 )
			{
				--m_NowIndexInVector;
			}
		}
	}
}

void CModifierManager::GoToModifier()
{
	if( !CORE->GetDebugGUIManager()->GetLogRender()->GetVisible() )
	{
		TVectorResources l_Resources = GetResourcesVector();
		l_Resources[m_NowIndexInVector]->ResetToMove();
		m_IsChild = true;

		AnalizeSizeInfo();
	}
}

void CModifierManager::GoToRoot()
{
	if( !CORE->GetDebugGUIManager()->GetLogRender()->GetVisible() )
	{
		m_IsChild = false;
		AnalizeSizeInfo();
	}
}

void CModifierManager::AddValueToModifier()
{
	if( m_IsChild && !CORE->GetDebugGUIManager()->GetLogRender()->GetVisible() )
	{
		TVectorResources l_Resources = GetResourcesVector();
		l_Resources[m_NowIndexInVector]->AddValue();
	}
}

void CModifierManager::SubsValueToModifier()
{
	if( m_IsChild && !CORE->GetDebugGUIManager()->GetLogRender()->GetVisible() )
	{
		TVectorResources l_Resources = GetResourcesVector();
		l_Resources[m_NowIndexInVector]->SubsValue();
	}
}

//Calcula el tamaño del Quad de fondo
void CModifierManager::AnalizeSizeInfo()
{
	CFontManager *FM = CORE->GetFontManager();

	std::string l_sInfo;
	bool l_Visible = GetVisible();
	if( l_Visible )
	{
		//Si se ve la información
		baseUtils::FormatSrting (l_sInfo, "---------------------------------------" );
	}
	else
	{
		//La información está oculta
		baseUtils::FormatSrting (l_sInfo, "Press LSHIFT + F2 to view the Modifiers" );
	}
	m_SizeRectangle.x = FM->SizeX(l_sInfo.c_str());
	m_SizeRectangle.y = FM->SizeY(l_sInfo.c_str());

	if( l_Visible )
	{
		if( m_IsChild )
		{
			//Calcula el tamaño de Y para un modificador concreto
			TVectorResources l_Resources = GetResourcesVector();
			uint16 l_NumOfElems = l_Resources[m_NowIndexInVector]->GetNumOfParams();
			if( l_NumOfElems > 0 )
			{
				m_SizeRectangle.y *= (l_NumOfElems + 4); //+4 porqué hay la cabecera del modifier y el mensaje de ocultación
			}
		}
		else
		{
			//Calcula el tamaño de Y para la lista de modificadores
			uint16 l_NumOfElems = GetResourcesVector().size();
			if( l_NumOfElems > 0 )
			{
				m_SizeRectangle.y *= (l_NumOfElems + 2); //+2 por el mensaje de ocultación
			}
		}
	}
}

void CModifierManager::SetVisible(bool visible)
{
	if( !CORE->GetDebugGUIManager()->GetLogRender()->GetVisible() )
	{
		SetActive(visible);
		AnalizeSizeInfo();
	}
}