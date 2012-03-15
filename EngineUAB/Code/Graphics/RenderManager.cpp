#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "Textures\Texture.h"
#include "Base.h"
#include "Core.h"
#include "Math\Vector3.h"
#include "Math\MathUtils.h"
#include "Vertexs\VertexType.h"
#include "Stadistics\Stadistics.h"
#include <assert.h>
#include <string>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderManager::CRenderManager()
	: m_uWidth(0)
	, m_uHeight(0)
	, m_SizeScreen(800, 600)
	, m_bIsOk(false)
	, m_AspectRatio(0.0f)
	, m_bPaintSolid(true)
	, m_bFullscreen(true)
	, m_hWnd(NULL)
	, m_pD3D(NULL)
	, m_pD3DDevice(NULL)
	, m_BackbufferColor_debug(colBLUE)
	, m_BackbufferColor_release(colBLACK)
	, m_HalfPixel(0.0f, 0.0f)
	, m_NowTargetSize(0.0f)
	, m_VBQuad(NULL)
	, m_IBQuad(NULL)
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
	CHECKED_RELEASE( m_VBQuad );
	CHECKED_RELEASE( m_IBQuad );
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
			d3dpp.BackBufferWidth   = m_SizeScreen.x;
			d3dpp.BackBufferHeight  = m_SizeScreen.y;
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

			//FXAA
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, 1);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, 0);
			m_pD3DDevice->SetRenderState(D3DRS_SRGBWRITEENABLE,1);
			m_pD3DDevice->SetRenderState(D3DRS_SRGBWRITEENABLE,0);

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
				m_uWidth    = m_SizeScreen.x;
				m_uHeight    = m_SizeScreen.y;
			}
			else
			{
				GetWindowRect(hWnd);
			}

			LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: La resolucion de pantalla es (%dx%d)",m_uWidth,m_uHeight);

			m_AspectRatio = static_cast<float>(m_uWidth) / static_cast<float>(m_uHeight);
			m_HalfPixel = Vect2f(0.5f / (float)m_uWidth, 0.5f / (float)m_uHeight);
		}
	}

	if (!m_bIsOk)
	{
		std::string msg_error = "Rendermanager::Init-> Error al inicializar Direct3D";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		Release();
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m_hWnd = hWnd;
	CreateQuadBuffers();

	return m_bIsOk;
}

void CRenderManager::DrawPlane(float size, const Vect3f& normal, float distance, CColor Color, int GridX, int GridZ )
{
  assert(IsOk());
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);

	//Ax + By + Cz + D = 0
	//if (x,y) = (0,0) then z = -D/C
	//if (x,y) = (1,1) then z = (-D-A-B)/C
	//With two points we have a vector-->

	float A,B,C,D;
	A = normal.x; 
	B = normal.y; 
	C = normal.z; 
	D = distance; 
	//float pointAz;
	//float pointBz;

	Vect3f pointA, pointB;
	if( C!= 0)
	{
		pointA = Vect3f(0.f,0.f, -D/C);
		pointB = Vect3f(1.f,1.f, (D-A-B)/C);
	}
	else if( B!= 0)
	{
		pointA = Vect3f(0.f,-D/B, 0.f);
		pointB = Vect3f(1.f,(-D-A-C)/B,1.f);
	}
	else if( A != 0)
	{
		pointA = Vect3f(-D/A,0.f, 0.f);
		pointB = Vect3f((-D-B-C)/A,1.f,1.f);
	}
	else
	{
		//error.
	}


	Vect3f vectorA = pointB - pointA;
  vectorA.Normalize();
	Vect3f vectorB;
  vectorB = normal^vectorA;
  vectorB.Normalize();
	Vect3f initPoint = normal*distance;

	assert(GridX>0);
	assert(GridZ>0);
	//LINEAS EN Z
	Vect3f initPointA = initPoint - vectorB*size*0.5;
	for(int b=0;b<=GridX;++b)
	{		
		DrawLine(initPointA + vectorA*size*0.5, initPointA - vectorA*size*0.5, Color );

		initPointA += vectorB*size/(float)GridX;
	}
	initPointA = initPoint - vectorA*size*0.5;
	for(int b=0;b<=GridX;++b)
	{
		DrawLine(initPointA + vectorB*size*0.5, initPointA - vectorB*size*0.5, Color);
		initPointA += vectorA*size/(float)GridX;
	}
}

void CRenderManager::GetWindowRect( HWND hwnd )
{
	RECT rec_window;
	GetClientRect(    hwnd, &rec_window);
	m_uWidth = rec_window.right - rec_window.left;
	m_uHeight = rec_window.bottom - rec_window.top;
}

