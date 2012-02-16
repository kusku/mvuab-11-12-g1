#include "ModifierManager.h"
#include "Fonts\FontManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CModifierManager::CModifierManager()
	: m_NowIndexInVector(0)
	, m_FileName("")
	, m_IsChild(false)
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

void CModifierManager::Render(CFontManager &FM, CColor Color)
{
	if( GetActive() )
	{
		uint32 dx = 20;
		uint32 dy = 50;

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
			l_Resources[m_NowIndexInVector]->Render(FM, l_Resources[m_NowIndexInVector]->GetName(), dx, dy, Color);
		}
	}
}

bool CModifierManager::Load(const std::string &FileName)
{
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

	return true;
}

void CModifierManager::MoveToNextModifier()
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

void CModifierManager::MoveToPreviousModifier()
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

void CModifierManager::GoToModifier()
{
	TVectorResources l_Resources = GetResourcesVector();
	l_Resources[m_NowIndexInVector]->ResetToMove();
	m_IsChild = true;
}

void CModifierManager::GoToRoot()
{
	m_IsChild = false;
}

void CModifierManager::AddValueToModifier()
{
	if( m_IsChild )
	{
		TVectorResources l_Resources = GetResourcesVector();
		l_Resources[m_NowIndexInVector]->AddValue();
	}
}

void CModifierManager::SubsValueToModifier()
{
	if( m_IsChild )
	{
		TVectorResources l_Resources = GetResourcesVector();
		l_Resources[m_NowIndexInVector]->SubsValue();
	}
}