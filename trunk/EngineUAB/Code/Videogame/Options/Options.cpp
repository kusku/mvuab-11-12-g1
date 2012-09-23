#include "Options.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

COptions::COptions()
	: m_Filename("")
	, m_bCustomInvertedMouse(false)
	, m_fCustomEffectsVolume(100.f)
	, m_fCustomGenericVolume(100.f)
	, m_fCustomMusicVolume(100.f)
	, m_fDefaultEffectsVolume(100.f)
	, m_fDefaultGenericVolume(100.f)
	, m_fDefaultMusicVolume(100.f)
	, m_bDefaultInvertedMouse(false)
{

}

COptions::~COptions()
{

}

bool COptions::Load(const std::string &_Filename)
{
	m_Filename = _Filename;
	return LoadFile();
}

bool COptions::Reload()
{
	return LoadFile();
}

bool COptions::LoadFile()
{
	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_Filename.c_str ( ) ) )
	{
		std::string msg_error = "COptions::LoadFile->Error al leer el fichero de opciones: " + m_Filename;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	CXMLTreeNode l_RootNode = newFile ["Options"];
	if ( l_RootNode.Exists() )
	{
		uint8 l_TotalNodes = l_RootNode.GetNumChildren();
		for ( uint8 i = 0; i < l_TotalNodes; ++i )
		{
			CXMLTreeNode l_OptionNode = l_RootNode(i);
			std::string l_TypeOptions = l_OptionNode.GetName();
			uint8 l_NumNodes = l_OptionNode.GetNumChildren();

			for(uint8 j=0; j<l_NumNodes; ++j)
			{
				std::string l_OptionField = l_OptionNode(j).GetName();
				if( l_OptionField == "generic_volume")
				{
					float volume = l_OptionNode(j).GetFloatKeyword("generic_volume", 100.f);
					if(l_TypeOptions == "Default")
						m_fDefaultGenericVolume = volume;
					else
						m_fCustomGenericVolume = volume;
				}
				else if( l_OptionField == "music_volume" )
				{
					float volume = l_OptionNode(j).GetFloatKeyword("music_volume", 100.f);
					if(l_TypeOptions == "Default")
						m_fDefaultMusicVolume = volume;
					else
						m_fCustomMusicVolume = volume;
				}
				else if( l_OptionField == "effect_volume" )
				{
					float volume = l_OptionNode(j).GetFloatKeyword("effect_volume", 100.f);
					if(l_TypeOptions == "Default")
						m_fDefaultEffectsVolume = volume;
					else
						m_fCustomEffectsVolume = volume;
				}
				else if( l_OptionField == "mouse_invert" )
				{
					bool value = l_OptionNode(j).GetBoolKeyword("mouse_invert", false);
					if(l_TypeOptions == "Default")
						m_bCustomInvertedMouse = value;
					else
						m_bCustomInvertedMouse = value;
				}
			}

		}
	}

	return true;
}

void COptions::Save()
{
	CXMLTreeNode l_Node;
	l_Node.StartNewFile( m_Filename.c_str() );

	l_Node.StartElement( "Options" );
	l_Node.StartElement("Default");

	l_Node.WriteFloatKeyword("generic_volume", m_fDefaultGenericVolume);
	l_Node.WriteFloatKeyword("music_volume", m_fDefaultMusicVolume);
	l_Node.WriteFloatKeyword("effect_volume", m_fDefaultEffectsVolume);
	l_Node.WriteBoolKeyword("mouse_invert", m_bDefaultInvertedMouse);

	l_Node.EndElement(); //Default
	l_Node.StartElement("Custom");

	l_Node.WriteFloatKeyword("generic_volume", m_fCustomGenericVolume);
	l_Node.WriteFloatKeyword("music_volume", m_fCustomMusicVolume);
	l_Node.WriteFloatKeyword("effect_volume", m_fCustomEffectsVolume);
	l_Node.WriteBoolKeyword("mouse_invert", m_bCustomInvertedMouse);

	l_Node.EndElement(); //Custom
	l_Node.EndElement(); //Options
	l_Node.EndNewFile();
}