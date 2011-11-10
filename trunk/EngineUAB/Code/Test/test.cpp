#include <Windows.h>
#include "Xml/XMLTreeNode.h"
#include "Enemies\EnemyManager.h"

#define APPLICATION_NAME	"TEST"

void LoadEnemies(const std::string &filename)
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(filename.c_str()))
	{
		return;
	}

	CXMLTreeNode l_EnemiesNode = newFile["enemies"];
	if (l_EnemiesNode.Exists())
	{
		int count = l_EnemiesNode.GetNumChildren();
		for(int i=0; i < count; ++i)
		{
			const char *l_Name = l_EnemiesNode(i).GetName();
			const char *l_Type = l_EnemiesNode(i).GetPszProperty("type","");
			const char *l_Pos = l_EnemiesNode(i).GetPszProperty("pos","");
			float l_Yaw = l_EnemiesNode(i).GetFloatProperty("yaw",0.0f);
			float l_Pitch = l_EnemiesNode(i).GetFloatProperty("pitch",0.0f);
		}
	}

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

  // Create the application's window
  HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL );


  // Añadir aquí el Init de la applicacioón
	CEnemyManager l_EnemyManager;
	l_EnemyManager.LoadXML(std::string("data/enemies.xml"));

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
		l_EnemyManager.Update(0.0f);
		l_EnemyManager.Render();
    }
  }
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

  return 0;
}