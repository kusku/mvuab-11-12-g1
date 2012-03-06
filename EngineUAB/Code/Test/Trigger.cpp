#include "Trigger.h"
#include "PhysicsManager.h"

#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"

CTrigger::CTrigger()
{
}

CTrigger::~CTrigger()
{
}

void CTrigger::OnEnter(CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape)
{
 	int a=1;
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnEnter->Element 1 es : %s" , _Entity_Trigger1->GetName ().c_str () );
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnEnter->Element 2 es : %s" ,_Other_Shape->GetName ().c_str () );
}
void CTrigger::OnLeave(CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape)
{
	int a=1;
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnLeave->Element 1 es : %s" , _Entity_Trigger1->GetName ().c_str () );
	LOGGER->AddNewLog ( ELL_INFORMATION, "CTriggerTest::OnLeave->Element 2 es : %s" ,_Other_Shape->GetName ().c_str () );
}
