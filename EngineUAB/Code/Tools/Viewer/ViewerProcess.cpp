#include "ViewerProcess.h"
#include "FontManager.h"
#include "InputManager.h"
#include "Periphericals\Mouse.h"
#include "Base.h"
#include "Core.h"
#include "Math\Color.h"

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
	Vect3i delta = CORE->GetInputManager()->GetMouseDelta();
	pos.x += delta.x;
	pos.y += delta.y;

	if(pos.x < 0)
	{
		pos.x = 0;
	}

	if(pos.x > screen.x)
	{
		pos.x = screen.x;
	}

	if(pos.y < 0)
	{
		pos.y = 0;
	}

	if(pos.y > screen.y)
	{
		pos.y = screen.y;
	}	
}

void CViewerProcess::Render()
{
	CORE->GetFontManager()->DrawDefaultText( pos.x, pos.y, colWHITE, "X" );
	//dx += CORE->GetFontManager()->DrawDefaultText( 10, dx, colYELLOW, "%f", posX );
}
