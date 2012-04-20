#include "SoundManager.h"
#include "Logger\Logger.h"
#include "Base.h"
#include "alc.h"
#include "alut.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSoundManager::CSoundManager()
	: m_bIsOk(false)
	, m_bSoundON(true)
	, m_bPause(false)
{
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CSoundManager::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CSoundManager::Release() 
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager::Release-> shutting down Sounds");

	//TODO

	LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager::Release-> offline (ok)");
}

//----------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------
bool CSoundManager::Init()
{
	//TODO

	return true;
}

//----------------------------------------------------------------------------
// General Methods
//----------------------------------------------------------------------------
void CSoundManager::Update(float deltaTime)
{
	//TODO: Actualitzar els LerpAnimations
}

bool CSoundManager::LoadSounds(const std::string &xmlSoundFile)
{
	//TODO: Llegir el fitxer XML
	return true;
}

void CSoundManager::Reset()
{
	Stop();
	_clear();
}

void CSoundManager::Pause()
{
	std::vector<tInfoSource>::iterator l_It = m_SourcesVector.begin();
	std::vector<tInfoSource>::iterator l_ItEnd = m_SourcesVector.end();
	for(; l_It != l_ItEnd; ++l_It)
	{
		if( m_bPause )
		{
			alSourcePlay( (*l_It).m_uSource );
			m_bPause = false;
		}
		else
		{
			alSourcePause( (*l_It).m_uSource );
			m_bPause = true;
		}
	}
}

void CSoundManager::Stop()
{
	std::vector<tInfoSource>::iterator l_It = m_SourcesVector.begin();
	std::vector<tInfoSource>::iterator l_ItEnd = m_SourcesVector.end();
	for(; l_It != l_ItEnd; ++l_It)
	{
		alSourceStop( (*l_It).m_uSource );
	}
}

void CSoundManager::SetGain(float gain)
{
	alListenerf(AL_GAIN, gain);
}

float CSoundManager::GetGain()
{
	float gain = 0.f;
	alGetListenerf(AL_GAIN, &gain);

	return gain;
}

//----------------------------------------------------------------------------
// Action Methods
//----------------------------------------------------------------------------
bool CSoundManager::PlayAction2D(const std::string &action)
{
	return true;
}

bool CSoundManager::PlayAction3D(const std::string &action, const Vect3f &position)
{
	return true;
}

//----------------------------------------------------------------------------
// Source Methods
//----------------------------------------------------------------------------
uint32 CSoundManager::CreateSource()
{
	return 0;
}

bool CSoundManager::DeleteSource(uint32 source)
{
	return true;
}

bool CSoundManager::PlaySource2D(uint32 source, const std::string &action, bool loop)
{
	return true;
}

bool CSoundManager::PlaySource3D(uint32 source, const std::string &action, bool loop)
{
	return true;
}

bool CSoundManager::PauseSource(uint32 source)
{
	return true;
}

bool CSoundManager::StopSource(uint32 source)
{
	return true;
}

bool CSoundManager::SetSourceGain(uint32 source, float inGain)
{
	return true;
}

bool CSoundManager::GetSourceGain(uint32 source, float& outGain)
{
	return true;
}

bool CSoundManager::SetSourcePosition(uint32 source, const Vect3f& inPosition)
{
	return true;
}

bool CSoundManager::GetSourcePosition(uint32 source, Vect3f& outPosition)
{
	return true;
}

bool CSoundManager::SetSourceVelocity(uint32 source, const Vect3f& inVelocity)
{
	return true;
}

bool CSoundManager::GetSourceVelocity(uint32 source, Vect3f& outVelocity)
{
	return true;
}

bool CSoundManager::FadeInSource(uint32 source, float totalTime, float finalGain, ETypeFunction type)
{
	return true;
}

bool CSoundManager::FadeOutSource(uint32 source, float totalTime, ETypeFunction type)
{
	return true;
}