#include "TestProcess.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"
#include "Console\Console.h"
#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "DebugOptions\DebugOptions.h"
#include "Commands\SceneRendererCommandManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "ActionToInput.h"
#include "ScriptManager.h"
#include "Modifiers\ModifierManager.h"
#include "DebugGUIManager.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "Trigger.h"
#include "PhysicTriggerReport.h"
#include "PhysicSphericalJoint.h"
#include "PhysicRevoluteJoint.h"
#include "PhysicController.h"

#undef min
#undef max
#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CTestProcess::CTestProcess(void)
	: m_pThPSCamera(NULL)
	, pos(0,0)
	, screen(800,600)
	, m_pTrigger(NULL)
{
}

CTestProcess::~CTestProcess(void)
{
	CHECKED_DELETE(m_pTrigger);
	CHECKED_DELETE( m_pThPSCamera );
	m_Camera = NULL;
}

void CTestProcess::Init()
{
	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	m_Player.SetPosition(Vect3f(0.f,1.f,0.f));
	m_Player.SetPitch(0.0f);
	m_Player.SetYaw(0.0f);
	m_Player.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player, 10.0f);
	m_Camera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_Camera);

	//-----PHYSX----------
	NxScene *l_Scene = CORE->GetPhysicsManager()->GetScene();

	//Plane
	CPhysicUserData *l_DataPlane = new CPhysicUserData("name");
	l_DataPlane->SetPaint(true);
	l_DataPlane->SetColor(colWHITE);

	m_pPlane = new CPhysicActor(l_DataPlane);
	m_pPlane->AddPlaneShape(Vect3f(0.0f,1.0f,0.0f), 1.f, 1<<0);
	CORE->GetPhysicsManager()->AddPhysicActor(m_pPlane);

	//Sphere
	CPhysicUserData *l_DataSphere = new CPhysicUserData("sphere");
	l_DataSphere->SetPaint(true);
	l_DataSphere->SetColor(colMAGENTA);

	m_pSphere = new CPhysicActor(l_DataSphere);
	m_pSphere->AddBoxSphape(1.f, Vect3f(0.0f,10.0f,0.0f));
	m_pSphere->CreateBody(10.f);
	m_pSphere->CreateBoxTrigger(Vect3f(0.0f, 10.0f, 0.0f), Vect3f(1.0f, 1.0f, 1.0f));

	CORE->GetPhysicsManager()->AddPhysicActor(m_pSphere);

	CPhysicRevoluteJoint *l_pJoint = new CPhysicRevoluteJoint;
	l_pJoint->SetInfo(Vect3f(1.f, 0.f, 0.f),Vect3f(0.0f,20.f, 0.0f), m_pSphere);
	l_pJoint->SetMotor(500.f, 10.f);

	CORE->GetPhysicsManager()->AddPhysicRevoluteJoint(l_pJoint);
	
	l_pJoint->ActiveMotor(1000.f);

	NxVec3 l_Pos = l_pJoint->GetPhXJoint()->getGlobalAnchor();
	m_PosAnchor = Vect3f(l_Pos.x, l_Pos.y, l_Pos.z);

	m_pTrigger = new CTrigger;
	CORE->GetPhysicsManager()->SetTriggerReport(m_pTrigger);

	//Controller
	CPhysicUserData *l_Data = new CPhysicUserData("controller");
	l_Data->SetPaint(true);
	l_Data->SetColor(colBLACK);

	m_Controller = new CPhysicController(0.5f, 2.f, 45.f, 0.5f, 1.0f, 1<<0, l_Data);
	m_Controller->SetPosition(Vect3f(0.f, 3.f, 0.f) );
	m_Controller->SetVisible(true);

	CORE->GetPhysicsManager()->AddPhysicController(m_Controller);
}

void CTestProcess::CreateSphereActor()
{
	//Sphere
	CPhysicUserData *l_DataSphere = new CPhysicUserData("sphere");
	l_DataSphere->SetPaint(true);
	l_DataSphere->SetColor(colMAGENTA);

	CPhysicActor *l_Actor = new CPhysicActor(l_DataSphere);
	l_Actor->AddSphereShape(1.f, m_pThPSCamera->GetPosition(),v3fZERO, 0, 1);
	l_Actor->CreateBody(5.f);
	
	CORE->GetPhysicsManager()->AddPhysicActor(l_Actor);
	l_Actor->SetLinearVelocity(m_pThPSCamera->GetDirection()*10.f);

	m_Actors.push_back(l_Actor);
}


