#include "TestProcess.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"
#include "Commands\SceneRendererCommandManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CTestProcess::CTestProcess(void)
{
}

CTestProcess::~CTestProcess(void)
{
}

void CTestProcess::Init()
{
}

void CTestProcess::Update(float elapsedTime)
{
}

void CTestProcess::Render(CRenderManager *RM)
{
	CORE->GetSceneRendererCommandManager()->Execute(*RM);
}