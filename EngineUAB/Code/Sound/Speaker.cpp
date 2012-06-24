#include "Speaker.h"
#include "Object3D.h"
#include "RenderManager.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSpeaker::CSpeaker(AkGameObjectID _ID, const std::string &_Name)
	: m_pObject3D(NULL)
	, m_ID(_ID)
{
	SetName(_Name);

	m_SoundPosition.Position.X = 0.0f;
	m_SoundPosition.Position.Y = 0.0f;
	m_SoundPosition.Position.Z = 0.0f;

	m_SoundPosition.Orientation.X = 1.0f;
	m_SoundPosition.Orientation.Y = 0.0f;
	m_SoundPosition.Orientation.Z = 0.0f;

	AK::SoundEngine::RegisterGameObj(m_ID, _Name.c_str() );
}

CSpeaker::CSpeaker(AkGameObjectID _ID, CObject3D *_Object3D, const std::string &_Name)
	: m_pObject3D(_Object3D)
	, m_ID(_ID)
{
	SetName(_Name);

	m_SoundPosition.Position.X = 0.0f;
	m_SoundPosition.Position.Y = 0.0f;
	m_SoundPosition.Position.Z = 0.0f;

	m_SoundPosition.Orientation.X = 1.0f;
	m_SoundPosition.Orientation.Y = 0.0f;
	m_SoundPosition.Orientation.Z = 0.0f;

	AK::SoundEngine::RegisterGameObj(m_ID, m_Name.c_str());
}

CSpeaker::~CSpeaker()
{
}

void CSpeaker::Init()
{
	AK::SoundEngine::RegisterGameObj( m_ID, m_Name.c_str() );
}

void CSpeaker::Update()
{
	if( m_pObject3D != NULL )
	{
		Vect3f l_Position = m_pObject3D->GetPosition();
		float l_Yaw = m_pObject3D->GetYaw();
		float l_Pitch = m_pObject3D->GetPitch();

		Vect3f l_Orientation( cos(l_Yaw) * cos(l_Pitch), sin(l_Pitch), sin(l_Yaw) * cos(l_Pitch) );

		m_SoundPosition.Position.X = l_Position.x;
		m_SoundPosition.Position.Y = l_Position.y;
		m_SoundPosition.Position.Z = l_Position.z;

		m_SoundPosition.Orientation.X = l_Orientation.x;
		m_SoundPosition.Orientation.Y = l_Orientation.y;
		m_SoundPosition.Orientation.Z = l_Orientation.z;
	}

	AK::SoundEngine::SetPosition(m_ID, m_SoundPosition);
}

void CSpeaker::Render(CRenderManager &_RM)
{
}