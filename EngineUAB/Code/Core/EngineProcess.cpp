#include "EngineProcess.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEngineProcess::CEngineProcess()
	: m_Camera(NULL)
{
}

CEngineProcess::~CEngineProcess()
{
}

void CEngineProcess::Init()
{
}

void CEngineProcess::Render(CRenderManager &RM)
{
}

void CEngineProcess::Update(float ElapsedTime)
{
}

