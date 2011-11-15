#include <Windows.h>
#include "Engine.h"
#include "ViewerProcess.h"

#define APPLICATION_NAME	"VIEWER"

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
	
	//Crear el Engine y leer la configuración
	CEngine l_Engine;
	CViewerProcess l_Viewer;
	SConfig l_Config;
	l_Engine.SetProcess(&l_Viewer);
	l_Engine.LoadConfigXML( "./Data/engine.xml", l_Config );


	// Create the application's window
	HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, l_Config.position.x, l_Config.position.y,
		l_Config.resolution.x, l_Config.resolution.y, NULL, NULL, wc.hInstance, NULL );


	// Init de la applicacioón
	l_Engine.Init( hWnd, l_Config );


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
			l_Engine.Update(0.0f);
			l_Engine.Render();
		}
	}
	UnregisterClass( APPLICATION_NAME, wc.hInstance );

	// Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

	return 0;
}