void CRenderManager::ClearTarget(CColor color)
{
	D3DCOLOR col = D3DCOLOR_ARGB(
		(uint32) (color.GetAlpha() * 255),
		(uint32) (color.GetRed() * 255),
		(uint32) (color.GetGreen() * 255),
		(uint32) (color.GetBlue()* 255)
		);

	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, col, 1.0f, 0 );
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

void CRenderManager::SetupMatrices(CCamera* camera)
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProject;
	D3DXVECTOR3 l_Eye;

	if(!camera)
	{
		//Set default view and projection matrix
		//Setup Matrix view
		l_Eye=D3DXVECTOR3(5.0f,5.0f,-5.0f);
		D3DXVECTOR3 l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
		D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

		//Setup Matrix projection
		D3DXMatrixPerspectiveFovLH( &m_matProject, 45.0f * D3DX_PI / 180.0f, m_AspectRatio, 1.0f, 10000.0f );
	}
	else
	{
		camera->UpdateMatrices();

		m_matView = camera->GetViewMatrixDX();

		m_matProject = camera->GetProjectionMatrixDX();
	}

	m_Frustum.Update( m_matView * m_matProject );
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

	CORE->GetEffectManager()->ActivateCamera(m_matView , m_matProject, Vect3f(l_Eye.x, l_Eye.y, l_Eye.z));
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

#if defined(_DEBUG)
		//Capture Info for Stadistics
		CStadistics *l_pStadistics = CORE->GetStadistics();

		l_pStadistics->AddDrawCall();
		l_pStadistics->AddDebugLine();
		l_pStadistics->AddVerticesInFrustum(2);
#endif
}

void CRenderManager::DrawAxis( float size )
{
	Vect3f center = Vect3f(0.0f, 0.0f, 0.0f);
	DrawLine( center, Vect3f(size, 0.0f, 0.0f), colRED );
	DrawLine( center, Vect3f(0.0f, size, 0.0f), colGREEN );
	DrawLine( center, Vect3f(0.0f, 0.0f, size), colCYAN );
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

void CRenderManager::DrawCone(float size, uint16 edges, CColor color)
{
	uint16 t = static_cast<uint16>(edges/2);
	float radius = size/2;
	float l_radiusRing=radius*sin(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges));
	for(int b=0;b<static_cast<int>(edges);++b)
	{		   
		Vect3f l_PosA(l_radiusRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges))),radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges)),l_radiusRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)b)/((float)edges))));
		Vect3f l_PosB(l_radiusRing*cos(mathUtils::Deg2Rad<float>((float)(360.0f*(float)(b+1))/((float)edges))),radius*cos(mathUtils::Deg2Rad<float>(180.0f*((float)t))/((float)edges)),l_radiusRing*sin(mathUtils::Deg2Rad<float>((float)(360.0f*(float)(b+1))/((float)edges))));

		l_PosA.y += size;
		l_PosB.y += size;

		DrawLine(l_PosA,l_PosB,color);
	}

	DrawLine( Vect3f(0.f, 0.f, 0.f), Vect3f(size/2.f, size, 0.f), color );
	DrawLine( Vect3f(0.f, 0.f, 0.f), Vect3f(0.f, size, -size/2.f), color );
	DrawLine( Vect3f(0.f, 0.f, 0.f), Vect3f(0.f, size, size/2.f), color );
	DrawLine( Vect3f(0.f, 0.f, 0.f), Vect3f(-size/2.f, size, 0.f), color );
}

void CRenderManager::DrawTower(float size, CColor color)
{
	float l_Size = size/2.f;
	DrawLine( Vect3f(l_Size, 0.f, l_Size), Vect3f(l_Size, 0.f, -l_Size), color );
	DrawLine( Vect3f(l_Size, 0.f, -l_Size), Vect3f(-l_Size, 0.f, -l_Size), color );
	DrawLine( Vect3f(-l_Size, 0.f, -l_Size), Vect3f(-l_Size, 0.f, l_Size), color );
	DrawLine( Vect3f(-l_Size, 0.f, l_Size), Vect3f(l_Size, 0.f, l_Size), color );

	float height = size*2.f;
	DrawLine( Vect3f(l_Size, height, l_Size), Vect3f(l_Size, height, -l_Size), color );
	DrawLine( Vect3f(l_Size, height, -l_Size), Vect3f(-l_Size, height, -l_Size), color );
	DrawLine( Vect3f(-l_Size, height, -l_Size), Vect3f(-l_Size, height, l_Size), color );
	DrawLine( Vect3f(-l_Size, height, l_Size), Vect3f(l_Size, height, l_Size), color );

	DrawLine( Vect3f(l_Size, 0.f, l_Size), Vect3f(l_Size, height, l_Size), color );
	DrawLine( Vect3f(l_Size, 0.f, -l_Size), Vect3f(l_Size, height, -l_Size), color );
	DrawLine( Vect3f(-l_Size, 0.f, -l_Size), Vect3f(-l_Size, height, -l_Size), color );
	DrawLine( Vect3f(-l_Size, 0.f, l_Size), Vect3f(-l_Size, height, l_Size), color );

	DrawLine( Vect3f(size, height, size), Vect3f(size, height, -size), color );
	DrawLine( Vect3f(size, height, -size), Vect3f(-size, height, -size), color );
	DrawLine( Vect3f(-size, height, -size), Vect3f(-size, height, size), color );
	DrawLine( Vect3f(-size, height, size), Vect3f(size, height, size), color );

	DrawLine( Vect3f(l_Size, height, l_Size), Vect3f(size, height, size), color );
	DrawLine( Vect3f(l_Size, height, -l_Size), Vect3f(size, height, -size), color );
	DrawLine( Vect3f(-l_Size, height, -l_Size), Vect3f(-size, height, -size), color );
	DrawLine( Vect3f(-l_Size, height, l_Size), Vect3f(-size, height, size), color );
}

