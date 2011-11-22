#include "RenderManager.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "Base.h"
#include "Math\Vector3.h"
#include "Math\MathUtils.h"
#include <assert.h>
#include <string>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderManager::CRenderManager()
	: m_uWidth(0)
	, m_uHeight(0)
	, m_bIsOk(false)
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
	Release();
}

void CRenderManager::Release()
{
	CHECKED_RELEASE( m_pD3D );
	CHECKED_RELEASE( m_pD3DDevice );
}

void CRenderManager::Done ()
{
	if ( IsOk() )
	{
		Release();
		m_bIsOk = false;
	}
}

bool CRenderManager::Init(HWND hWnd)
{
	m_bIsOk = false;

   // Create the D3D object.
   m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
   assert(m_pD3D);

   m_bIsOk = m_pD3D != NULL;
   if ( m_bIsOk )
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
           LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: La resolucion de pantalla es (%dx%d)",m_uWidth,m_uHeight);
       }   
    }

   if (!m_bIsOk)
   {
       std::string msg_error = "Rendermanager::Init-> Error al inicializar Direct3D";
       LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
       Release();
       throw CException(__FILE__, __LINE__, msg_error);
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
	//D3DXVECTOR3 l_Eye(0.0f,5.0f,-5.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
	D3DXVECTOR3 l_Eye(5.0f,5.0f,5.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
	D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

	//Setup Matrix projection
	D3DXMatrixPerspectiveFovLH( &m_matProject, 45.0f * D3DX_PI / 180.0f, 1.0f, 1.0f, 100.0f );

	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
}

void CRenderManager::DrawLine( const Vect3f &PosA, const Vect3f &PosB, CColor Color)
{
   DWORD color_aux = Color.GetUint32Argb();

   CUSTOMVERTEX v[2] =
   {
       {PosA.x, PosA.y, PosA.z, color_aux},
       {PosB.x, PosB.y, PosB.z, color_aux},
   };

   m_pD3DDevice->SetTexture(0,NULL);
   m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
   m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,1, v,sizeof(CUSTOMVERTEX));
}

void CRenderManager::DrawAxis( float size )
{
	Vect3f center = Vect3f(0.0f, 0.0f, 0.0f);
	DrawLine( center, Vect3f(size, 0.0f, 0.0f), colRED );
	DrawLine( center, Vect3f(0.0f, size, 0.0f), colGREEN );
	DrawLine( center, Vect3f(0.0f, 0.0f, size), colBLUE );
}

void CRenderManager::DrawGrid(float sizeW, float sizeH, uint16 lines, CColor color )
{
	assert(lines > 0);
	assert(sizeW > 0);
	assert(sizeH > 0);

	float x = -sizeW / 2;
	float y = -sizeH / 2;

	--lines;
	for(int i=0; i<=lines; ++i)
	{
		DrawLine( Vect3f(x + ((i*sizeW)/lines), 0.0f, -sizeH/2), Vect3f(x + ((i*sizeW)/lines), 0.0f, sizeH/2), color);
		DrawLine( Vect3f(-sizeW/2, 0.0f, y + ((i*sizeH)/lines)), Vect3f(sizeW/2, 0.0f, y+ ((i*sizeH)/lines)), color);
	}
}

void CRenderManager::DrawCube(const Vect3f &dimensions, CColor color)
{
	//Draw a square
	DrawLine( Vect3f(-dimensions.x, -dimensions.y, -dimensions.z), Vect3f(dimensions.x, -dimensions.y, -dimensions.z), color );
	DrawLine( Vect3f(-dimensions.x, -dimensions.y, -dimensions.z), Vect3f(-dimensions.x, dimensions.y, -dimensions.z), color );
	DrawLine( Vect3f(-dimensions.x, dimensions.y, -dimensions.z), Vect3f(dimensions.x, dimensions.y, -dimensions.z), color );
	DrawLine( Vect3f(dimensions.x, -dimensions.y, -dimensions.z), Vect3f(dimensions.x, dimensions.y, -dimensions.z), color );

	//Draw another square
	DrawLine( Vect3f(-dimensions.x, -dimensions.y, dimensions.z), Vect3f(dimensions.x, -dimensions.y, dimensions.z), color );
	DrawLine( Vect3f(-dimensions.x, -dimensions.y, dimensions.z), Vect3f(-dimensions.x, dimensions.y, dimensions.z), color );
	DrawLine( Vect3f(-dimensions.x, dimensions.y, dimensions.z), Vect3f(dimensions.x, dimensions.y, dimensions.z), color );
	DrawLine( Vect3f(dimensions.x, -dimensions.y, dimensions.z), Vect3f(dimensions.x, dimensions.y, dimensions.z), color );
	
	//Draw the conectors linking the squares
	DrawLine ( Vect3f(-dimensions.x, -dimensions.y, -dimensions.z), Vect3f(-dimensions.x, -dimensions.y, dimensions.z), color );
	DrawLine ( Vect3f(dimensions.x, -dimensions.y, -dimensions.z), Vect3f(dimensions.x, -dimensions.y, dimensions.z), color );
	DrawLine ( Vect3f(-dimensions.x, dimensions.y, -dimensions.z), Vect3f(-dimensions.x, dimensions.y, dimensions.z), color );
	DrawLine ( Vect3f(dimensions.x, dimensions.y, -dimensions.z), Vect3f(dimensions.x, dimensions.y, dimensions.z), color );
}

void CRenderManager::DrawSphere(float radius, uint32 edges, CColor color )
{
	for(int t=0;t<static_cast<int>(edges);++t)
   {
       float l_radiusRing=radius*sin(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges));
       for(int b=0;b<static_cast<int>(edges);++b)
       {		   
           Vect3f l_PosA(l_radiusRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges))),radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges)),l_radiusRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges))));
           Vect3f l_PosB(l_radiusRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)(b+1))/((float)edges))),radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges)),l_radiusRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)(b+1))/((float)edges))));
           DrawLine(l_PosA,l_PosB,color);
           
           float l_radiusNextRing=radius*sin(mathUtils::Deg2Rad<float>(180.0f*((float)(t+1)))/((float)edges));
           
           Vect3f l_PosC(l_radiusRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges))),radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges)),l_radiusRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges))));
           Vect3f l_PosD(l_radiusNextRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges))),radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)(t+1)))/((float)edges)),l_radiusNextRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges))));
           DrawLine(l_PosC,l_PosD,color);
       }
   }
}

void CRenderManager::SetTransform( const Mat44f &mat)
{
	D3DXMATRIX aux( mat.m00, mat.m10, mat.m20, mat.m30,
					mat.m01, mat.m11, mat.m21, mat.m31,
					mat.m02, mat.m12, mat.m22, mat.m32,
					mat.m03, mat.m13, mat.m23, mat.m33 );

	m_pD3DDevice->SetTransform(D3DTS_WORLD, &aux);
}