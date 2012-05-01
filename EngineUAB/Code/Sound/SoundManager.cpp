#include "SoundManager.h"

// ----- openAL -------------//
#include "alc.h"
#include "alut.h"
// --------------------------//

#include "Billboard\Billboard.h"
#include "Textures\Texture.h"
#include "Object3D.h"

#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CSoundManager::CSoundManager ( void )
	: m_szFileName		( "" )
	, m_fSoundVolume	( 1.f )
	, m_bIsOk			( false )
	, m_bSoundON		( true )
	, m_bPause			( false ) 
	, m_pListener		( NULL )
{
	Init();
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------

bool CSoundManager::Init ( void )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CSoundManager::Init-->Iniciating drivers and sounds." );
	m_bIsOk = _initAL();

	if ( !m_bIsOk )
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "CSoundManager::Init-->Error iniciating openAL." );
		Release();
	}
	else
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "CSoundManager::Init-->Working." );
	}

	return m_bIsOk;
}
   
void CSoundManager::Done ( void )
{
	if ( IsOk() )
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CSoundManager::Release ( void )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CSoundManager::Release-->Stopping Sound Manager." );
	
	std::vector<CBillboard*>::iterator l_It = m_BillboardVector.begin();
	std::vector<CBillboard*>::iterator l_End = m_BillboardVector.end();
		
	for ( ; l_It != l_End; l_It++ )
		CHECKED_DELETE(*l_It);

	m_BillboardVector.clear();

	_finalizeAL();
	
	LOGGER->AddNewLog ( ELL_INFORMATION, "CSoundManager::Release-->Sound Manager stopped." );
}

//----------------------------------------------------------------------------
// Load : Per carregar un fitxer XML amb tots els scrits
//----------------------------------------------------------------------------
bool CSoundManager::Load ( const std::string& _XMLSoundsFile )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CSoundManager::Load-->Loading sounds." );
	m_szFileName = _XMLSoundsFile;
	return LoadSounds();
}

//----------------------------------------------------------------------------
// Update : Per actualitzar l'escena i realitzar les físiques i simulacions
//----------------------------------------------------------------------------
void CSoundManager::Update ( float _ElapsedTime )
{
	std::vector<uint32> l_Finished;
	std::map<uint32, CLerpAnimator1D>::iterator l_It = m_SourcesFadeInOutMap.begin ();
	std::map<uint32, CLerpAnimator1D>::iterator l_End = m_SourcesFadeInOutMap.end ();	

	for ( ; l_It != l_End; l_It++ )
	{
		float l_Value = 0.f;
		if ( l_It->second.Update ( _ElapsedTime, l_Value ) )
		{
			// Ha acabado de realizar el FadeIn/FadeOut --> Sacar del map
			l_Finished.push_back ( l_It->first );
		}
		SetSourceGain ( l_It->first, l_Value );
	}
		
	for ( uint32 l_Cont = 0; l_Cont < l_Finished.size(); l_Cont++ )
	{
		std::map<uint32, CLerpAnimator1D>::iterator l_It = m_SourcesFadeInOutMap.find ( l_Finished[l_Cont] );
		if ( l_It != m_SourcesFadeInOutMap.end() )
		{
			m_SourcesFadeInOutMap.erase ( l_It );
		}
	}

	if ( m_pListener != NULL )
	{
		SetListenerPosition( m_pListener->GetPosition () );
		SetListenerVelocity( Vect3f(0.f,0.f,0.f) );
		SetListenerOrientation( CORE->GetCamera()->GetEye().Normalize(), CORE->GetCamera()->GetVecUp().Normalize() );

		//Vect3f dir(cos(alpha),0.f,sin(alpha));	
		//SetListenerOrientation( m_pCamera->GetEye().Normalize(), m_pCamera->GetVecUp().Normalize() );
	}
	else
	{
		SetListenerPosition( CORE->GetCamera()->GetEye() );
		SetListenerVelocity( Vect3f(0.f,0.f,0.f) );
		SetListenerOrientation( CORE->GetCamera()->GetEye().Normalize(), CORE->GetCamera()->GetVecUp().Normalize() );
	}
	
	for ( uint16 l_Index = 0; l_Index < m_SourcesVector.size(); l_Index ++ )
	{
		Vect3f l_Pos;
		if ( GetSourcePosition ( l_Index, l_Pos ) )
		{
			m_BillboardVector[l_Index]->SetPosition( l_Pos );
			m_BillboardVector[l_Index]->Update( CORE->GetCamera(), 0.f );
		}
	}
}

//----------------------------------------------------------------------------
// Render : Renderitzar altaveus en mode debug
//----------------------------------------------------------------------------
void CSoundManager::Render ( CRenderManager &_RM )
{
	for ( uint16 l_Index = 0; l_Index < m_BillboardVector.size(); l_Index ++ )
		m_BillboardVector[l_Index]->Render( _RM );
}

// -----------------------------------------
//					MÈTODES
// -----------------------------------------

//----------------------------------------------------------------------------
// Reload : Per recarregar el XML
//----------------------------------------------------------------------------
bool CSoundManager::Reload ( void )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CSoundManager::Reload--> Reloading sounds.");
	Release ();
	return LoadSounds ();
}

