#include "Process.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CProcess::CProcess()
	: m_Camera(NULL)
{
}

CProcess::~CProcess()
{
	CHECKED_DELETE( m_Camera );
}

void CProcess::Init()
{
}

void CProcess::Render(CRenderManager *RM)
{
}

void CProcess::Update(float ElapsedTime)
{
}

