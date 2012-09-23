#include <Windows.h>
#include "Engine.h"
#include "Base.h"
#include "Logger\Logger.h"
#include "TestAudioProcess.h"
#include "Math\Vector2.h"
#include "Exceptions\Exception.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

#define APPLICATION_NAME	"TEST_AUDIO"

CEngine *g_Engine = NULL;

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
    }
    break;
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
    }
    break;
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	CTestAudioProcess* l_TestAudioProcess = NULL;

	// Register the window class
	WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	RegisterClassEx( &wc );
	try
	{
		// Añadir aquí el Init de la applicacioón
		g_Engine = new CEngine();
		g_Engine->LoadConfigXML("./Data/XML/engine.xml");

		Vect2i position = g_Engine->GetPosition();
		Vect2i resolution = g_Engine->GetResolution();

		// Create the application's window
		HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, position.x, position.y,
				resolution.x, resolution.y, NULL, NULL, wc.hInstance, NULL );

		l_TestAudioProcess = new CTestAudioProcess();

		g_Engine->SetProcess(l_TestAudioProcess);
		g_Engine->Init(hWnd);
		
		CORE->SetProcess(l_TestAudioProcess);

		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );
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

	// Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
	CHECKED_DELETE(l_TestAudioProcess);
	CHECKED_DELETE(g_Engine);

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