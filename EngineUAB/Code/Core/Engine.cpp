#include "Engine.h"
#include "Base.h"

CEngine::CEngine()
	: m_Core(NULL)
	, m_Process(NULL)
{
}

CEngine::~CEngine()
{
	CHECKED_DELETE(m_Core);
}

void CEngine::Init(HWND hWnd)
{
	m_Core = new CCore();
	m_Core->Init(hWnd);
	m_Process->Init();
}

void CEngine::Update(float ElapsedTime)
{
	m_Core->Update(ElapsedTime);
	m_Process->Update(ElapsedTime);
}

void CEngine::Render()
{
	CRenderManager* l_RenderManager = m_Core->GetRenderManager();

	l_RenderManager->BeginRendering();
	l_RenderManager->SetupMatrices();
	{
		RenderScene(l_RenderManager);		
	}
	l_RenderManager->EndRendering();
}

void CEngine::RenderScene(CRenderManager *renderManager)
{
	m_Process->RenderScene();
}

void CEngine::SetProcess(CProcess *process)
{
	m_Process = process;
}