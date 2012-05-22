#include <Windows.h>
#include "Engine.h"
#include "Base.h"
#include "Logger\Logger.h"
#include "GameProcess.h"
#include "GUIProcess.h"
#include "Math\Vector2.h"
#include "Exceptions\Exception.h"
#include "Core.h"
#include "TestDef.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

#define APPLICATION_NAME	"TEST_HITO_MAYO"

CEngine *g_Engine = NULL;
CGUIProcess* g_GUIProcess = NULL;
CGameProcess* g_GameProcess = NULL;

HWND g_hWnd = NULL;

//Headers
void ShowErrorMessage (const std::string& message);

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch( msg )
  {
  case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      return 0;

	  break;
    }
  case WM_KEYDOWN:
    {
      switch( wParam )
      {
      case VK_ESCAPE:
        //Cleanup();
        PostQuitMessage( 0 );
        return 0;
        break;
      }
	  break;
    }
	case WM_GAME_PROCESS:
		{
			if( g_GameProcess == NULL )
			{
				g_GameProcess = new CGameProcess(g_hWnd);
				g_GameProcess->Init();
			}

			g_Engine->SetProcess( g_GameProcess );
			CORE->SetProcess(g_GameProcess);
			CORE->SetGameMode(true);
			break;
		}
	case WM_GUI_PROCESS:
		{
			if( g_GUIProcess == NULL )
			{
				g_GUIProcess = new CGUIProcess(g_hWnd);
				g_GUIProcess->Init();
			}

			g_Engine->SetProcess( g_GUIProcess );
			CORE->SetProcess(g_GUIProcess);
			CORE->SetGameMode(false);
			break;
		}
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	// Register the window class
	WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	RegisterClassEx( &wc );
	try
	{
		// Añadir aquí el Init de la applicacioón
		g_Engine = new CEngine();

		g_Engine->LoadConfigXML("./Data/XML/engine.xml");
		Vect2i position		= g_Engine->GetPosition();
		Vect2i resolution	= g_Engine->GetResolution();

		// Create the application's window
		g_hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, position.x, position.y,
				resolution.x, resolution.y, NULL, NULL, wc.hInstance, NULL );

		g_GUIProcess = new CGUIProcess(g_hWnd);

		g_Engine->SetProcess(g_GUIProcess);
		g_Engine->Init(g_hWnd);

		CORE->SetProcess(g_GUIProcess);
		CORE->SetGameMode( false );

		ShowWindow( g_hWnd, SW_SHOWDEFAULT );
		UpdateWindow( g_hWnd );
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );

		// Añadir en el while la condición de salida del programa de la aplicación

		while( msg.message != WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				// Main loop: Añadir aquí el Update y Render de la aplicación principal
				g_Engine->Update();
				g_Engine->Render();
			}
		}
	}
	catch(CException &e)
	{
		ShowErrorMessage(e.GetDescription());
	}
	UnregisterClass( APPLICATION_NAME, wc.hInstance );

	CHECKED_DELETE(g_Engine);
	CHECKED_DELETE(g_GUIProcess);
	CHECKED_DELETE(g_GameProcess);

  return 0;
}

void ShowErrorMessage (const std::string& message)
{
	bool logSaved = false;

    logSaved = LOGGER->SaveLogsInFile();

    std::string end_message = "";
    if (logSaved)   
	{
		end_message += "Sorry, Application failed. Logs saved\n";
	}
    else
	{ 
		end_message += "Sorry, Application failed. Logs could not be saved\n";
	}
    end_message += message;
    MessageBox(0, end_message.c_str(), "FlostiProject Report", MB_OK | MB_ICONERROR);
}

