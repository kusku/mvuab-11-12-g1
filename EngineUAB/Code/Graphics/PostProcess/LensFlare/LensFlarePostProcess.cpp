

#include "LensFlarePostProcess.h"
#include "Textures\Texture.h"
#include "Core.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "RenderManager.h"


SFlare::SFlare( float position, float scale, CColor color, std::string textureName)		
	: m_Position(position)
	, m_Scale(scale)
	, m_Color(color)
	, m_TextureName(textureName)
	, m_Texture(NULL)
{
}

SFlare::~SFlare()
{ 
	CHECKED_DELETE(m_Texture);
}


CLensFlarePostProcess::CLensFlarePostProcess()
	: m_LightBehindCamera(false)
	, m_GlowSprite(NULL)
	, m_OcclusionQueryActive(false)
	, m_OcclusionAlpha(0.0f)
	, m_GlowSize(250.0f)
	, m_QuerySize(75.0f)
	, m_LensFlarBS(TGraphicBlendStates::NoColorWriteChannels)
	, m_LightDirection(v3fZERO)
	, m_LightPosition(v2fZERO)
	, m_OcclusionQuery(NULL)
	, m_VertexBuffer(NULL)
	, m_BasicTechnique(NULL)
	, m_Effect(NULL)
	, m_SpriteBatch(NULL)
	, m_WorldParam(NULL)
	, m_ViewParam(NULL)
	, m_ProjParam(NULL)
	, m_GlowColor(colWHITE)
{
	m_Flares[0] = SFlare(-0.5f, 0.7f, CColor( 50.0f / 255.0f,  25.0f / 255.0f,  50.0f / 255.0f), "./Data/General/Textures/LensFlare/flare1.dds");
	m_Flares[1] = SFlare( 0.3f, 0.4f, CColor(100.0f / 255.0f, 255.0f / 255.0f, 200.0f / 255.0f), "./Data/General/Textures/LensFlare/flare1.dds");
	m_Flares[2] = SFlare( 1.2f, 1.0f, CColor(100.0f / 255.0f,  50.0f / 255.0f,  50.0f / 255.0f), "./Data/General/Textures/LensFlare/flare1.dds");
	m_Flares[3] = SFlare( 1.5f, 1.5f, CColor( 50.0f / 255.0f, 100.0f / 255.0f,  50.0f / 255.0f), "./Data/General/Textures/LensFlare/flare1.dds");

	m_Flares[4] = SFlare(-0.3f, 0.7f, CColor(200.0f / 255.0f,  50.0f / 255.0f,  50.0f / 255.0f), "./Data/General/Textures/LensFlare/flare2.dds");
	m_Flares[5] = SFlare( 0.6f, 0.9f, CColor( 50.0f / 255.0f, 100.0f / 255.0f,  50.0f / 255.0f), "./Data/General/Textures/LensFlare/flare2.dds");
	m_Flares[6] = SFlare( 0.7f, 0.4f, CColor( 50.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f), "./Data/General/Textures/LensFlare/flare2.dds");

	m_Flares[7] = SFlare(-0.7f, 0.7f, CColor( 50.0f / 255.0f, 100.0f / 255.0f,  25.0f / 255.0f), "./Data/General/Textures/LensFlare/flare3.dds");
	m_Flares[8] = SFlare( 0.0f, 0.6f, CColor( 25.0f / 255.0f,  25.0f / 255.0f,  25.0f / 255.0f), "./Data/General/Textures/LensFlare/flare3.dds");
	m_Flares[9] = SFlare( 2.0f, 1.4f, CColor( 25.0f / 255.0f,  50.0f / 255.0f, 100.0f / 255.0f), "./Data/General/Textures/LensFlare/flare3.dds");
}

CLensFlarePostProcess::~CLensFlarePostProcess()
{
	CHECKED_DELETE(m_GlowSprite);
	CHECKED_RELEASE(m_OcclusionQuery);
	CHECKED_RELEASE(m_VertexBuffer);
	CHECKED_RELEASE(m_SpriteBatch);
}