//----------------------------------------------------------------------------
// LoadSounds : Carrega realment el XML
//----------------------------------------------------------------------------
bool CSoundManager::LoadSounds ( void  )
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CSoundManager::LoadSounds --> Loading sounds." );
	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_szFileName.c_str ( ) ) )
	{
		std::string msg_error = "CSoundManager::LoadSounds --> Error when trying to load the sounds object file: " + m_szFileName;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	std::map<tSoundFile, tIdBuffer> l_FilesLoaded;
	bool isOK = false;

	CXMLTreeNode l_NodePare = newFile ["Sounds"];
	if ( l_NodePare.Exists ( ) )
	{
		uint16 l_TotalNodes = l_NodePare.GetNumChildren ();
		// Recorro els sons 
		for ( uint16 i = 0; i < l_TotalNodes; ++i )
		{
			std::string l_Node = l_NodePare(i).GetName();
			if ( l_Node == "Sound" ) 
			{
				CXMLTreeNode l_TriggerNode = l_NodePare(i);

				tSoundFile l_File = static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "path", "" ) );
				std::string l_Id  = static_cast<std::string> ( l_TriggerNode.GetPszProperty ( "soundId", "" ) );
				
				std::map<tSoundFile, tIdBuffer>::iterator l_It = l_FilesLoaded.find( l_Id );
				if ( l_It != l_FilesLoaded.end() )
				{
					std::string msg_error = "CSoundManager::LoadSounds --> The sound " + l_File + " was already loaded";
					LOGGER->AddNewLog( ELL_WARNING, msg_error.c_str() );
					return false;
				}
				else 
				{
					tIdBuffer l_IdBuffer;
					if ( _loadSound ( l_File, l_IdBuffer ) )
					{
						m_BuffersMap.insert (std::pair<std::string,tIdBuffer>(l_Id, l_IdBuffer)); 
						std::string msg_error = "CSoundManager::LoadSounds --> The sound " + l_File + " was loaded";
						LOGGER->AddNewLog( ELL_INFORMATION, msg_error.c_str() );
					}
					else
					{
						std::string msg_error = "CSoundManager::LoadSounds --> The sound " + l_File + " was not loaded";
						LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
					}
				}
			}
			else if ( l_Node != "comment" ) 
			{
				std::string msg_error = "CSoundManager::LoadSounds --> Error when trying to load a node : " + l_Node + " from file: " + m_szFileName;
				LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			}
		}
	}
	return true;
}

// -----------------------------------------
//				METODOS GENERALES
// -----------------------------------------
//--------------------------------------------------------------------------------------
// Reset : Para todos los sonidos y limpia buffers(sonidos) i sources (altavoces)
//--------------------------------------------------------------------------------------
void CSoundManager::Reset ( void )
{
	Stop();
	_clear();
}

//---------------------------------------------------------------------------------------------------
// Pause : Determina si hace pausa/play de todos los sonidos segun la variable global "m_bPause"
//---------------------------------------------------------------------------------------------------
void CSoundManager::Pause ( void )
{
	std::vector<tInfoSource>::iterator l_It = m_SourcesVector.begin();
	std::vector<tInfoSource>::iterator l_End = m_SourcesVector.end();
	for ( ; l_It < l_End; l_It++ ) 
	{
		if ( m_bPause ) 
		{
			alSourcePlay ( (*l_It).m_uSource );
		}
		else
		{
			alSourcePause ( (*l_It).m_uSource );
		}
	}
	m_bPause = !m_bPause;
}

//---------------------------------------------------------------------------------------------------
// Stop : Para todos los sonidos de los altavoces
//---------------------------------------------------------------------------------------------------
void CSoundManager::Stop ( void )
{
	std::vector<tInfoSource>::iterator l_It = m_SourcesVector.begin();
	std::vector<tInfoSource>::iterator l_End = m_SourcesVector.end();
	for ( ; l_It < l_End; l_It++ ) 
	{
		alSourceStop( l_It->m_uSource );
	}
}

//-------------------------------------------------------------------------------------------
// PlayAction2D : Ejecuta un play 2D en un source que encuentre libre, indicando
//			la acción y si se repetirá
//-------------------------------------------------------------------------------------------	 
bool CSoundManager::PlayAction2D ( const std::string& _Action, float _Gain )
{
	if ( m_bSoundON )
	{
		std::map<tAction, tIdBuffer>::const_iterator l_It = m_BuffersMap.find ( _Action );
		if ( l_It == m_BuffersMap.end() )
		{
			std::string l_Description = "Action " + _Action + " not found";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> %s", l_Description.c_str() );
			return false;
		}

		// Obtinc el buffer trobat
		tIdBuffer l_IdBuffer = (*l_It).second;

		// Obtinc un altaveu lliure
		int l_SourceIndex;
		l_SourceIndex = _getSource( false );

		ALfloat l_Vector[3] = { 0.0, 0.0, 0.0 };		// Si volem que sigui 2D cal posar-ho a 0.0f!!

		// vinculo un buffer al altaveu i l'executo
		alSourcei ( m_SourcesVector[l_SourceIndex].m_uSource, AL_BUFFER, l_IdBuffer );
		
		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_BUFFER. %s", l_Description.c_str());
			return false;
		}
		
		alSourcei ( m_SourcesVector[l_SourceIndex].m_uSource, AL_SOURCE_RELATIVE, AL_TRUE );
		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_SOURCE_RELATIVE. %s", l_Description.c_str());
			return false;
		}

		alSourcef ( m_SourcesVector[l_SourceIndex].m_uSource, AL_PITCH, 1.0 );
		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_PITCH. %s", l_Description.c_str());
			return false;
		}

		alSourcef ( m_SourcesVector[l_SourceIndex].m_uSource, AL_GAIN, 1.0 );

		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_GAIN. %s", l_Description.c_str());
			return false;
		}
		alSourcefv( m_SourcesVector[l_SourceIndex].m_uSource, AL_POSITION, l_Vector );

		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_POSITION. %s", l_Description.c_str());
			return false;
		}

		alSourcefv( m_SourcesVector[l_SourceIndex].m_uSource, AL_VELOCITY, l_Vector );

		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_VELOCITY. %s", l_Description.c_str());
			return false;
		}

		alSourcefv( m_SourcesVector[l_SourceIndex].m_uSource, AL_DIRECTION, l_Vector );

		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_DIRECTION. %s", l_Description.c_str());
			return false;
		}

		alSourcei ( m_SourcesVector[l_SourceIndex].m_uSource, AL_LOOPING, AL_FALSE );
		
		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = "Error attaching buffer to source (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_LOOPING. %s", l_Description.c_str());
			return false;
		}

		// Faig el Play!
		alSourcePlay ( m_SourcesVector[l_SourceIndex].m_uSource );

		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = "Error playing buffer to source (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> %s", l_Description.c_str());
			return false;
		}
	}

	return true;
}