void CRenderManager::DrawQuad2D(const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color)
{
	Vect2i finalPos = pos;
	CalculateAlignment(w, h, alignment, finalPos);

	// finalPos = [0]
	//
	//  [0]------[2]
	//   |		  |
	//   |        |
	//   |		  |
	//  [1]------[3]

	unsigned short indices[6]={0,2,1,1,2,3};
	SCREEN_COLOR_VERTEX v[4] =
	{
		{ (float)finalPos.x, (float)finalPos.y, 0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) sup_esq.

		,{ (float)finalPos.x, (float)finalPos.y+h, 0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) inf_esq.

		,{ (float)finalPos.x+w, (float)finalPos.y, 0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) sup_dr.

		,{ (float)finalPos.x+w, (float)finalPos.y+h, 0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) inf_dr.

	};

	m_pD3DDevice->SetFVF( SCREEN_COLOR_VERTEX::getFlags() );
	m_pD3DDevice->SetTexture(0, NULL);
	m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,4,2,indices,D3DFMT_INDEX16,v,sizeof( SCREEN_COLOR_VERTEX ) );
}

void CRenderManager::DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CEffectTechnique* EffectTechnique, const CColor &color, Vect2f vec1, Vect2f vec2,
	float U0, float V0, float U1, float V1)
{
	EffectTechnique->BeginRender();
	LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetD3DEffect();
	if( l_Effect != NULL )
	{
		l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );
		UINT l_NumPasses = 0;
		l_Effect->Begin(&l_NumPasses, 0);
		for( UINT iPass = 0; iPass < l_NumPasses; ++iPass )
		{
			l_Effect->BeginPass( iPass );
			DrawColoredQuad2DTexturedInPixels( vec1, vec2, color, U0, V0, U1, V1 );
			l_Effect->EndPass();
		}
		l_Effect->End();
	}
}

void CRenderManager::DrawQuad2DTexturedInPixelsInFullScreen(CEffectTechnique* EffectTechnique)
{
	EffectTechnique->BeginRender();
	LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetD3DEffect();
	if( l_Effect != NULL )
	{
		l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );
		UINT l_NumPasses = 0;
		if( SUCCEEDED(l_Effect->Begin(&l_NumPasses, 0)) )
		{
			m_pD3DDevice->SetVertexDeclaration(TCOLOREDTEXTURE1_VERTEX::GetVertexDeclaration());
			m_pD3DDevice->SetStreamSource(0,m_VBQuad,0,sizeof(TCOLOREDTEXTURE1_VERTEX));
			m_pD3DDevice->SetIndices(m_IBQuad);

			for( UINT iPass = 0; iPass < l_NumPasses; ++iPass )
			{
				l_Effect->BeginPass( iPass );
				m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLEFAN, 0, 0,
						static_cast<UINT>(4), 0, static_cast<UINT>(2));
				l_Effect->EndPass();
			}
			l_Effect->End();
		}
	}
}

