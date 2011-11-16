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
}

void CViewerProcess::Update(float elapsedTime)
{
	
}

void CViewerProcess::Render()
{
	uint32 dx = 10;
	dx += CORE->GetFontManager()->DrawLiteral( 10, dx, "HiWorld" );
	//dx += CORE->GetFontManager()->DrawDefaultText( 10, dx, colYELLOW, "%f", posX );
}