void CLensFlarePostProcess::Init()
{
	m_GlowSprite = new CTexture();

	m_GlowSprite->Load("./Data/General/Textures/LensFlare/glow.dds");

	for(int i = 0; i < 10; ++i)
	{
		m_Flares[i].m_Texture = new CTexture();

		m_Flares[i].m_Texture->Load(m_Flares[i].m_TextureName);
	}

	ZeroMemory(m_QueryVertices, sizeof(TCOLORED_VERTEX) * 4);

	m_QueryVertices[0].x = -m_QuerySize / 2;
	m_QueryVertices[0].y = -m_QuerySize / 2;
	m_QueryVertices[0].z = 1.0f;

	m_QueryVertices[1].x = m_QuerySize / 2;
	m_QueryVertices[1].y = -m_QuerySize / 2;
	m_QueryVertices[1].z = 1.0f;

	m_QueryVertices[2].x = -m_QuerySize / 2;
	m_QueryVertices[2].y = m_QuerySize / 2;
	m_QueryVertices[2].z = 1.0f;

	m_QueryVertices[3].x = m_QuerySize / 2;
	m_QueryVertices[3].y = m_QuerySize / 2;
	m_QueryVertices[3].z = 1.0f;

	LPDIRECT3DDEVICE9 dx9Device = CORE->GetRenderManager()->GetDevice();

	//Set VB to VRAM
	uint32 sizeVB = sizeof(TCOLORED_VERTEX) * 4;
	void* data = NULL;
	dx9Device->CreateVertexBuffer(sizeVB, 0, 0, D3DPOOL_DEFAULT, &m_VertexBuffer, NULL);
	m_VertexBuffer->Lock(0, sizeVB, &data, 0);
	memcpy(data, m_QueryVertices, sizeVB);
	m_VertexBuffer->Unlock();

	dx9Device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &m_OcclusionQuery);
	
	D3DXCreateSprite(dx9Device, &m_SpriteBatch);
	
	m_BasicTechnique = CORE->GetEffectManager()->GetEffectTechnique("BasicEffectTechnique");
	assert(m_BasicTechnique);
	
	m_Effect = CORE->GetEffectManager()->GetEffect("BasicEffect");

	m_Effect->GetParameterBySemantic("WORLD", m_WorldParam);
	m_Effect->GetParameterBySemantic("VIEW", m_ViewParam);
	m_Effect->GetParameterBySemantic("PROJECTION", m_ProjParam);
}

void CLensFlarePostProcess::Render(const Vect3f& lightDirection)
{
	m_LightDirection = lightDirection;

	UpdateOcclusion();
	
	DrawGlows();
	DrawFlares();

	CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::DefaultState);
	CORE->GetRenderManager()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CLensFlarePostProcess::UpdateOcclusion()
{
	Mat44f infView = CORE->GetCamera()->GetViewMatrix();

	//Set trans to 0
	infView.m03 = 0.0f;
	infView.m13 = 0.0f;
	infView.m23 = 0.0f;

	Vect3f projPos = Get2DScreenSpace(-m_LightDirection, CORE->GetCamera()->GetProjectionMatrix(), infView);

	//Light Behind
	if ((projPos.z < 0) || (projPos.z > 1))
	{
		m_LightBehindCamera = true;
		return;
	}

	m_LightPosition = Vect2f(projPos.x, projPos.y);
	m_LightBehindCamera = false;
	
	if(m_OcclusionQueryActive)
	{
		DWORD numberOfPixelsDrawn = 0;

		bool queryNotCompleted = (S_FALSE == m_OcclusionQuery->GetData(&numberOfPixelsDrawn, sizeof(DWORD), D3DGETDATA_FLUSH));

		if(queryNotCompleted)
		{
			return;
		}

		const float queryArea = m_QuerySize * m_QuerySize;

		m_OcclusionAlpha = mathUtils::Min( (float)numberOfPixelsDrawn / queryArea, 1.0f);
	}

	CORE->GetRenderManager()->SetGraphicBlendState(m_LensFlarBS);
	CORE->GetRenderManager()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);

	D3DXMATRIX dxProj;
	Vect2i screenSize = CORE->GetRenderManager()->GetScreenSize();
	D3DXMatrixOrthoOffCenterLH(&dxProj, 0, (float)screenSize.x, (float)screenSize.y, 0, 0, 1);

	D3DXMATRIX dxWorld;
	D3DXMatrixTranslation(&dxWorld, m_LightPosition.x, m_LightPosition.y, 0);

	D3DXMATRIX dxView;
	D3DXMatrixIdentity(&dxView);

	m_Effect->GetD3DEffect()->SetMatrix(m_ViewParam, &dxView);
	m_Effect->GetD3DEffect()->SetMatrix(m_WorldParam, &dxWorld);
	m_Effect->GetD3DEffect()->SetMatrix(m_ProjParam, &dxProj);

	LPDIRECT3DDEVICE9 dxDevice = CORE->GetRenderManager()->GetDevice();

	dxDevice->SetVertexDeclaration(TCOLORED_VERTEX::GetVertexDeclaration());
	dxDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(TCOLORED_VERTEX));

	UINT passes = 0;

	m_Effect->GetD3DEffect()->Begin(&passes, 0);
	m_Effect->GetD3DEffect()->BeginPass(0);

	// Issue the occlusion query.
	m_OcclusionQuery->Issue(D3DISSUE_BEGIN);

	dxDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_OcclusionQuery->Issue(D3DISSUE_END);

	m_Effect->GetD3DEffect()->EndPass();
	m_Effect->GetD3DEffect()->End();

	m_OcclusionQueryActive = true;

	CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::DefaultState);
}