void CTestProcess::Update(float elapsedTime)
{
	CORE->SetCamera(m_Camera);
	m_Player.Update(elapsedTime, m_Camera);
	UpdateInputs(elapsedTime);

	SCollisionInfo info;
	int kk = 0;
	kk |= 1 << 0;
	Vect3f direction =  m_pThPSCamera->GetDirection();
	direction.Normalize();
	CPhysicUserData *data = CORE->GetPhysicsManager()->RaycastClosestActor(m_pThPSCamera->GetPosition(), direction, kk, info);
	if( data != NULL)
		data->SetColor(colCYAN);

	NxVec3 l_Pos = m_pSphere->GetPhXActor()->getGlobalPosition();
	m_PosActor = Vect3f(l_Pos.x, l_Pos.y, l_Pos.z);

	m_Controller->Move(Vect3f(0.f, 0.f, 0.01f), elapsedTime);

	CORE->GetRenderableObjectsLayersManager()->Update(elapsedTime);
}

void CTestProcess::Render(CRenderManager &RM)
{
	RM.DrawLine( m_PosAnchor, m_PosActor );
}

void CTestProcess::UpdateInputs(float elapsedTime)
{
	CActionToInput *action2Input = CORE->GetActionToInput();
	CScriptManager *SCRIPT = CORE->GetScriptManager();

	if( action2Input->DoAction("Sphere") )
	{
		CreateSphereActor();
	}

	if( action2Input->DoAction("Console") )
	{
		SCRIPT->RunCode("toggle_console()");
	}

	if( action2Input->DoAction("ReloadTTFs") )
	{
		SCRIPT->RunCode("reload_fonts()");
	}

	if( action2Input->DoAction("ReloadLanguageXMLs") )
	{
		SCRIPT->RunCode("reload_languages()");
	}

	if( action2Input->DoAction("ReloadActions") )
	{
		SCRIPT->RunCode("reload_inputs()");
	}

	if( action2Input->DoAction("ReloadRenderableObjects") )
	{
		SCRIPT->RunCode("reload_renderable_objects_layers()");
	}
	
	if( action2Input->DoAction("ReloadMeshes") )
	{
		SCRIPT->RunCode("reload_meshes()");
	}

	if( action2Input->DoAction("ReloadShaders") )
	{
		SCRIPT->RunCode("reload_effects()");
	}

	if( action2Input->DoAction("ReloadPools") )
	{
		SCRIPT->RunCode("reload_pools()");
	}

	if( action2Input->DoAction("ReloadScripts") )
	{
		SCRIPT->RunCode("reload_scripts()");
	}

	if( action2Input->DoAction("ReloadCommands") )
	{
		SCRIPT->RunCode("reload_render_commands()");
	}

	UpdateDebugInputs(elapsedTime, *action2Input);
}

void CTestProcess::UpdateDebugInputs(float elapsedTime, CActionToInput &action2Input)
{
	//Show & Unshow de debuggers
	if( action2Input.DoAction("DebugOptions") )
	{
		CORE->GetDebugOptions()->SetActive( !CORE->GetDebugOptions()->GetActive() );
	}

	if( action2Input.DoAction("ModifiersShow") )
	{
		bool visible = CORE->GetModifierManager()->GetVisible();
		CORE->GetModifierManager()->SetVisible( !visible );
	}

	//Modifiers actions
	if( !CORE->GetDebugGUIManager()->GetConsole()->IsActive() && !CORE->GetDebugOptions()->GetActive() )
	{
		if( action2Input.DoAction("Modifier_Previous") && !action2Input.DoAction("LogRender_PrevLine") )
		{
			CORE->GetModifierManager()->MoveToPreviousModifier();
		}

		if( action2Input.DoAction("Modifier_Next") && !action2Input.DoAction("LogRender_NextLine") )
		{
			CORE->GetModifierManager()->MoveToNextModifier();
		}

		if( action2Input.DoAction("GoToModifier") )
		{
			CORE->GetModifierManager()->GoToModifier();
		}

		if( action2Input.DoAction("GoToRootModifier") )
		{
			CORE->GetModifierManager()->GoToRoot();
		}

		if( action2Input.DoAction("AddValueToModifierByPass") || action2Input.DoAction("AddValueToModifier") )
		{
			CORE->GetModifierManager()->AddValueToModifier();
		}

		if( action2Input.DoAction("SubsValueToModifierByPass") || action2Input.DoAction("SubsValueToModifier") )
		{
			CORE->GetModifierManager()->SubsValueToModifier();
		}
	}

	//Debug Options actions
	if( CORE->GetDebugOptions()->GetActive() )
	{
		CDebugOptions *l_DebugOptions = CORE->GetDebugOptions();
		if( action2Input.DoAction("NextPage") )
		{
			l_DebugOptions->MoveToNextPage();
		}

		if( action2Input.DoAction("PrevPage") )
		{
			l_DebugOptions->MoveToPrevPage();
		}

		if( action2Input.DoAction("NextLine") )
		{
			l_DebugOptions->MoveToNextLine();
		}

		if( action2Input.DoAction("PrevLine") )
		{
			l_DebugOptions->MoveToPrevLine();
		}

		if( action2Input.DoAction("DoAction") )
		{
			l_DebugOptions->DoAction();
		}
	}
}