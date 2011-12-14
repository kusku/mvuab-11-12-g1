#include "ViewerProcess.h"
#include "RenderManager.h"
#include "ActionToInput.h"
#include "Cameras\ThPSCamera.h"
#include "Base.h"
#include "Core.h"
#include "Math\Color.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "Math\Matrix44.h"
#include "Cameras\Camera.h"
#include "Vertexs\VertexType.h"
#include "Vertexs\RenderableVertexs.h"
#include "Vertexs\IndexedVertexs.h"
#include "Textures\TextureManager.h"
#include "Textures\Texture.h"
#include "RenderableObjects\StaticMeshManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderableVertexs	*g_RV = NULL;
CTexture			*g_Tex = NULL;

CViewerProcess::CViewerProcess()
	: pos(0,0)
	, screen(800,600)
	, yaw(0.0f)
	, m_pThPSCamera(NULL)	
	, m_SMManager(NULL)
{
}

CViewerProcess::~CViewerProcess()
{
	CHECKED_DELETE(g_RV);
	CHECKED_DELETE(g_Tex);
	CHECKED_DELETE( m_pThPSCamera );
	CHECKED_DELETE(m_SMManager);
	m_Camera = NULL;
}

void CViewerProcess::Init()
{
	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	m_Player.SetPosition(Vect3f(0.f,1.f,0.f));
	m_Player.SetPitch(0.0f);
	m_Player.SetYaw(0.0f);
	m_Player.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 100.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player, 10.0f);
	m_Camera = static_cast<CCamera*>(m_pThPSCamera);


	g_Tex = new CTexture();
	g_Tex->Load(".\\data\\5marc.jpg");
	CORE->GetTextureManager()->AddResource("Marc", g_Tex);
	TTEXTURE1_VERTEX l_Vertexs[4] = {{-1.f, 0.f, 0.f, 0.f, 1.f},
								{-1.f, 2.f, 0.f, 0.f, 0.f},
								{1.f, 2.f, 0.f, 1.f, 0.f},
								{1.f, 0.f, 0.f, 1.f, 1.f}};
	
	unsigned short l_Indices[6] = {0,1,2,0,2,3};

	g_RV = new CIndexedVertexs<TTEXTURE1_VERTEX>(CORE->GetRenderManager(), l_Vertexs, l_Indices, 4, 6);

	m_SMManager = new CStaticMeshManager();
	m_SMManager->Load("./Data/Meshes/static_meshes.xml");
}

void CViewerProcess::Update(float elapsedTime)
{
	m_Player.Update(elapsedTime, m_Camera);
}

void CViewerProcess::UpdateInputs(float elapsedTime)
{
	CActionToInput *action2Input = CORE->GetActionToInput();
	if( action2Input->DoAction("ReloadTTFs") )
	{
		CORE->ReloadTTFs();
	}

	if( action2Input->DoAction("ReloadLanguageXMLs") )
	{
		CORE->ReloadLanguages();
	}

	if( action2Input->DoAction("ReloadActions") )
	{
		CORE->ReloadInputs();
	}
}

void CViewerProcess::Render(CRenderManager *RM)
{
	Mat44f mat;
	mat.SetIdentity();
	RM->SetTransform(mat);
	RM->DrawAxis(1.0f);
	RM->DrawGrid(100.0f, 100.0f, 30, colBLACK);

	m_Player.Render(RM);

	//mat.SetIdentity();
	//RM->SetTransform(mat);
	//g_Tex->Activate(0);
	//g_RV->Render(RM);

	mat.SetIdentity();
	RM->SetTransform(mat);
	m_SMManager->Render(RM, "Box001");
}