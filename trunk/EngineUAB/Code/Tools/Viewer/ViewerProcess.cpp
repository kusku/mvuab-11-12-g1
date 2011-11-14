#include "ViewerProcess.h"
#include "Base.h"
#include "Core.h"
#include "Math\Color.h"


CViewerProcess::CViewerProcess()
{
}


CViewerProcess::~CViewerProcess()
{
}

void CViewerProcess::Init()
{
	return;
}

void CViewerProcess::Update(float ElapsedTime)
{
	return;
}

void CViewerProcess::Render()
{
	uint32 dx = 0;
	dx += CORE->GetFontManager()->DrawLiteral(10, dx, "HiWorld" );
	dx += CORE->GetFontManager()->DrawTextA(10, dx, colMAGENTA, 0, "Adéu");
}
