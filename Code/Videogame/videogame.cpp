#include <Windows.h>
#include "Xml/XMLTreeNode.h"

#define APPLICATION_NAME	"VIDEOGAME"

int myTest()
{
	CXMLTreeNode NewXML;

	if (NewXML.StartNewFile("test.xml"))
	{
	  // We fill the doc here
	  NewXML.StartElement("Companies");
	  NewXML.StartElement("Company");
	  NewXML.WritePszProperty("name", "UAB");
	  NewXML.WritePszProperty("location", "Marina 13");
	  NewXML.StartElement("Workers");
	  NewXML.WriteIntProperty("number", 2);
	  NewXML.StartElement("worker");
	  NewXML.WritePszProperty("name", "Gabriel");
	  NewXML.WritePszProperty("surname", "Cirera");
	  NewXML.WriteIntProperty("age", 27);
	  NewXML.WriteFloatProperty("salary", 232.4f);
	  NewXML.WriteBoolProperty("dead", true);
	  NewXML.EndElement();
	  NewXML.StartElement("worker");
	  NewXML.WritePszProperty("name", "Toni");
	  NewXML.WritePszProperty("surname", "Navarrete");
	  NewXML.WriteIntProperty("age", 46);
	  NewXML.WriteFloatProperty("salary", 122.3214f);
	  NewXML.WriteBoolProperty("dead", false);
	  NewXML.EndElement();
	  NewXML.EndElement();
	  NewXML.EndElement();
	  NewXML.EndElement();
	  NewXML.EndNewFile();

	  CXMLTreeNode newFile;
	  if (!newFile.LoadFile("test.xml"))
	  {
		printf("ERROR loading the file.");
		return 0;
	  }

	  CXMLTreeNode  m = newFile["Workers"];
	  if (m.Exists())
	  {
		int count = m.GetNumChildren();
		for (int i = 0; i < count; ++i)
		{
		  const char* name = m(i).GetPszProperty("name");
		  int iAge = m(i).GetIntProperty("age");
		  float fSalary = m(i).GetFloatProperty("salary");
		  bool bDead = m(i).GetBoolProperty("dead");

		  printf("Name: %s, Age: %d, Salary: %f, Dead: %d\n", name, iAge, fSalary, bDead);
		}
	  }
	}

	return 1;
}

void LoadEnemies()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile("data/enemies.xml"))
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
	LoadEnemies();

  // Register the window class
  WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

  RegisterClassEx( &wc );

  // Create the application's window
  HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL );


  // Añadir aquí el Init de la applicacioón

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
    }
  }
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

  return 0;
}