//-------------------------------------------------------------------------------------------
// PlayAction3D : Ejecuta un play 3D en un source que encuentre libre, indicando
//			la acción y si se repetirá
//-------------------------------------------------------------------------------------------	
bool CSoundManager::PlayAction3D ( const std::string& _Action, const Vect3f& _Position, float _Gain )
{
	if ( m_bSoundON )
	{
		std::map<tAction, tIdBuffer>::const_iterator l_It = m_BuffersMap.find ( _Action );
		if ( l_It == m_BuffersMap.end() )
		{
			std::string l_Description = "Action " + _Action + " not found";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction3D --> %s", l_Description.c_str() );
			return false;
		}

		// Obtinc el buffer trobat
		tIdBuffer l_IdBuffer = (*l_It).second;

		// Obtinc un altaveu lliure
		int l_SourceIndex;
		l_SourceIndex = _getSource( false );

		if ( l_SourceIndex == -1 ) 
		{
			std::string l_Description = "Error getting/creating source";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction3D --> %s", l_Description.c_str() );
			return false;
		}

		// Aquesta posició, velocitat i direcció
		ALfloat l_Pos[3] = { _Position.x, _Position.y, _Position.z };		// Si volem que sigui 2D cal posar-ho a 0.0f!!
		ALfloat l_Vel[3] = { 0.0, 0.0, 0.0 };
		ALfloat l_Dir[3] = { 0.0, 0.0, 0.0 };

		// vinculo un buffer al altaveu i l'executo
		alSourcei ( m_SourcesVector[l_SourceIndex].m_uSource, AL_BUFFER, l_IdBuffer );
		
		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_BUFFER. %s", l_Description.c_str());
			return false;
		}
		
		alSourcef ( m_SourcesVector[l_SourceIndex].m_uSource, AL_PITCH, 1.0 );
		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_PITCH. %s", l_Description.c_str());
			return false;
		}

		alSourcef ( m_SourcesVector[l_SourceIndex].m_uSource, AL_GAIN, 1.0 );

		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_GAIN. %s", l_Description.c_str());
			return false;
		}
		alSourcefv( m_SourcesVector[l_SourceIndex].m_uSource, AL_POSITION, l_Pos );

		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_POSITION. %s", l_Description.c_str());
			return false;
		}

		alSourcefv( m_SourcesVector[l_SourceIndex].m_uSource, AL_VELOCITY, l_Vel );

		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = _getALErrorString( l_Error ) ;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_VELOCITY. %s", l_Description.c_str());
			return false;
		}
		
		//alSourcefv( m_SourcesVector[l_SourceIndex].m_uSource, AL_DIRECTION, l_Dir );

		//// Check for an error.
		//if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		//{
		//	std::string l_Description = _getALErrorString( l_Error ) ;
		//	LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_DIRECTION. %s", l_Description.c_str());
		//	return false;
		//}

		alSourcei ( m_SourcesVector[l_SourceIndex].m_uSource, AL_LOOPING, AL_FALSE );
		
		// Check for an error.
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = "Error attaching buffer to source (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_LOOPING. %s", l_Description.c_str());
			return false;
		}
		
		// Faig el Play!
		alSourcePlay ( m_SourcesVector[l_SourceIndex].m_uSource );

		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = "Error playing buffer to source (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> %s", l_Description.c_str());
			return false;
		}
	}

	return true;
}

//---------------------------------------------------------------------------------------------------
// SetGain : Setea el volumen general
//---------------------------------------------------------------------------------------------------
void CSoundManager::SetGain ( float _Gain )
{
	alListenerf( AL_GAIN, _Gain );
	
	// Check for an error.
    ALenum l_Error;
	if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
	{
		std::string l_Description = "Error setting gain (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::SetGain --> %s", l_Description.c_str());
		return;
	}
}

//---------------------------------------------------------------------------------------------------
// GetGain : Obtiene el sonido general
//---------------------------------------------------------------------------------------------------
float CSoundManager::GetGain ( void )
{
	float l_Gain;
	alGetListenerf( AL_GAIN, &l_Gain ); 

	// Check for an error.
    ALenum l_Error;
	if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
		return l_Gain;
	else
	{
		std::string l_Description = "Error getting gain (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::GetGain --> %s", l_Description.c_str());
		return false;
	}
}
     

// -----------------------------------------
//				METODOS SOURCE
// -----------------------------------------

//---------------------------------------------------------------------------------------------------
// CreateSource : Obtiene el indice del vector de un source libre
//---------------------------------------------------------------------------------------------------
uint32 CSoundManager::CreateSource ( void )
{
	return _getSource ( true );
}

