#include "RenderManager.h"
#include "Base.h"
#include "Math\Vector3.h"
#include <string>

CRenderManager::CRenderManager()
	: m_uWidth(0)
	, m_uHeight(0)
	, m_Size(Vect2i(800,600))
	, m_bPaintSolid(true)
	, m_bFullscreen(true)
	, m_hWnd(NULL)
	, m_pD3D(NULL)
	, m_pD3DDevice(NULL)
	, m_BackbufferColor_debug(colBLUE)
	, m_BackbufferColor_release(colBLACK)
{
}


CRenderManager::~CRenderManager()
{
}

bool CRenderManager::Init(HWND hWnd)
{
	bool m_bIsOk = false;

   // Create the D3D object.
   m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
   assert(m_pD3D);

   m_bIsOk = m_pD3D != NULL;
   if (m_bIsOk)
   {
       // Set up the structure used to create the D3DDevice
       D3DPRESENT_PARAMETERS d3dpp;
       ZeroMemory( &d3dpp, sizeof(d3dpp) );

       if(m_bFullscreen)
       {
           d3dpp.Windowed          = FALSE;
           d3dpp.BackBufferWidth   = m_Size.x;
           d3dpp.BackBufferHeight  = m_Size.y;
           d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
       }
       else
       {
           d3dpp.Windowed          = TRUE;
           d3dpp.BackBufferFormat    = D3DFMT_UNKNOWN;
       }

       d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
       d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
       d3dpp.EnableAutoDepthStencil = TRUE;
       d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
       d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
       d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

       // Create the D3DDevice
       m_bIsOk = !FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                               D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) );

	   /* if (!m_bIsOk)
       {
           m_bIsOk = !FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) );

           if (m_bIsOk)
           {
               LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: D3DCREATE_SOFTWARE_VERTEXPROCESSING");
           }
       }
       else
       {
           LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: D3DCREATE_HARDWARE_VERTEXPROCESSING");
       }
       */

	   if (m_bIsOk)
       {
           // Turn off culling, so we see the front and back of the triangle
           m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
           m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
           m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
           m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
           m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
           m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

           m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
           m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
           m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
           m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );


           m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
           m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
           m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
           m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

           // Turn off D3D lighting, since we are providing our own vertex colors
           m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

			if (m_bFullscreen)
			{
               m_uWidth    = m_Size.x;
               m_uHeight    = m_Size.y;
           }
           else
           {
               GetWindowRect(hWnd);
           }
           //LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: La resolucion de pantalla es (%dx%d)",m_uWidth,m_uHeight);
       }   
    }

   if (!m_bIsOk)
   {
       std::string msg_error = "Rendermanager::Init-> Error al inicializar Direct3D";
       //LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
       //Release();
       //throw CException(__FILE__, __LINE__, msg_error);
   }

   return m_bIsOk;
}

void CRenderManager::GetWindowRect( HWND hwnd )
{
	RECT rec_window;
	GetClientRect(    hwnd, &rec_window);
	m_uWidth = rec_window.right - rec_window.left;
	m_uHeight = rec_window.bottom - rec_window.top;
}

void CRenderManager::BeginRendering()
{
#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
	uint32 red        = (uint32) (m_BackbufferColor_debug.GetRed() * 255);
	uint32 green    = (uint32) (m_BackbufferColor_debug.GetGreen() * 255);
	uint32 blue        = (uint32) (m_BackbufferColor_debug.GetBlue()* 255);
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#else // Clear the backbuffer to a black color in a Release mode
	uint32 red        = (uint32) (m_BackbufferColor_release.GetRed() * 255);
	uint32 green    = (uint32) (m_BackbufferColor_release.GetGreen() * 255);
	uint32 blue        = (uint32) (m_BackbufferColor_release.GetBlue()* 255);
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#endif

   // Begin the scene
	HRESULT hr = m_pD3DDevice->BeginScene();
	assert( SUCCEEDED( hr ) );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

	if(m_bPaintSolid)
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}
	else
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	}
}

void CRenderManager::EndRendering()
{
	m_pD3DDevice->EndScene();

	// Present the backbuffer contents to the display
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CRenderManager::SetupMatrices(/*CCamera* camera*/)
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProject;

	//Setup Matrix view
	D3DXVECTOR3 l_Eye(0.0f,5.0f,-5.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
	D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

	//Setup Matrix projection
	D3DXMatrixPerspectiveFovLH( &m_matProject, 45.0f * D3DX_PI / 180.0f, 1.0f, 1.0f, 100.0f );

	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
}

void CRenderManager::SetPaintSolid( bool paintSolid )
{
	m_bPaintSolid = paintSolid;
}

void CRenderManager::SetFullscreen( bool fullscreen )
{
	m_bFullscreen = fullscreen;
}

void CRenderManager::SetColorDebug( CColor color )
{
	m_BackbufferColor_debug = color;
}

void CRenderManager::SetColorRelease( CColor color )
{
	m_BackbufferColor_release = color;
}

bool CRenderManager::GetPaintSolid() const
{
	return m_bPaintSolid;
}

void CRenderManager::SetScreenSize( Vect2i size )
{
	m_Size = size;
}

HWND CRenderManager::GetHWND() const
{
	return m_hWnd;
}

LPDIRECT3D9 CRenderManager::GetD3D() const
{
	return m_pD3D;
}

LPDIRECT3DDEVICE9 CRenderManager::GetDevice() const
{
	return m_pD3DDevice;
}