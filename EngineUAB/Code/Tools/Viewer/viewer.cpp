#include <Windows.h>
#include "Engine.h"
#include "ViewerProcess.h"
#include "Base.h"

#define APPLICATION_NAME	"VIEWER"

CEngine *g_Engine = NULL;

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
	  case VK_F1:
		  {
			  g_Engine->Reload();
			  return 0;
			  break;
		  }
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
	
	//Crear el Engine y leer la configuración
	g_Engine = new CEngine();
	CViewerProcess *l_Viewer;
	l_Viewer = new CViewerProcess();
	g_Engine->SetProcess(l_Viewer);

	g_Engine->LoadConfigXML( "./Data/engine.xml" );
	Vect2i position = g_Engine->GetPosition();
	Vect2i resolution = g_Engine->GetResolution();

	// Create the application's window
	HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, position.x, position.y,
		resolution.x, resolution.y, NULL, NULL, wc.hInstance, NULL );

	
	// Init de la applicacioón
	g_Engine->Init( hWnd );


	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );

	// While la condición de salida del programa de la aplicación

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// Main loop: Update y Render de la aplicación principal
			g_Engine->Update();
			g_Engine->Render();
		}
	}
	UnregisterClass( APPLICATION_NAME, wc.hInstance );

	// Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
	CHECKED_DELETE(g_Engine);


	return 0;
}