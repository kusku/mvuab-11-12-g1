#include "Triggers\Types\SphereTrigger.h"
#include "RenderManager.h"
#include "Triggers\Trigger.h"
#include "PhysicUserData.h"
#include "PhysicActor.h"
#include "PhysicsDefs.h"
#include "PhysicsManager.h"
#include "Core.h"
#include "Base.h"
#include "Math/Vector3.h"
#include "Math\Matrix44.h"
#include "Math\Color.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

//----------------------------------------------
CSphereTrigger::CSphereTrigger()
{
}

//----------------------------------------------
CSphereTrigger::~CSphereTrigger()
{
}

//----------------------------------------------
void CSphereTrigger::ReadData( CXMLTreeNode &_Node )
{
	m_Name			= _Node.GetPszProperty("name", "");
	m_Position		= _Node.GetVect3fProperty("position", v3fZERO);
	m_fRadius		= _Node.GetFloatProperty("radius", 0.f);
	m_RenderColor	= CColor( _Node.GetVect4fProperty("color", Vect4f(1.f, 1.f, 1.f, 1.f)) );
}

//----------------------------------------------
void CSphereTrigger::Init()
{
	CPhysicUserData * l_UserData = new CPhysicUserData ( GetName(), UD_IS_TRIGGER );
	l_UserData->SetColor ( m_RenderColor );
	l_UserData->SetPaint ( m_bRenderDebugTrigger );

	m_pTriggerActor = new CPhysicActor( l_UserData );
	m_pTriggerActor->CreateSphereTrigger( m_Position, m_fRadius, ECG_TRIGGERS );

	CORE->GetPhysicsManager()->AddPhysicActor(m_pTriggerActor);
}

//----------------------------------------------
void CSphereTrigger::Update( float _fElapsedTime )
{

}

//----------------------------------------------
void CSphereTrigger::Render( CRenderManager *_RM )
{
	Mat44f l_Translation;

	if( m_bRenderDebugTrigger )
	{
		l_Translation.SetIdentity();
		l_Translation.Translate(m_Position);
		_RM->SetTransform(l_Translation);

		_RM->DrawSphere(m_fRadius, 10, m_RenderColor);
	}
}

//----------------------------------------------
void CSphereTrigger::OnEnter()
{

}

//----------------------------------------------
void CSphereTrigger::OnStay()
{

}

//----------------------------------------------
void CSphereTrigger::OnExit()
{

}