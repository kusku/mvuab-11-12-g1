#include "SoundManager.h"
#include "Logger\Logger.h"
#include "Speaker.h"
#include "Listener.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"
#include "Base.h"
#include "Core.h"
#include "Exceptions\Exception.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/IBytes.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AkFilePackageLowLevelIOBlocking.h>					// Sample low-level I/O implementation
#include <AK/MusicEngine/Common/AkMusicEngine.h>

#include <AkSoundEngineDLL.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

const AkGameObjectID GAME_OBJECT_ID_TEST = 100;


// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
#ifdef WIN32
    void * AllocHook( size_t in_size )
    {
        return malloc( in_size );
    }
    void FreeHook( void * in_ptr )
    {
        free( in_ptr );
    }
    // Note: VirtualAllocHook() may be used by I/O pools of the default implementation
    // of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
    // - refer to the Windows SDK documentation for more details). This is NOT mandatory;
    // you may implement it with a simple malloc().
    void * VirtualAllocHook(
        void * in_pMemAddress,
        size_t in_size,
        DWORD in_dwAllocationType,
        DWORD in_dwProtect
        )
    {
        return VirtualAlloc( in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect );
    }
    void VirtualFreeHook( 
        void * in_pMemAddress,
        size_t in_size,
        DWORD in_dwFreeType
        )
    {
        VirtualFree( in_pMemAddress, in_size, in_dwFreeType );
    }
#endif
}


CSoundManager::CSoundManager()
	: m_pListener(NULL)
	, m_SoundBanksFilename("")
	, m_SpeakersFilename("")
	, m_Path("data/soundbanks/")
{
}

CSoundManager::~CSoundManager()
{
	Terminate();
}

bool CSoundManager::Init()
{
	// Initialize audio engine
	// Memory.
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	// Streaming.
    AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings( stmSettings );
	
    AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );

	AkInitSettings l_InitSettings;
	AkPlatformInitSettings l_platInitSetings;
	AK::SoundEngine::GetDefaultInitSettings( l_InitSettings );
	AK::SoundEngine::GetDefaultPlatformInitSettings( l_platInitSetings );

	// Setting pool sizes for this game.
	// These sizes are tuned for this game, every game should determine its own optimal values.
	l_InitSettings.uDefaultPoolSize				=  256*1024;
																					 
	l_platInitSetings.uLEngineDefaultPoolSize	=  2*1024*1024;

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings( musicInit );

	AKRESULT eResult = AK::SOUNDENGINE_DLL::Init( &memSettings,
						   &stmSettings,
                           &deviceSettings,
						   &l_InitSettings,
                           &l_platInitSetings,
						   &musicInit );

	if( eResult != AK_Success )
	{
		// Then, we will run the game without sound
		AK::SOUNDENGINE_DLL::Term();
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::Init->No se ha podido inicializar el motor de audio.");
		throw CException(__FILE__, __LINE__, "CSoundManager::Init->No se ha podido inicializar el motor de audio.");
		return false;
	}

	m_pListener = new CListener(CORE->GetCamera());

	/*AddResource("test", new CSpeaker(100, "test") );
	AK::SoundEngine::RegisterGameObj(GetResource("test")->GetID(), "TestGameObject");*/

	return true;
}

bool CSoundManager::InitBanks()
{
	// load initialization and main soundbanks
	AkOSChar *path;
	CONVERT_CHAR_TO_OSCHAR(m_Path.c_str(), path);
	AK::SOUNDENGINE_DLL::SetBasePath( path );
	AK::StreamMgr::SetCurrentLanguage( L"English(US)" );

	AkBankID bankID;
	AKRESULT retValue;
	retValue = AK::SoundEngine::LoadBank( "Init.bnk", AK_DEFAULT_POOL_ID, bankID );
	if( retValue != AK_Success )
	{
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::Init->No se ha podido cargar Init.bnk");
		return false;
	}

	return true;
}

bool CSoundManager::Load(const std::string &soundbanks_filename, const std::string &speakers_filename)
{
	m_SoundBanksFilename = soundbanks_filename;
	m_SpeakersFilename = speakers_filename;

	bool l_IsOk = true;
	l_IsOk = LoadSoundBanksXML();
	l_IsOk &= LoadSpeakersXML();

	return l_IsOk;
}

bool CSoundManager::Reload()
{
	Terminate();
	bool l_IsOk = true;
	l_IsOk = LoadSoundBanksXML();
	l_IsOk &= LoadSpeakersXML();

	return l_IsOk;
}

