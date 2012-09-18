#include "SpotLight.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Math\Matrix44.h"
#include "Math\Vector2.h"
#include "XML\XMLTreeNode.h"
#include "Effects\EffectManager.h"
#include "Core.h"
#include "Base.h"
#include "Cameras\Camera.h"
#include "Core.h"
#include "Lights\LightManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "Textures\TextureManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSpotLight::CSpotLight(CXMLTreeNode &XMLNode)
{
	m_Type = SPOT;
	m_Direction = XMLNode.GetVect3fProperty("dir", Vect3f(0.0f, 0.0f, 0.0f));
	m_Angle = XMLNode.GetFloatProperty("angle", 0.0f);
	m_FallOff = XMLNode.GetFloatProperty("fall_off", 0.0f);
	
	//Extract Common Info
	ExtractCommonLightInfo(XMLNode);
}

CSpotLight::CSpotLight()
	: m_Angle(0.0f)
	, m_FallOff(0.0f)
{
	m_Direction = Vect3f(0.f,0.f,0.f);
	m_Type = SPOT;
}

CSpotLight::~CSpotLight()
{
}

void CSpotLight::Render(CRenderManager *RM)
{
	Mat44f mat, matRotYaw, matRotPitch;
	Vect3f l_Pos = GetPosition();

	mat.SetIdentity();
	matRotYaw.SetIdentity();
	matRotPitch.SetIdentity();

	mat.Translate( l_Pos );
	matRotYaw.RotByAngleZ( GetYaw() );
	matRotPitch.RotByAngleX( GetPitch() );

	mat = mat * matRotYaw * matRotPitch;

	RM->SetTransform( mat );
	RM->DrawCone(0.6f, 10, colYELLOW);

	mat.SetIdentity();
	RM->SetTransform(mat);
	Vect3f l_Dir = l_Pos + (m_Direction.GetNormalized() * 10.0f);
	RM->DrawLine( l_Pos , l_Dir );
	
	RM->SetTransform(m44fIDENTITY);

	if(m_LightFrustum != NULL)
	{
		m_LightFrustum->DrawFrustum();
	}
}

void CSpotLight::SetShadowMap()
{
	D3DXMATRIX l_View;
	D3DXMATRIX l_Projection;

	CEffectManager* l_EffectManager = CORE->GetEffectManager();

    D3DXVECTOR3 l_Eye = D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z);

    Vect3f lookat = m_Direction;
    D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);
	
    Vect3f vup(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);

    //Setup Matrix view
	D3DXMatrixLookAtLH( &l_View, &l_Eye, &l_LookAt, &l_VUP);
	
    //Setup Matrix projection
	D3DXMatrixPerspectiveFovLH(&l_Projection, D3DX_PI * .25f, 1.0f, 1.0f, m_EndRangeAttenuation);

	m_ViewShadowMap = Mat44f(l_View);
	m_ProjectionShadowMap= Mat44f(l_Projection);

	// Compute near and far
	float Near = CORE->GetCamera()->GetZn();
	float Far  = m_EndRangeAttenuation;

	// Compute linear (distance to light) near and min/max for this perspective matrix
	float CosLightFOV = mathUtils::Cos<float>(0.5f * D3DX_PI * .25f);
	m_LightLinNearFar.x = Near;
	m_LightLinNearFar.y  = Far / (CosLightFOV * CosLightFOV);

	m_LightFrustum->Update((m_ProjectionShadowMap * m_ViewShadowMap).GetD3DXMatrix());
	l_EffectManager->SetShadowCameraEye(m_Position);
	l_EffectManager->SetShadowProjectionMatrix(m_ProjectionShadowMap);
	l_EffectManager->SetShadowViewMatrix(m_ViewShadowMap);
}

void CSpotLight::RenderShadows( CRenderManager* RM )
{
	CORE->GetLightManager()->SetCurrentFrustum(m_LightFrustum);

	if( m_GenerateStaticShadowMap && m_MustUpdateStaticShadowMap )
	{
		m_pStaticShadowMap->SetAsRenderTarget(0);
		m_StaticDepthStencil->SetAsDepthStencil();

		RM->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0 );

		CORE->GetEffectManager()->SetLightShadowLinNearFar(m_LightLinNearFar);

		for(size_t i=0; i<m_StaticShadowMapRenderableObjectsManagers.size(); ++i)
		{
			m_StaticShadowMapRenderableObjectsManagers[i]->Render(RM);
		}

		m_MustUpdateStaticShadowMap = false;

		m_pStaticShadowMap->UnsetAsRenderTarget(0);
		m_StaticDepthStencil->UnsetAsDepthStencil();
	}

	if( m_DynamicShadowMapRenderableObjectsManagers.size() > 0)
	{
		m_pDynamicShadowMap->SetAsRenderTarget(0);
		m_DynamicDepthStencil->SetAsDepthStencil();

		RM->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0 );

		CORE->GetEffectManager()->SetLightShadowLinNearFar(m_LightLinNearFar);

		for(size_t i=0; i<m_DynamicShadowMapRenderableObjectsManagers.size(); ++i)
		{
			m_DynamicShadowMapRenderableObjectsManagers[i]->Render(RM);
		}

		m_pDynamicShadowMap->UnsetAsRenderTarget(0);
		m_DynamicDepthStencil->UnsetAsDepthStencil();
	}

	CORE->GetLightManager()->SetCurrentFrustum(NULL);
}