//---------------------------------------------------------------------------------------------------
// CreateSource : Elimina un source segun el indice del vector
//---------------------------------------------------------------------------------------------------
bool CSoundManager::DeleteSource ( uint32 _Source )
{
	if ( _Source < m_SourcesVector.size () )
	{
		StopSource ( _Source );
		m_SourcesVector[_Source].m_bReserved = false;
	}

	// Check for an error.
    ALenum l_Error;
	if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
		return true;
	else
	{
		std::string l_Description = "Error deleting source (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::DeleteSource --> %s", l_Description.c_str());
		return false;
	}
}

//-------------------------------------------------------------------------------------------
// PlaySource2D : Ejecuta un play 2D de un indice del vector de sources determinado, indicando
//			la acción y si se repetirá
//-------------------------------------------------------------------------------------------
bool CSoundManager::PlaySource2D ( uint32 _Source, const std::string& _Action, bool _Loop, float _Gain )
{
	if ( m_bSoundON )
	{
		if ( _Source < m_SourcesVector.size () && m_SourcesVector[_Source].m_bReserved ) 
		{
			ALboolean l_AlLoop;

			if ( _Loop )
				l_AlLoop = AL_TRUE;
			else
				l_AlLoop = AL_FALSE;
			
			StopSource ( _Source );
				
			if ( m_BuffersMap.find ( _Action ) == m_BuffersMap.end() )
			{
				std::string l_Description = "Action " + _Action + " not found";
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlaySource2D --> %s", l_Description.c_str() );
				return false;
			}

			ALfloat l_Vector[3] = { 0.0, 0.0, 0.0 };		// Si volem que sigui 2D cal posar-ho a 0.0f!!

			// vinculo un buffer al altaveu i l'executo
			alSourcei ( m_SourcesVector[_Source].m_uSource, AL_BUFFER, m_BuffersMap[_Action] );
		
			// Check for an error.
			ALenum l_Error;
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_BUFFER. %s", l_Description.c_str());
				return false;
			}
		
			alSourcei ( m_SourcesVector[_Source].m_uSource, AL_SOURCE_RELATIVE, AL_TRUE );
			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_SOURCE_RELATIVE. %s", l_Description.c_str());
				return false;
			}

			alSourcef ( m_SourcesVector[_Source].m_uSource, AL_PITCH, 1.0 );
			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_PITCH. %s", l_Description.c_str());
				return false;
			}

			alSourcef ( m_SourcesVector[_Source].m_uSource, AL_GAIN, 1.0 );

			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_GAIN. %s", l_Description.c_str());
				return false;
			}

			alSourcefv( m_SourcesVector[_Source].m_uSource, AL_POSITION, l_Vector );

			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_POSITION. %s", l_Description.c_str());
				return false;
			}

			alSourcefv( m_SourcesVector[_Source].m_uSource, AL_VELOCITY, l_Vector );

			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_VELOCITY. %s", l_Description.c_str());
				return false;
			}

			alSourcefv( m_SourcesVector[_Source].m_uSource, AL_DIRECTION, l_Vector );

			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_DIRECTION. %s", l_Description.c_str());
				return false;
			}

			alSourcei ( m_SourcesVector[_Source].m_uSource, AL_LOOPING, l_AlLoop );
		
			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = "Error attaching buffer to source (" + _getALErrorString( l_Error ) + ")";
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_LOOPING. %s", l_Description.c_str());
				return false;
			}
		
			// Faig el Play!
			alSourcePlay ( m_SourcesVector[_Source].m_uSource );

			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = "Error playing buffer to source (" + _getALErrorString( l_Error ) + ")";
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> %s", l_Description.c_str());
				return false;
			}
		}
		else
		{
			std::string l_Description = "Can't play soyuce. Invalid source " + _Source;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlaySource2D --> %s", l_Description.c_str());
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------
// PlaySource3D : Ejecuta un play 3D de un indice del vector de sources determinado, indicando
//			la acción y si se repetirá
//-------------------------------------------------------------------------------------------
bool CSoundManager::PlaySource3D ( uint32 _Source, const std::string& _Action, bool _Loop, float _Gain )
{
	if ( m_bSoundON )
	{
		if ( _Source < m_SourcesVector.size () && m_SourcesVector[_Source].m_bReserved ) 
		{
			ALboolean l_AlLoop;

			if ( _Loop )
				l_AlLoop = AL_TRUE;
			else
				l_AlLoop = AL_FALSE;
			
			StopSource ( _Source );
				
			if ( m_BuffersMap.find ( _Action ) == m_BuffersMap.end() )
			{
				std::string l_Description = "Action " + _Action + " not found";
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlaySource2D --> %s", l_Description.c_str() );
				return false;
			}

			// vinculo un buffer al altaveu i l'executo
			alSourcei ( m_SourcesVector[_Source].m_uSource, AL_BUFFER, m_BuffersMap[_Action] );
		
			// Check for an error.
			ALenum l_Error;
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_BUFFER. %s", l_Description.c_str());
				return false;
			}
		
			alSourcef ( m_SourcesVector[_Source].m_uSource, AL_PITCH, 1.0 );
			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_PITCH. %s", l_Description.c_str());
				return false;
			}

			alSourcef ( m_SourcesVector[_Source].m_uSource, AL_GAIN, 1.0 );

			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = _getALErrorString( l_Error ) ;
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_GAIN. %s", l_Description.c_str());
				return false;
			}

 			alSourcei ( m_SourcesVector[_Source].m_uSource, AL_LOOPING, l_AlLoop );
		
			// Check for an error.
			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = "Error attaching buffer to source (" + _getALErrorString( l_Error ) + ")";
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> AL_LOOPING. %s", l_Description.c_str());
				return false;
			}
		
			// Faig el Play!
			alSourcePlay ( m_SourcesVector[_Source].m_uSource );

			if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
			{
				std::string l_Description = "Error playing buffer to source (" + _getALErrorString( l_Error ) + ")";
				LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlayAction2D --> %s", l_Description.c_str());
				return false;
			}
		}
		else
		{
			std::string l_Description = "Can't play soyuce. Invalid source " + _Source;
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PlaySource3D --> %s", l_Description.c_str());
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------
// PauseSource : Pausamos un altavoz concreto. Si este estava en pausa lo ponemos en Play.
//				Si estava en Stop u otro estado retornamos false --> no hacemos nada
//-------------------------------------------------------------------------------------------
bool CSoundManager::PauseSource ( uint32 _Source )
{
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved )
	{
		ALint l_State;
		
		// Obtenemos el estado actual
		alGetSourcei( m_SourcesVector[_Source].m_uSource,AL_SOURCE_STATE, &l_State );

		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
		{
			std::string l_Description = "Error getting state (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PauseSource --> %s", l_Description.c_str());
			return false;
		}

		if ( l_State == AL_PLAYING ) 
			alSourcePause ( m_SourcesVector[_Source].m_uSource );
		else if ( l_State == AL_PAUSED ) 
			alSourcePlay ( m_SourcesVector[_Source].m_uSource );

		// Check for an error.
		if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
			return true;
		else
		{
			std::string l_Description = "Error pausing source (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::PauseSource --> %s", l_Description.c_str());
			return false;
		}
	}
	else
		return true;
}