bool CSoundManager::LoadSoundBanksXML()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_SoundBanksFilename.c_str()))
	{
		std::string msg_error = "CSoundManager::LoadSoundBanksXML->Error al intentar leer el archivo de sonidos: " + m_SoundBanksFilename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_Banks = newFile["SoundBanks"];
	if( l_Banks.Exists() )
	{
		m_Path = l_Banks.GetPszProperty("path", "");
		InitBanks(); //Carga el banco de sonidos Init.bnk

		//Lee los bancos de sonido y los carga
		uint16 l_Count = l_Banks.GetNumChildren();
		for( uint16 i = 0; i < l_Count; ++i )
		{
			std::string l_Type = l_Banks(i).GetName();
			if( l_Type == "Bank" )
			{
				std::string l_BankName = l_Banks(i).GetPszProperty("name", "");
				LoadSoundBank(l_BankName);
			}
		}
	}
	else
	{
		std::string msg_error = "CSoundManager::LoadSoundBanksXML->Error al intentar leer el archivo de sonidos: " + m_SoundBanksFilename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	return true;
}

bool CSoundManager::LoadSpeakersXML()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_SpeakersFilename.c_str()))
	{
		std::string msg_error = "CSoundManager::LoadSpeakersXML->Error al intentar leer el archivo de speakers: " + m_SpeakersFilename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_Speakers = newFile["Speakers"];
	if( l_Speakers.Exists() )
	{
		uint16 l_Count = l_Speakers.GetNumChildren();
		for( uint16 i = 0; i < l_Count; ++i )
		{
			std::string l_Type = l_Speakers(i).GetName();
			if( l_Type == "Speaker" )
			{
				std::string l_Name = l_Speakers(i).GetPszProperty("name", "");
				Vect3f l_Position = l_Speakers(i).GetVect3fProperty("position", Vect3f(0.f, 0.f, 0.f));
				Vect3f l_Orientation = l_Speakers(i).GetVect3fProperty("orientation", Vect3f(0.f, 0.f, 0.f));

				CSpeaker *l_pSpeaker = new CSpeaker( GetResourcesVector().size(), l_Name );
				l_pSpeaker->SetPosition( l_Position );
				l_pSpeaker->SetOrientation( l_Orientation );
				l_pSpeaker->Init();
				AddResource( l_Name, l_pSpeaker );
			}
		}
	}
	else
	{
		std::string msg_error = "CSoundManager::LoadSpeakersXML->Error al intentar leer el archivo de speakers: " + m_SpeakersFilename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	return true;
}

bool CSoundManager::LoadSoundBank(const std::string &bank)
{
	AkBankID bankID;
	AKRESULT retValue;

	retValue = AK::SoundEngine::LoadBank( bank.c_str(), AK_DEFAULT_POOL_ID, bankID );
	if( retValue != AK_Success )
	{
		LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::Init->No se ha podido cargar el soundbank: %s", bank.c_str());
		return false;
	}

	return true;
}

void CSoundManager::Update()
{
	if( !AK::SoundEngine::IsInitialized() )
	{
		return;
	}

	//Actualiza el micrófono
	m_pListener->Update();

	//Actualiza los speakers
	TVectorResources l_Speakers = GetResourcesVector();
	TVectorResources::iterator l_It = l_Speakers.begin();
	TVectorResources::iterator l_ItEnd = l_Speakers.end();
	for(; l_It != l_ItEnd; ++l_It)
	{
		(*l_It)->Update();
	}

	//Actualiza Wwise
	AK::SOUNDENGINE_DLL::Tick();
}

void CSoundManager::Render(CRenderManager *_RM)
{
}

void CSoundManager::Terminate()
{
	CHECKED_DELETE(m_pListener);
	Destroy();

	AK::SoundEngine::UnregisterAllGameObj();
	AK::SOUNDENGINE_DLL::Term();
}

void CSoundManager::PlayEvent(const std::string &event_name)
{
	if( !AK::SoundEngine::IsInitialized() )
	{
		return;
	}

	AK::SoundEngine::PostEvent( event_name.c_str(), GetResource("Test")->GetID());
}

void CSoundManager::SetSwitch(const std::string &switch_name, const std::string &container_name)
{
	AKRESULT res = AK::SoundEngine::SetSwitch( switch_name.c_str(), container_name.c_str(), GetResource("Test")->GetID());
}

void CSoundManager::RegisterMethods()
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		class_<CSoundManager>("CSoundManager")
			.def("play_event", &CSoundManager::PlayEvent)
			.def("set_switch", &CSoundManager::SetSwitch)
	];
}