#include "TestProcess.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"
#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "Commands\SceneRendererCommandManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "ActionToInput.h"
#include "ScriptManager.h"
#include "Modifiers\ModifierManager.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"

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
{
}

CTestProcess::~CTestProcess(void)
{
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
	NxPlaneShapeDesc planeDesc;    
	NxActorDesc actorDesc;
	planeDesc.normal = NxVec3(0.0f,1.0f,0.0f);    
	planeDesc.d = 1.0f;
    actorDesc.shapes.pushBack(&planeDesc);
	CPhysicUserData *l_DataPlane = new CPhysicUserData("name");
	l_DataPlane->SetPaint(true);
	l_DataPlane->SetColor(colWHITE);
	actorDesc.userData = l_DataPlane;
	l_Scene->createActor(actorDesc);

	//Sphere
	NxBodyDesc bodyDesc;   
	NxActorDesc actorDescSphere;    
	NxSphereShapeDesc sphereDesc;    

	sphereDesc.radius = 1.0f;
    actorDescSphere.shapes.pushBack(&sphereDesc);  
	actorDescSphere.body = &bodyDesc;   
	actorDescSphere.density = 10.0f;    
	actorDescSphere.globalPose.t = NxVec3(0.0f,10.0f,0.0f); //Position at the origin.
	CPhysicUserData *l_DataSphere = new CPhysicUserData("sphere");
	l_DataSphere->SetPaint(true);
	l_DataSphere->SetColor(colMAGENTA);
	actorDescSphere.userData = l_DataSphere;
    NxActor *actor=l_Scene->createActor(actorDescSphere);
}

void CTestProcess::Update(float elapsedTime)
{
	CORE->SetCamera(m_Camera);
	m_Player.Update(elapsedTime, m_Camera);
	UpdateInputs(elapsedTime);

	CORE->GetRenderableObjectsLayersManager()->Update(elapsedTime);
}

void CTestProcess::Render(CRenderManager *RM)
{
	CORE->GetSceneRendererCommandManager()->Execute(*RM);
}

void CTestProcess::UpdateInputs(float elapsedTime)
{
	CActionToInput *action2Input = CORE->GetActionToInput();
	CScriptManager *SCRIPT = CORE->GetScriptManager();

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

	if( action2Input->DoAction("ModifiersShow") )
	{
		bool visible = CORE->GetModifierManager()->GetVisible();
		CORE->GetModifierManager()->SetVisible( !visible );
	}

	if( action2Input->DoAction("Modifier_Previous") && !action2Input->DoAction("LogRender_PrevLine") )
	{
		CORE->GetModifierManager()->MoveToPreviousModifier();
	}

	if( action2Input->DoAction("Modifier_Next") && !action2Input->DoAction("LogRender_NextLine") )
	{
		CORE->GetModifierManager()->MoveToNextModifier();
	}

	if( action2Input->DoAction("GoToModifier") )
	{
		CORE->GetModifierManager()->GoToModifier();
	}

	if( action2Input->DoAction("GoToRootModifier") )
	{
		CORE->GetModifierManager()->GoToRoot();
	}

	if( action2Input->DoAction("AddValueToModifierByPass") || action2Input->DoAction("AddValueToModifier") )
	{
		CORE->GetModifierManager()->AddValueToModifier();
	}

	if( action2Input->DoAction("SubsValueToModifierByPass") || action2Input->DoAction("SubsValueToModifier") )
	{
		CORE->GetModifierManager()->SubsValueToModifier();
	}
}