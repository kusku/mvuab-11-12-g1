#include "WwiseSoundManager.h"
#include "Cameras\Camera.h"
#include "Logger\Logger.h"
#include "Base.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/IBytes.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AkFilePackageLowLevelIOBlocking.h>					// Sample low-level I/O implementation
#include <AK/MusicEngine/Common/AkMusicEngine.h>

#include <AkSoundEngineDLL.h>

const AkGameObjectID GAME_OBJECT_ID_TEST = 100;

CWwiseSoundManager::CWwiseSoundManager()
{
}

CWwiseSoundManager::~CWwiseSoundManager()
{
}

bool CWwiseSoundManager::Init()
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
		LOGGER->AddNewLog(ELL_ERROR, "CWwiseSoundManager::Init->No se ha podido inicializar el motor de audio.");
		return false;
	}

	// load initialization and main soundbanks

	AK::SOUNDENGINE_DLL::SetBasePath( L"data/soundbanks/" );
	AK::StreamMgr::SetCurrentLanguage( L"English(US)" );

	AkBankID bankID;
	AKRESULT retValue;
	retValue = AK::SoundEngine::LoadBank( "Init.bnk", AK_DEFAULT_POOL_ID, bankID );
	if( retValue != AK_Success )
	{
		LOGGER->AddNewLog(ELL_ERROR, "CWwiseSoundManager::Init->No se ha podido cargar Init.bnk");
		return false;
	}

	retValue = AK::SoundEngine::LoadBank( "Test_SoundBank.bnk", AK_DEFAULT_POOL_ID, bankID );
	if( retValue != AK_Success )
	{
		LOGGER->AddNewLog(ELL_ERROR, "CWwiseSoundManager::Init->No se ha podido cargar Test_SoundBank.bnk");
		return false;
	}

	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_TEST, "TestGameObject");

	return true;
}

void CWwiseSoundManager::Update(CCamera &camera)
{
	if( !AK::SoundEngine::IsInitialized() )
	{
		return;
	}

	//Establecer el micrófono
	AkListenerPosition listener;

	listener.OrientationFront.X = camera.GetDirection().x;
	listener.OrientationFront.Y = camera.GetDirection().y;
	listener.OrientationFront.Z = camera.GetDirection().z;
	listener.OrientationTop.X = 0;
	listener.OrientationTop.Y = 0;
	listener.OrientationTop.Z = 1.0f;
	listener.Position.X = camera.GetPosition().x;
	listener.Position.Y = camera.GetPosition().y;
	listener.Position.Z = camera.GetPosition().z;

	AK::SoundEngine::SetListenerPosition( listener );

	//Establecer la posición de los altavoces
	AkSoundPosition snd;

	snd.Orientation.X = camera.GetDirection().x;
	snd.Orientation.Y = camera.GetDirection().y;
	snd.Orientation.Z = camera.GetDirection().z;
	snd.Position.X = camera.GetPosition().x;
	snd.Position.Y = camera.GetPosition().y;
	snd.Position.Z = camera.GetPosition().z;

	AK::SoundEngine::SetPosition(GAME_OBJECT_ID_TEST , snd );

	AK::SOUNDENGINE_DLL::Tick();
}

void CWwiseSoundManager::Terminate()
{
	AK::SoundEngine::UnregisterAllGameObj();
	AK::SOUNDENGINE_DLL::Term();
}

void CWwiseSoundManager::PlayEvent(const std::string &event_name)
{
	if( !AK::SoundEngine::IsInitialized() )
	{
		return;
	}

	AK::SoundEngine::PostEvent( event_name.c_str(), GAME_OBJECT_ID_TEST);
}

void CWwiseSoundManager::SetSwitch(const std::string &switch_name, const std::string &container_name)
{
	AKRESULT res = AK::SoundEngine::SetSwitch( switch_name.c_str(), container_name.c_str(), GAME_OBJECT_ID_TEST);
}