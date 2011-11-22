#include "ViewerProcess.h"
#include "FontManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Periphericals\Mouse.h"
#include "Base.h"
#include "Core.h"
#include "Math\Color.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "Math\Matrix44.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CViewerProcess::CViewerProcess()
	: pos(0,0)
	, screen(800,600)
{
}


CViewerProcess::~CViewerProcess()
{
}

void CViewerProcess::Init()
{
	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;
}

void CViewerProcess::Update(float elapsedTime)
{
}

void CViewerProcess::Render()
{
	Mat44f mat;
	mat.SetIdentity();
	CORE->GetRenderManager()->SetTransform(mat);

	CORE->GetRenderManager()->DrawAxis(2.0f);
	//CORE->GetRenderManager()->DrawSphere(2, 50);
	//CORE->GetRenderManager()->DrawGrid(3,3,4);

	mat.SetIdentity();
	mat.Translate(Vect3f(0.f, 0.f, 3.f));
	CORE->GetRenderManager()->SetTransform(mat);

	CORE->GetRenderManager()->DrawCube(Vect3f(2.f, 1.f, 2.f));
}