//--------------------------------------------------------------------------------------
// StopSource : Paramos un altavoz concreto
//--------------------------------------------------------------------------------------
bool CSoundManager::StopSource ( uint32 _Source )
{
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved )
	{
		ALint l_State;
		
		alGetSourcei( m_SourcesVector[_Source].m_uSource,AL_SOURCE_STATE, &l_State );
		if ( l_State == AL_PLAYING || l_State == AL_PAUSED )
		{
			alSourceStop(m_SourcesVector[_Source].m_uSource);
		}
	
		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
			return true;
		else
		{
			std::string l_Description = "Error stopping source (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::StopSource --> %s", l_Description.c_str());
			return false;
		}
	}
	else 
		return true;
}

//--------------------------------------------------------------------------------------
// SetSourceGain : Damos el volumen a un altavoz concreto
//--------------------------------------------------------------------------------------
bool CSoundManager::SetSourceGain ( uint32 _Source, float _InGain )
{
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved )
	{
		alSourcef ( m_SourcesVector[_Source].m_uSource, AL_GAIN, _InGain );

		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
			return true;
		else
		{
			std::string l_Description = "Error setting source gain (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::SetSourceGain --> %s", l_Description.c_str());
			return false;
		}
	}
	else 
		return true;
}

//--------------------------------------------------------------------------------------
// GetSourceGain : Obtenemos el volumen de un altavoz concreto
//--------------------------------------------------------------------------------------
bool CSoundManager::GetSourceGain ( uint32 _Source, float& _OutGain )
{
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved)
	{
		float l_Gain; 

		alGetSourcef ( m_SourcesVector[_Source].m_uSource, AL_GAIN, &l_Gain );

		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
		{
			_OutGain = ( float ) l_Gain;
			return true;
		}
		else
		{
			std::string l_Description = "Error getting source gain (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::GetSourceGain --> %s", l_Description.c_str());
			return false;
		}
	}
	else
		return true;
}

//--------------------------------------------------------------------------------------
// SetSourcePosition : Colocamos la posición de un altavoz concreto
//--------------------------------------------------------------------------------------
bool CSoundManager::SetSourcePosition ( uint32 _Source, const Vect3f& _InPosition )
{
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved )
	{
		ALfloat l_Pos[3];
		l_Pos[0] = _InPosition.x;
		l_Pos[1] = _InPosition.y;
		l_Pos[2] = _InPosition.z;

		alSourcefv ( m_SourcesVector[_Source].m_uSource, AL_POSITION, l_Pos );
	
		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
			return true;
		else
		{
			std::string l_Description = "Error setting source position (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::SetSourcePosition --> %s", l_Description.c_str());
			return false;
		}
	}
	else
		return true;
}

//--------------------------------------------------------------------------------------
// GetSourcePosition : Obtenemos la posición de un altavoz concreto
//--------------------------------------------------------------------------------------
bool CSoundManager::GetSourcePosition ( uint32 _Source, Vect3f& _OutPosition )
{
	ALfloat l_Pos[3];
		
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved)
	{
		alGetSourcefv ( m_SourcesVector[_Source].m_uSource, AL_POSITION, l_Pos );
	
		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
		{
			_OutPosition.x = l_Pos[0];
			_OutPosition.y = l_Pos[1];
			_OutPosition.z = l_Pos[2];
			return true;
		}
		else
		{
			std::string l_Description = "Error getting position source (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::GetSourcePosition --> %s", l_Description.c_str());
			return false;
		}
	}
	else 
		return true;
}

//--------------------------------------------------------------------------------------
// SetSourceVelocity : Colocamos la velocidad de un altavoz concreto
//--------------------------------------------------------------------------------------
bool CSoundManager::SetSourceVelocity ( uint32 _Source, const Vect3f& _InVelocity )
{
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved)
	{
		ALfloat l_Vel[3];
		l_Vel[0] = _InVelocity.x;
		l_Vel[1] = _InVelocity.y;
		l_Vel[2] = _InVelocity.z;

		alSourcefv ( m_SourcesVector[_Source].m_uSource, AL_POSITION, l_Vel );
	
		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
			return true;
		else
		{
			std::string l_Description = "Error setting source velocity (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::SetSourceVelocity --> %s", l_Description.c_str());
			return false;
		}
	}
	else 
		return true;
}

