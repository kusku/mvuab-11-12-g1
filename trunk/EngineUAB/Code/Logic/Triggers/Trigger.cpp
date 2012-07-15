#include "Trigger.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "PhysicUserData.h"
#include "Core.h"
#include "Base.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

//----------------------------------------------
CTrigger::CTrigger()
	: m_pTriggerActor(NULL)
	, m_pTriggerObject(NULL)
	, m_bIsActive(true)
	, m_bRenderDebugTrigger(true)
	, m_Position(v3fZERO)
	, m_RenderColor(colGREEN)
{
}

//----------------------------------------------
CTrigger::~CTrigger()
{
	CleanUp();
}

//----------------------------------------------
void CTrigger::Init()
{
}

//----------------------------------------------
void CTrigger::CleanUp()
{
	CPhysicUserData *l_pUserData = m_pTriggerActor->GetUserData();
	CORE->GetPhysicsManager()->ReleasePhysicActor(m_pTriggerActor);

	CHECKED_DELETE(l_pUserData);
	CHECKED_DELETE(m_pTriggerActor);
}