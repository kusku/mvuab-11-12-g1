#include "Listener.h"
#include "Cameras\Camera.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CListener::CListener()
	: m_pCamera(NULL)
{
}

CListener::CListener(CCamera*camera)
	: m_pCamera(camera)
{
}

CListener::~CListener()
{
	m_pCamera = NULL;
}

void CListener::Update()
{
	assert( m_pCamera );

	Vect3f l_Position = m_pCamera->GetEye();
	Vect3f l_Orientation = m_pCamera->GetDirection().Normalize();
	Vect3f l_VectorUp = m_pCamera->GetVecUp();

	m_ListenerPosition.Position.X = l_Position.x;
	m_ListenerPosition.Position.Y = l_Position.y;
	m_ListenerPosition.Position.Z = l_Position.z;

	m_ListenerPosition.OrientationFront.X = l_Orientation.x;
	m_ListenerPosition.OrientationFront.Y = l_Orientation.y;
	m_ListenerPosition.OrientationFront.Z = l_Orientation.z;

	m_ListenerPosition.OrientationTop.X = l_VectorUp.x;
	m_ListenerPosition.OrientationTop.Y = l_VectorUp.y;
	m_ListenerPosition.OrientationTop.Z = l_VectorUp.z;

	AK::SoundEngine::SetListenerPosition( m_ListenerPosition );
}