void CSpotLight::CreateShadowTextures(bool staticMap, CTexture::TFormatType formatStatic, uint32 staticWidth, uint32 staticHeight, bool dynamicMap, CTexture::TFormatType formatDynamic, uint32 dynamicWidth, uint32 dynamicHeight)
{
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	if(dynamicMap)
	{
		std::string l_TextureName = m_Name + "_dynamic";
		l_pTextureManager->RemoveResource( l_TextureName );

		m_pDynamicShadowMap = new CTexture();
		
		m_pDynamicShadowMap->Create(l_TextureName, dynamicWidth, dynamicHeight, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, formatDynamic);

		l_pTextureManager->AddResource(m_pDynamicShadowMap->GetName(), m_pDynamicShadowMap);

		//////Depth Stencil

		//Elimina la textura si ya existía
		std::string l_TextureNameDS = m_Name + "_dynamic_ds";
		l_pTextureManager->RemoveResource( l_TextureNameDS );

		m_DynamicDepthStencil = new CTexture();
		m_DynamicDepthStencil->CreateDepthStencil(dynamicWidth, dynamicHeight, CTexture::D24S8, static_cast<D3DMULTISAMPLE_TYPE>(m_MultiSamples));
		l_pTextureManager->AddResource(l_TextureNameDS, m_DynamicDepthStencil);
	}

	if(staticMap)
	{
		std::string l_TextureName = m_Name + "_static";
		l_pTextureManager->RemoveResource( l_TextureName );

		m_pStaticShadowMap = new CTexture();
		m_pStaticShadowMap->Create(l_TextureName, staticWidth, staticHeight, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, formatStatic);

		l_pTextureManager->AddResource(m_pStaticShadowMap->GetName(), m_pStaticShadowMap);

		m_MustUpdateStaticShadowMap = true;

		//////Depth Stencil

		//Elimina la textura si ya existía
		std::string l_TextureNameDS = m_Name + "_static_ds";
		l_pTextureManager->RemoveResource( l_TextureNameDS );

		m_StaticDepthStencil = new CTexture();
		m_StaticDepthStencil->CreateDepthStencil(staticWidth, staticHeight, CTexture::D24S8, static_cast<D3DMULTISAMPLE_TYPE>(m_MultiSamples));
		l_pTextureManager->AddResource(l_TextureNameDS, m_StaticDepthStencil);
	}
}

void CSpotLight::CalculateOrientationDebugRender()
{
	Vect2f l_XYCam = Vect2f(0.0f, 1.0f);
	Vect2f l_YZCam = Vect2f(1.0f, 0.0f);

	Vect2f l_DirPlane;
	l_DirPlane.x = m_Direction.x;
	l_DirPlane.y = m_Direction.y;
	float l_Yaw = 0.f;
	if( l_DirPlane.Length() != 0.f)
	{
		if( m_Direction.y > 0.f )
		{
			if( m_Direction.x < 0.f )
				l_Yaw = mathUtils::ACos<float>((l_XYCam * l_DirPlane)/(l_XYCam.Length()*l_DirPlane.Length()));
			else
				l_Yaw = -mathUtils::ACos<float>((l_XYCam * l_DirPlane)/(l_XYCam.Length()*l_DirPlane.Length()));
		}
		else
		{
			if( m_Direction.x < 0.f )
				l_Yaw = mathUtils::ACos<float>((l_XYCam * l_DirPlane)/(l_XYCam.Length()*l_DirPlane.Length()));
			else
				l_Yaw =-mathUtils::ACos<float>((l_XYCam * l_DirPlane)/(l_XYCam.Length()*l_DirPlane.Length()));
		}
	}


	l_DirPlane.x = m_Direction.y;
	l_DirPlane.y = m_Direction.z;
	float l_Pitch = 0.f;
	if( l_DirPlane.Length() != 0.f)
	{
		if( m_Direction.y > 0.f )
		{
			if( m_Direction.z > 0.f )
				l_Pitch = mathUtils::ACos<float>((l_YZCam*l_DirPlane)/(l_YZCam.Length()*l_DirPlane.Length()));
			else
				l_Pitch =-mathUtils::ACos<float>((l_YZCam*l_DirPlane)/(l_YZCam.Length()*l_DirPlane.Length()));
		}
		else
		{
			if( m_Direction.z > 0.f )
				l_Pitch = D3DX_PI-mathUtils::ACos<float>((l_YZCam*l_DirPlane)/(l_YZCam.Length()*l_DirPlane.Length()));
			else
				l_Pitch = -D3DX_PI + mathUtils::ACos<float>((l_YZCam*l_DirPlane)/(l_YZCam.Length()*l_DirPlane.Length()));
		}
	}

	SetYaw(l_Yaw);
	SetPitch(l_Pitch);
}