//--------------------------------------------------------------------------------------
// GetSourceVelocity : Obtenemos la velocidad de un altavoz concreto
//--------------------------------------------------------------------------------------
bool CSoundManager::GetSourceVelocity ( uint32 _Source, Vect3f& _OutVelocity )
{
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved)
	{
		ALfloat l_Vel[3];

		alGetSource3f ( _Source, AL_POSITION, &l_Vel[0], &l_Vel[1], &l_Vel[2] );

		// Check for an error.
		ALenum l_Error;
		if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
		{
			_OutVelocity.x = l_Vel[0];
			_OutVelocity.y = l_Vel[1];
			_OutVelocity.z = l_Vel[2];
			return true;
		}
		else
		{
			std::string l_Description = "Error getting source velocity (" + _getALErrorString( l_Error ) + ")";
			LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::GetSourceVelocity --> %s", l_Description.c_str());
			return false;
		}
	}
	else
		return true;
}

//--------------------------------------------------------------------------------------
// FadeInSource : Crea un efecto de Fade In de un source concreto
//--------------------------------------------------------------------------------------
bool CSoundManager::FadeInSource ( uint32 _Source, float _TotalTime, float _FinalGain, ETypeFunction _Type )
{	
	bool l_IsOK = false;
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved)
	{
		//Primero de todo miramos que no haya ya un FadeIn/Out del source en cuestion:
		if ( m_SourcesFadeInOutMap.find( _Source ) == m_SourcesFadeInOutMap.end() )
		{
			CLerpAnimator1D l_Animator;
			l_Animator.SetValues( 0.f, _FinalGain, _TotalTime, _Type );
			m_SourcesFadeInOutMap.insert(std::pair<uint32,CLerpAnimator1D> ( _Source, l_Animator ) );
			l_IsOK = true;
		}
		else
		{
			std::string l_Description = "Ya se esta realizando un FadeIn/Out con el source" + _Source;
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::FadeInSource --> %s", l_Description.c_str());
			l_IsOK =  false;
		}
	}
	else
	{
		std::string l_Description = "Can't do FadeIn with source. Invalid source " + _Source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::FadeInSource --> %s", l_Description.c_str());
		l_IsOK =  false;
	}
	return l_IsOK;
}

//--------------------------------------------------------------------------------------
// FadeOutSource : Crea un efecto de Fade Out de un source concreto
//--------------------------------------------------------------------------------------
bool CSoundManager::FadeOutSource ( uint32 _Source, float _TotalTime, ETypeFunction _Type )
{
	bool l_IsOK = false;
	if ( _Source < m_SourcesVector.size() && m_SourcesVector[_Source].m_bReserved)
	{
		//Primero de todo miramos que no haya ya un FadeIn/Out del source en cuestion:
		if ( m_SourcesFadeInOutMap.find( _Source ) == m_SourcesFadeInOutMap.end() )
		{
			CLerpAnimator1D l_Animator;
			float l_CurrentGain = 0.f;
			GetSourceGain( _Source, l_CurrentGain );
			l_Animator.SetValues( l_CurrentGain, 0.f, _TotalTime, _Type );
			m_SourcesFadeInOutMap.insert( std::pair<uint32,CLerpAnimator1D> ( _Source, l_Animator ) );
			l_IsOK = true;
		}
		else
		{
			std::string l_Description = "Ya se esta realizando un FadeIn/Out con el source" + _Source;
			LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::FadeOutSource --> %s", l_Description.c_str());
			l_IsOK =  false;
		}
	}
	else
	{
		std::string l_Description = "Can't do FadeIn with source. Invalid source " + _Source;
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::FadeOutSource --> %s", l_Description.c_str());
		l_IsOK =  false;
	}
	return l_IsOK;
}


// -----------------------------------------
//			   MÈTODES LISTENER
// -----------------------------------------

//--------------------------------------------------------------------------------------
// SetListenerPosition : Coloca la posición del que escucha
//--------------------------------------------------------------------------------------
void CSoundManager::SetListenerPosition ( const Vect3f& _InPosition )
{
	ALfloat l_ListenerPos[] = { _InPosition.x, _InPosition.y, _InPosition.z } ;
	
	alListenerfv ( AL_POSITION, l_ListenerPos );
	
	// Check for an error.
    ALenum l_Error;
	if ( ( l_Error = alGetError()) != AL_NO_ERROR )
	{
		std::string l_Description = "Error setting listener position (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::SetListenerPosition --> %s", l_Description.c_str());
		return;
	}
}						 
				
//--------------------------------------------------------------------------------------
// GetListenerPosition : Obtiene la posición del que escucha
//--------------------------------------------------------------------------------------
void CSoundManager::GetListenerPosition ( Vect3f& _OutPosition )
{
	ALfloat l_ListenerPos[3];

	alGetListenerfv( AL_POSITION, l_ListenerPos );

	// Check for an error.
    ALenum l_Error;
	if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
	{
		_OutPosition.x = l_ListenerPos[0];
		_OutPosition.y = l_ListenerPos[1];
		_OutPosition.z = l_ListenerPos[2];
		return;
	}
	else
	{
		std::string l_Description = "Error getting listener velocity (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::GetListenerPosition --> %s", l_Description.c_str());
		return;
	}
}						 
	
//--------------------------------------------------------------------------------------
// SetListenerVelocity : Coloca la posición del que escucha
//--------------------------------------------------------------------------------------
void CSoundManager::SetListenerVelocity ( const Vect3f& _InVelocity )
{
	ALfloat l_ListenerVel[] = { _InVelocity.x, _InVelocity.y, _InVelocity.z };

	alListenerfv ( AL_VELOCITY, l_ListenerVel );
	
	// Check for an error.
    ALenum l_Error;
	if ( ( l_Error = alGetError()) != AL_NO_ERROR )
	{
		std::string l_Description = "Error setting listener velocity (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::SetListenerVelocity --> %s", l_Description.c_str());
		return;
	}
}						 
						 
