#include <Windows.h>
#include "GUIProcess.h"
#include "GameProcess.h"
#include "Exceptions\Exception.h"
#include "Engine.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"
#include "Scripting\ScriptManager.h"
#include "VideogameDefs.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

#if defined (_DEBUG)
#define APPLICATION_NAME	"VIDEOGAME DEBUG"
#else
#define APPLICATION_NAME	"VIDEOGAME RELEASE"
#endif

CEngine			*g_pEngine = NULL;
CGUIProcess		*g_pGUIProcess = NULL;
CGameProcess	*g_pGameProcess = NULL;

HWND			g_hWnd = NULL;

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
			if( g_pGameProcess == NULL )
			{
				g_pGameProcess = new CGameProcess(g_hWnd);
				CORE->SetProcess(g_pGameProcess);
				g_pGameProcess->Init();
			}

			g_pEngine->SetProcess( g_pGameProcess );
			CORE->SetProcess(g_pGameProcess);
			CORE->SetGameMode(true);
			break;
		}
	case WM_GUI_PROCESS:
		{
			SCRIPT->RunCode("unload_data()");
			CHECKED_DELETE(g_pGameProcess);

			if( g_pGUIProcess == NULL )
			{				
				g_pGUIProcess = new CGUIProcess(g_hWnd);
				CORE->SetProcess(g_pGUIProcess);
				g_pGUIProcess->Init();
			}

			g_pEngine->SetProcess( g_pGUIProcess );
			CORE->SetProcess(g_pGUIProcess);
			g_pGUIProcess->SetCameraToCore();
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
		//Crea el engine
		g_pEngine = new CEngine();
		g_pEngine->LoadConfigXML("./Data/XML/engine.xml");

		Vect2i position = g_pEngine->GetPosition();
		Vect2i resolution = g_pEngine->GetResolution();

		 // Create the application's window
		g_hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 
			position.x, position.y, resolution.x, resolution.y, NULL, NULL, wc.hInstance, NULL );

		if( INIT_GUI )
		{
			g_pGUIProcess = new CGUIProcess(g_hWnd);

			g_pEngine->SetProcess(g_pGUIProcess);
			g_pEngine->Init(g_hWnd);
		
			CORE->SetGameMode(false);

			CGUIProcess::RegisterMethods();
			CGameProcess::RegisterMethods();
		}
		else
		{
			g_pGameProcess = new CGameProcess(g_hWnd);

			g_pEngine->SetProcess(g_pGameProcess);
			g_pEngine->Init(g_hWnd);

			CORE->SetGameMode(true);
		}

		ShowWindow( g_hWnd, SW_SHOWDEFAULT );
		UpdateWindow( g_hWnd );
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );

		//Bucle principal
		while( msg.message != WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				g_pEngine->Update();
				g_pEngine->Render();
			}
		}
	}
	catch(CException &e)
	{
		ShowErrorMessage(e.GetDescription());
	}

	UnregisterClass( APPLICATION_NAME, wc.hInstance );

	//Elimina los datos
	CHECKED_DELETE(g_pGUIProcess);
	CHECKED_DELETE(g_pGameProcess);
	CHECKED_DELETE(g_pEngine);

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