#include "Core.h"
#include "Base.h"

CCore::CCore()
	: m_RenderManager(NULL)
{
}

CCore::~CCore()
{
	CHECKED_DELETE(m_RenderManager);
}

void CCore::Init(HWND hWnd)
{
	m_RenderManager = new CRenderManager();
	m_RenderManager->Init(hWnd);
}

void CCore::Update(float ElapsedTime)
{
	int a=1;
}

void CCore::Render()
{
	int a=1;
}

CRenderManager* CCore::GetRenderManager() const
{
	return m_RenderManager;
}