#include "DebugRender.h"
#include "Fonts\FontManager.h"
#include "RenderManager.h"
#include "Utils\Timer.h"
#include "ActionToInput.h"
#include "Utils\BaseUtils.h"
#include "Cameras\Camera.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDebugRender::CDebugRender(void)
	: m_bIsVisible			(true)
	, m_SizeRectangle		( Vect2i(0,0) )
	, m_bFPSVisible			(true)
	, m_bDeltaTimeVisible	(true)
	, m_bGamePadVisible		(true)
	, m_bCameraVisible		(true)
	, m_bStateVisible		(true)
	, m_StateName			("")
	, m_EnemyStateName		("")
{
}


CDebugRender::~CDebugRender(void)
{
}

void CDebugRender::Init()
{
	AnalizeSizeInfo();
}

void CDebugRender::Render(CRenderManager *_RM, CFontManager *fm, CTimer *timer, CColor color )
{
	uint32 dy = 6;

	if( m_bIsVisible )
	{
		uint32 dx = m_SizeRectangle.x + 20;
		Vect2i screen = _RM->GetScreenSize();

		CColor backgroundColor = colGREEN;
		backgroundColor.SetAlpha(0.7f);
		CColor edgesColor = colBLACK;
		edgesColor.SetAlpha(0.7f);
		
		_RM->DrawRectangle2D(Vect2i(screen.x - dx, dy), m_SizeRectangle.x, m_SizeRectangle.y, backgroundColor, 2, 2, edgesColor);

		std::string gamepad, hideInfo;
		gamepad = CORE->GetActionToInput()->GetGamepadState() ? "true" : "false";
		CORE->GetActionToInput()->GetActionInfo("DebugInfo", hideInfo);

		if( m_bFPSVisible )
			dy += fm->DrawDefaultText(screen.x - dx, dy, color, "FPS: %f", timer->GetFPS() );

		if( m_bDeltaTimeVisible )
		{
			float l_Dt = timer->GetElapsedTime() * 1000.f;
			if( l_Dt < 16.8f )
			{
				dy += fm->DrawDefaultText(screen.x - dx, dy, color, "Delta Time (ms): %f",  l_Dt);
			}
			else
			{
				fm->DrawDefaultText(screen.x - dx, dy, color, "Delta Time (ms): ");
				dy += fm->DrawDefaultText(screen.x - dx + fm->SizeX(std::string("Delta Time (ms): ").c_str()), dy, colRED, " %f", l_Dt);
			}
		}
		
		if( m_bGamePadVisible )
		{
			dy += fm->DrawDefaultText(screen.x - dx, dy, color, "Gamepad: %s", gamepad.c_str());
		}
		
		if( m_bCameraVisible )
		{
			std::string camera = CORE->GetCamera()->GetName();
			dy += fm->DrawDefaultText(screen.x - dx, dy, color, "Camera: %s", camera.c_str());
		}

		if( m_bStateVisible )
		{
			dy += fm->DrawDefaultText(screen.x - dx, dy, color, "State: %s", m_StateName.c_str());
		}

		if( m_bStateVisible )
		{
			dy += fm->DrawDefaultText(screen.x - dx, dy, color, "Enemy State: %s", m_EnemyStateName.c_str());
		}


		//Info para ocultar la información
		dy += fm->DrawDefaultText(screen.x - dx, dy, color, "_________________________");
		dy += fm->DrawDefaultText(screen.x - dx, dy, color, "%s to hide", hideInfo.c_str());
	}
	else
	{
		Vect2i l_SizeRectangle;
		Vect2i l_Screen = _RM->GetScreenSize();
		CFontManager* fm = CORE->GetFontManager();

		CColor backgroundColor = colGREEN;
		backgroundColor.SetAlpha(0.7f);
		CColor edgesColor = colBLACK;
		edgesColor.SetAlpha(0.7f);

		std::string l_sInfo, l_Action;
		CORE->GetActionToInput()->GetActionInfo("DebugInfo", l_Action);
		baseUtils::FormatSrting (l_sInfo, "Press %s to view the Debug Info", l_Action.c_str() );
		l_SizeRectangle.x = fm->SizeX(l_sInfo.c_str());
		l_SizeRectangle.y = fm->SizeY(l_sInfo.c_str());

		uint32 dx = l_SizeRectangle.x + 20;
		_RM->DrawRectangle2D(Vect2i(l_Screen.x - dx, dy), l_SizeRectangle.x, l_SizeRectangle.y, backgroundColor, 2, 2, edgesColor);

		fm->DrawDefaultText(l_Screen.x - dx, dy, colWHITE, "Press %s to view the Debug Info", l_Action.c_str());
	}

}
void CDebugRender::AnalizeSizeInfo()
{
	std::string l_sInfo;
	CFontManager* fm = CORE->GetFontManager();

	//Calculate the numbers of lines
	uint16 l_NumOfElements = 2; //2 because icludes the line and the keys' information
	if( m_bFPSVisible ) ++l_NumOfElements;
	if( m_bDeltaTimeVisible ) ++l_NumOfElements;
	if( m_bGamePadVisible ) ++l_NumOfElements;
	if( m_bCameraVisible ) ++l_NumOfElements;
	if( m_bStateVisible ) ++l_NumOfElements;

	//Calculate the size of the area
	baseUtils::FormatSrting(l_sInfo,"_________________________");
	m_SizeRectangle.x = fm->SizeX(l_sInfo.c_str());
	m_SizeRectangle.y = fm->SizeY(l_sInfo.c_str()) * l_NumOfElements;
}