void CRenderManager::DrawColoredQuad2DTexturedInPixels(Vect2f vec1, Vect2f vec2, const CColor& color, float U0, float V0, float U1, float V1)
{
	//  [0]------[2]
	//   |		  |
	//   |        |
	//   |		  |
	//  [1]------[3]
	D3DCOLOR d3dColor = D3DCOLOR_COLORVALUE((color.GetRed()), 
		(color.GetGreen()),
		(color.GetBlue()),
		(color.GetAlpha()));

	uint16 indices[4] = {0,1,2,3};
	TCOLOREDTEXTURE1_VERTEX v[4] =
	{
		{ vec2.x, vec1.y, 0.0f, d3dColor, U1, V1 }
		,{ vec1.x, vec1.y, 0.0f, d3dColor, U0, V1 }
		,{ vec1.x, vec2.y, 0.0f, d3dColor, U0, V0 }
		,{ vec2.x, vec2.y, 0.0f, d3dColor, U1, V0 }
	};

	m_pD3DDevice->SetVertexDeclaration(TCOLOREDTEXTURE1_VERTEX::GetVertexDeclaration());
	m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLEFAN,0,4,2,indices,D3DFMT_INDEX16,v,sizeof( TCOLOREDTEXTURE1_VERTEX ) );
}

void CRenderManager::CalculateAlignment (uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & finalPos)
{
	switch (alignment)
	{
	case CENTER:
		{
			finalPos.x -= (uint32)(w*0.5f);
			finalPos.y -= (uint32)(h*0.5f);
		}
		break;
	case UPPER_LEFT:
		{
			//Por defecto ya est alienado de esta manera :)
		}
		break;
	case UPPER_RIGHT:
		{
			finalPos.x -= w;
		}
		break;
	case LOWER_RIGHT:
		{
			finalPos.x -= w;
			finalPos.y -= h;
		}
		break;
	case LOWER_LEFT:
		{
			finalPos.y -= h;
		}
		break;
	default:
		{
			LOGGER->AddNewLog(ELL_ERROR, "RenderManager:: Se está intentado renderizar un quad2d con una alineacion desconocida");
		}
		break;
	}   
}

void CRenderManager::DrawRectangle2D ( const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor, uint32 edge_w, uint32 edge_h, CColor& edgeColor )
{
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	//Draw background quad2D:
	DrawQuad2D(pos, w, h, UPPER_LEFT, backGroundColor);

	//Draw the four edges:
	//2 Horizontal:
	Vect2i pos_aux = pos;
	pos_aux.y -= edge_h;
	DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);

	pos_aux = pos;
	pos_aux.y += h;
	DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);

	//2 Vertical:
	pos_aux = pos;
	pos_aux.x -= edge_w;
	pos_aux.y -= edge_h;
	DrawQuad2D(pos_aux, edge_w, h + (2*edge_w), UPPER_LEFT, edgeColor);

	pos_aux.x = pos.x + w;
	DrawQuad2D(pos_aux, edge_w, h + (2*edge_w), UPPER_LEFT, edgeColor);   

	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

void CRenderManager::CreateQuadBuffers()
{
	D3DCOLOR d3dColor = D3DCOLOR_COLORVALUE((colWHITE.GetRed()), 
		(colWHITE.GetGreen()),
		(colWHITE.GetBlue()),
		(colWHITE.GetAlpha()));

	uint16 indices[4] = { 0, 1, 2, 3 };
	TCOLOREDTEXTURE1_VERTEX vertices[4] =
	{
		 {  1.f, -1.f, 0.0f, d3dColor, 1.f, 1.f }
		,{ -1.f, -1.f, 0.0f, d3dColor, 0.f, 1.f }
		,{ -1.f, 1.f, 0.0f, d3dColor, 0.f, 0.f }
		,{  1.f, 1.f, 0.0f, d3dColor, 1.f, 0.f }
	};

	//Create the Vertex Buffer and Index Buffer
	void *l_Data = NULL;
	UINT l_Length = sizeof(TCOLOREDTEXTURE1_VERTEX) * 4;
	m_pD3DDevice->CreateVertexBuffer( l_Length, 0, TCOLOREDTEXTURE1_VERTEX::GetFVF(), D3DPOOL_DEFAULT, &m_VBQuad, NULL);
	m_VBQuad->Lock(0, l_Length, &l_Data, 0);
	memcpy(l_Data, &vertices, l_Length);
	m_VBQuad->Unlock();

	l_Length = sizeof(uint16) * 4;
	m_pD3DDevice->CreateIndexBuffer( l_Length, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IBQuad, NULL);
	m_IBQuad->Lock(0, l_Length, &l_Data, 0);
	memcpy(l_Data, &indices, l_Length);
	m_IBQuad->Unlock();
}

void CRenderManager::SetTransform( const Mat44f &mat)
{
	D3DXMATRIX aux( mat.m00, mat.m10, mat.m20, mat.m30,
		mat.m01, mat.m11, mat.m21, mat.m31,
		mat.m02, mat.m12, mat.m22, mat.m32,
		mat.m03, mat.m13, mat.m23, mat.m33 );

	CORE->GetEffectManager()->SetWorldMatrix(aux);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &aux);
}