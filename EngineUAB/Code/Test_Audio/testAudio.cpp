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

// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
#ifdef WIN32
    void * AllocHook( size_t in_size )
    {
        return malloc( in_size );
    }
    void FreeHook( void * in_ptr )
    {
        free( in_ptr );
    }
    // Note: VirtualAllocHook() may be used by I/O pools of the default implementation
    // of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
    // - refer to the Windows SDK documentation for more details). This is NOT mandatory;
    // you may implement it with a simple malloc().
    void * VirtualAllocHook(
        void * in_pMemAddress,
        size_t in_size,
        DWORD in_dwAllocationType,
        DWORD in_dwProtect
        )
    {
        return VirtualAlloc( in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect );
    }
    void VirtualFreeHook( 
        void * in_pMemAddress,
        size_t in_size,
        DWORD in_dwFreeType
        )
    {
        VirtualFree( in_pMemAddress, in_size, in_dwFreeType );
    }
#endif
}

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
	// Register the window class
	WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	RegisterClassEx( &wc );
	try
	{
		// A�adir aqu� el Init de la applicacio�n
		g_Engine = new CEngine();
		g_Engine->LoadConfigXML("./Data/XML/engine.xml");

		Vect2i position = g_Engine->GetPosition();
		Vect2i resolution = g_Engine->GetResolution();

		// Create the application's window
		HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, position.x, position.y,
				resolution.x, resolution.y, NULL, NULL, wc.hInstance, NULL );

		CTestAudioProcess* l_TestAudioProcess;
		l_TestAudioProcess = new CTestAudioProcess();

		g_Engine->SetProcess(l_TestAudioProcess);
		g_Engine->Init(hWnd);
		
		CORE->SetProcess(l_TestAudioProcess);

		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );

		// A�adir en el while la condici�n de salida del programa de la aplicaci�n

		while( msg.message != WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				// Main loop: A�adir aqu� el Update y Render de la aplicaci�n principal
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

  // A�adir una llamada a la alicaci�n para finalizar/liberar memoria de todos sus datos

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