//--------------------------------------------------------------------------------------
// GetListenerVelocity : Obtiene la velocidad del que escucha
//--------------------------------------------------------------------------------------
void CSoundManager::GetListenerVelocity ( Vect3f& _OutVelocity )
{
	ALfloat l_ListenerVel[3];

	alGetListenerfv( AL_VELOCITY, l_ListenerVel );

	// Check for an error.
    ALenum l_Error;
	if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
	{
		_OutVelocity.x = l_ListenerVel[0];
		_OutVelocity.y = l_ListenerVel[1];
		_OutVelocity.z = l_ListenerVel[2];
		return;
	}
	else
	{
		std::string l_Description = "Error getting listener velocity (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::GetListenerVelocity --> %s", l_Description.c_str());
		return;
	}
}

//--------------------------------------------------------------------------------------
// SetListenerOrientation : Coloca la orientación del que escucha
//--------------------------------------------------------------------------------------
void CSoundManager::SetListenerOrientation	( const Vect3f& _InAt, const Vect3f& _InUp )
{
	ALfloat l_ListenerOri[] = { _InAt.x, _InAt.y, _InAt.z, _InUp.x, _InUp.y, _InUp.z };

	alListenerfv ( AL_ORIENTATION, l_ListenerOri );
	
	// Check for an error.
    ALenum l_Error;
	if ( ( l_Error = alGetError()) != AL_NO_ERROR )
	{
		std::string l_Description = "Error setting listener orientation (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::SetListenerOrientation --> %s", l_Description.c_str());
		return;
	}
}

//--------------------------------------------------------------------------------------
// GetListenerOrientation : Obtiene la orientación del que escucha
//--------------------------------------------------------------------------------------
void CSoundManager::GetListenerOrientation	( Vect3f& _OutAt, Vect3f& _OutUp )
{
	ALfloat l_ListenerOri[6];

	alGetListenerfv( AL_ORIENTATION, l_ListenerOri );

	// Check for an error.
    ALenum l_Error;
	if ( ( l_Error = alGetError() ) == AL_NO_ERROR )
	{
		_OutAt.x = l_ListenerOri[0];
		_OutAt.y = l_ListenerOri[1];
		_OutAt.z = l_ListenerOri[2];
		_OutUp.x = l_ListenerOri[3];
		_OutUp.y = l_ListenerOri[4];
		_OutUp.z = l_ListenerOri[5];

		return;
	}
	else
	{
		std::string l_Description = "Error getting listener orientation (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::GetListenerOrientation --> %s", l_Description.c_str());
		return;
	}
}

// -----------------------------------------
//				MÈTODES PRIVATS
// -----------------------------------------

// ------------------------------------------------------------------------------
// _initAL : Inicializa el device i context
// ------------------------------------------------------------------------------
bool CSoundManager::_initAL ( void )
{
	ALenum error;
    ALCdevice* pDevice;
    ALCcontext* pContext;

    // Get handle to default device.
    pDevice = alcOpenDevice(NULL);

    // Get the device specifier.
    //const ALCubyte* deviceSpecifier = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);

    // Create audio context.
    pContext = alcCreateContext(pDevice, NULL);

    // Set active context.
    alcMakeContextCurrent(pContext);

    // Check for an error.
    if ( ( error = alcGetError(pDevice) ) != ALC_NO_ERROR )
    {
        std::string description = "Can't create openAL context (" + _getALErrorString(error) + ")";
        LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
        return false;
    }
    return true;
}

// ------------------------------------------------------------------------------
// _finalizeAL : Resetea i liberamos el context i device
// ------------------------------------------------------------------------------
void CSoundManager::_finalizeAL ( void )
{
	_clear();
    
    ALCcontext* pCurContext;
    ALCdevice* pCurDevice;

    // Get the current context.
    pCurContext = alcGetCurrentContext();

    // Get the device used by that context.
    pCurDevice = alcGetContextsDevice(pCurContext);

    // Reset the current context to NULL.
    alcMakeContextCurrent(NULL);

    // Release the context and the device.
    alcDestroyContext(pCurContext);
    alcCloseDevice(pCurDevice);
}

// ------------------------------------------------------------------------------
// _loadSound: Cargar un fichero wav i dando un indetificador
// ------------------------------------------------------------------------------
bool CSoundManager::_loadSound ( const std::string& _File, tIdBuffer& _Buffer )
{
    // Variables to load into.
    FILE *		l_Fd = NULL;
    ALenum		l_Format;
    ALenum		l_Error;
    ALsizei		l_Size;
    ALvoid*		l_Data;
    ALsizei		l_Freq;
    ALboolean	l_Loop;

    // Load wav data into buffers.
    alGenBuffers( 1, &_Buffer );

    if ( ( l_Error = alGetError() ) != AL_NO_ERROR)
    {    
        alDeleteBuffers(1,&_Buffer);
        std::string description = "Error: Can't create openAL Buffer (" + _getALErrorString( l_Error )  + ")";
        LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
        return false;    
    }

    // Check if the file exists
    if ( ( l_Fd = fopen ( _File.c_str(), "r" ) ) == NULL )
    {
        alDeleteBuffers(1,&_Buffer);
        std::string description = "Error: Can't open file " + _File;
        LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
        return false;
    }
    else
    {
        fclose( l_Fd );
    }
    alutLoadWAVFile((ALbyte*)_File.c_str(), &l_Format, &l_Data, &l_Size, &l_Freq, &l_Loop);
    alBufferData	( _Buffer, l_Format, l_Data, l_Size, l_Freq );
    alutUnloadWAV	( l_Format, l_Data, l_Size, l_Freq );
    if ( ( l_Error = alGetError()) != AL_NO_ERROR )
    {        
        alDeleteBuffers( 1, &_Buffer );
        std::string description = "Error: Can't load sound file " + _File + " (" + _getALErrorString( l_Error )  + ")";
        LOGGER->AddNewLog ( ELL_ERROR, "CSoundManager:: %s", description.c_str() );
        return false;
    }        
    return true;
}