void CLensFlarePostProcess::DrawGlows()
{
	if(m_LightBehindCamera || m_OcclusionAlpha <= 0)
	{
		return;
	}

	CColor color = m_GlowColor * m_OcclusionAlpha;
	Vect2f origin((float)m_GlowSprite->GetWidth() / 2, (float)m_GlowSprite->GetHeight() / 2);
	float scale = m_GlowSize * 2 / (float)m_GlowSprite->GetWidth();

	D3DXVECTOR3 lp(
		m_LightPosition.x / scale, 
		m_LightPosition.y / scale, 
		0);

	D3DXVECTOR3 center(
		origin.x , 
		origin.y , 
		0);

	D3DXVECTOR2 scale2DDX(
		scale,
		scale
		);

	D3DXMATRIX scaleDX;
	D3DXMatrixTransformation2D(&scaleDX, NULL, 0, &scale2DDX, NULL, 0, NULL);

	//CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::AlphaBlend);

	m_SpriteBatch->Begin(D3DXSPRITE_ALPHABLEND);

	m_SpriteBatch->SetTransform(&scaleDX);

	m_SpriteBatch->Draw(m_GlowSprite->GetDXTexture(), NULL, &center, &lp, (D3DCOLOR)color.GetUint32Argb());

	m_SpriteBatch->End();
}

void CLensFlarePostProcess::DrawFlares()
{
	if(m_LightBehindCamera || m_OcclusionAlpha <= 0)
	{
		return;
	}

	Vect2i screenSize = CORE->GetRenderManager()->GetScreenSize();

	Vect2f screenCenter((float)screenSize.x / 2.0f, (float)screenSize.y / 2.0f);

	Vect2f flareVector = screenCenter - m_LightPosition;

	D3DXMATRIX scaleDX;

	CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::Additive);

	m_SpriteBatch->Begin(0);

	for(uint32 i = 0; i < 10; ++i)
	{
		CColor flareColor = m_Flares[i].m_Color;
		flareColor.SetAlpha(flareColor.GetAlpha() * m_OcclusionAlpha);

		Vect2f flarePosition = m_LightPosition + flareVector * m_Flares[i].m_Position;

		D3DXVECTOR3 flarePositionDX(
						flarePosition.x / m_Flares[i].m_Scale,
						flarePosition.y / m_Flares[i].m_Scale,
						0);

		D3DXVECTOR3 flareOrigin(
						(float)m_Flares[i].m_Texture->GetWidth()  / 2 ,  
						(float)m_Flares[i].m_Texture->GetHeight() / 2 ,
						0);

		D3DXVECTOR2 scale2DDX(m_Flares[i].m_Scale, m_Flares[i].m_Scale);
		D3DXMatrixTransformation2D(&scaleDX, NULL, 0, &scale2DDX, NULL, 0, NULL);
		m_SpriteBatch->SetTransform(&scaleDX);

		m_SpriteBatch->Draw(m_Flares[i].m_Texture->GetDXTexture(), NULL, &flareOrigin, &flarePositionDX, (D3DCOLOR)flareColor.GetUint32Argb());
	}

	m_SpriteBatch->End();

	CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::DefaultState);
}

Vect3f CLensFlarePostProcess::Get2DScreenSpace(const Vect3f& pos, const Mat44f& proj, const Mat44f& view)
{
	Vect3f projected = v3fZERO;

	Vect2i screenSize = CORE->GetRenderManager()->GetScreenSize();

	D3DXMATRIX wvpDX;
	D3DXMATRIX projDX = proj.GetD3DXMatrix();
	D3DXMATRIX viewDX = view.GetD3DXMatrix();

	D3DXMatrixMultiply(&wvpDX, &viewDX, &projDX);

	D3DXVECTOR4 posDX(pos.x, pos.y, pos.z, 1);
	
	D3DXVECTOR4 transform;
	D3DXVec4Transform(&transform, &posDX, &wvpDX);

	//Vect4f screenVect = wvp * Vect4f(pos, 1.0f);

	//screenVect /= screenVect.w;
	transform /= transform.w;

	//Assign Z
	//projected.z = screenVect.z;
	projected.z = transform.z;

	//Assign X & Y
	//projected.x = (screenSize.x * ( (screenVect.x + 1) / 2 ));
	//projected.y = (screenSize.y * ( (-screenVect.y + 1) / 2 ));
	projected.x = (screenSize.x * ( (transform.x + 1) / 2 ));
	projected.y = (screenSize.y * ( (-transform.y + 1) / 2 ));

	return projected;
}
