#include "Engine.h"
#include "Base.h"

CEngine::CEngine()
	: m_pCore(NULL)
	, m_pProcess(NULL)
{
}

CEngine::~CEngine()
{
	CHECKED_DELETE(m_pCore);
}

void CEngine::Init(HWND hWnd)
{
	m_pCore = new CCore();
	m_pCore->Init(hWnd);
	m_pProcess->Init();
}

void CEngine::Update(float ElapsedTime)
{
	m_pCore->Update(ElapsedTime);
	m_pProcess->Update(ElapsedTime);
}

void CEngine::Render()
{
	CRenderManager* l_RenderManager = m_pCore->GetRenderManager();

	l_RenderManager->BeginRendering();
	l_RenderManager->SetupMatrices();
	{
		RenderScene(l_RenderManager);		
	}
	l_RenderManager->EndRendering();
}

void CEngine::RenderScene(CRenderManager *renderManager)
{
	m_pProcess->Render();
}

void CEngine::SetProcess(CProcess *process)
{
	m_pProcess = process;
}