// ------------------------------------------------------------------------------
// _clear: Borra los buffers i sources
// ------------------------------------------------------------------------------
void CSoundManager::_clear ( void )
{
    // Delete AL objets
    ALuint aux;    
    std::map<tAction, tIdBuffer>::iterator itBuffer;
    for( itBuffer = m_BuffersMap.begin(); itBuffer != m_BuffersMap.end(); ++itBuffer )
    {
        aux = (*itBuffer).second;
        alDeleteBuffers (1,&aux);
    }
    
    std::vector<tInfoSource>::iterator itSource;
    for( itSource = m_SourcesVector.begin(); itSource != m_SourcesVector.end(); ++itSource )
    {
        aux = (*itSource).m_uSource;
        alDeleteSources (1,&aux);
    }
    
    m_BuffersMap.clear();
    m_SourcesVector.clear();
}

// ------------------------------------------------------------------------------
// _getSource: obtiene los sources i pregunta si estàn reservados. Si alguno está 
//			libre (no pausado, no playing) lo devuelve. Si todo está ocupado
//			creamos un nuevo altavoz
// ------------------------------------------------------------------------------
int CSoundManager::_getSource ( bool _Reserved )
{
    // Find a unused source
    uint32 i = 0;
	ALint l_State;
    ALenum l_Error;
    
    for ( unsigned int i=0; i < m_SourcesVector.size(); i++)
    {
        if ( !m_SourcesVector[i].m_bReserved )
        {
            // Check if it is being used
            alGetSourcei ( m_SourcesVector[i].m_uSource, AL_SOURCE_STATE, &l_State );
            if ( l_State != AL_PLAYING && l_State != AL_PAUSED )
            {
                m_SourcesVector[i].m_bReserved = _Reserved;
                return i;
            }            
        }
    }
        
    tInfoSource l_Info;									
    alGenSources ( 1, &l_Info.m_uSource );				// Genere n sources i també pot passar un punté a un array de ALuint
    if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
    {
        std::string description = "Can't create source (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
        i=-1;
    }
    else
    {
        l_Info.m_bReserved = _Reserved;
        m_SourcesVector.push_back( l_Info );
        i = (uint32) m_SourcesVector.size() - 1;    

		CBillboard *l_Bb = new CBillboard();
		CTexture* l_Tex = new CTexture();
		if ( !l_Tex->Load ( "Data/General/Textures/Altavoz.png" ) )
			CHECKED_DELETE ( l_Tex );

		l_Bb->SetTexture ( l_Tex );
		l_Bb->SetWidth ( 1.f );
		l_Bb->SetHeight ( 1.f );
		l_Bb->SetVisible ( true );
		m_BillboardVector.push_back( l_Bb );
		l_Bb = NULL;
    }
    
    return i;
}

// ------------------------------------------------------------------------------
// _getALErrorString: Permite retornar el String de error concatenados. 
//			No estan todos los errores
// ------------------------------------------------------------------------------
std::string CSoundManager::_getALErrorString( ALenum _Err )
{
    switch ( _Err )
    {
        case AL_NO_ERROR:
            return std::string("AL_NO_ERROR - there is not currently an error");
			break;

        case AL_INVALID_NAME:
            return std::string("AL_INVALID_NAME - a bad name (ID) was passed to an OpenAL function");
            break;
        case AL_INVALID_ENUM:
            return std::string("AL_INVALID_ENUM - an invalid enum value was passed to an OpenAL function");
            break;
        case AL_INVALID_VALUE:
            return std::string("AL_INVALID_VALUE - an invalid value was passed to an OpenAL function");
            break;
        case AL_INVALID_OPERATION:
            return std::string("AL_INVALID_OPERATION - the requested operation is not valid");
            break;
        case AL_OUT_OF_MEMORY:
            return std::string("AL_OUT_OF_MEMORY - the requested operation resulted in OpenAL running out of memory");
            break;
        default:
            return std::string("JORLS!");
    };
}

// -----------------------------------------
//				PROPIEDADES
// -----------------------------------------
// --------------------------------------------------------------------------------------------
// IsSourceFinished: Devuelve si el source está inactivo o ha finalizado su reproducción 
// --------------------------------------------------------------------------------------------
bool CSoundManager::IsSourceFinished ( uint32 _Source )
{
	
	ALint l_State; 
	
	// Obtenemos el estado actual
	alGetSourcei ( m_SourcesVector[_Source].m_uSource , AL_SOURCE_STATE, &l_State);

	// Check for an error.
	ALenum l_Error;
	if ( ( l_Error = alGetError() ) != AL_NO_ERROR )
	{
		std::string l_Description = "Error getting state (" + _getALErrorString( l_Error ) + ")";
        LOGGER->AddNewLog( ELL_ERROR, "CSoundManager::IsSourceFinished --> %s", l_Description.c_str());
		return false;
	}

	return ( l_State == AL_STOPPED ); // || l_State == AL_INITIAL );
}
