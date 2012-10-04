#include "DirectionalLight.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Math\Matrix44.h"
#include "XML\XMLTreeNode.h"
#include "Effects\EffectManager.h"
#include "Base.h"
#include "Core.h"
#include "Cameras\Camera.h"
#include "Cameras\Frustum.h"
#include "Math\Vector3.h"
#include "BoundingObjects.h"
#include "Cameras\Frustum.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "Textures\TextureManager.h"
#include "Core.h"
#include "Lights\LightManager.h"
#include "Cameras\Frustum.h"
#include "Cameras\Camera.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDirectionalLight::CDirectionalLight( CXMLTreeNode &XMLNode )
	: m_NumCascades(0)
	, m_ShadowFrustums(NULL)
	, m_CascadeViewShadowMap(NULL)
	, m_CascadeProjectionShadowMap(NULL)
	, m_CascadeViewProjShadowMap(NULL)
	, m_SplitDepthsTmp(NULL)
	, m_CascadeShadowPositions(NULL)
	, m_DynamicShadowMapSize(v2iZERO)
	, m_StaticShadowMapSize(v2iZERO)
	, m_ShadowMapPixelSize(v2fZERO)
	, m_CascadeDistances(v3fZERO)
{

	m_Type = DIRECTIONAL;
	m_Direction = XMLNode.GetVect3fProperty("dir", v3fZERO);
	m_DirectionNormalize = m_Direction.GetNormalized();

	m_GenerateDynamicShadowMap = XMLNode.GetBoolProperty("generate_shadow_map", false, false);
	m_GenerateStaticShadowMap = XMLNode.GetBoolProperty("generate_static_shadow_map", false, false);

	if(m_GenerateDynamicShadowMap || m_GenerateStaticShadowMap)
	{
		m_OrthoShadowMapSize.x = XMLNode.GetIntProperty("directional_light_camara_size_w", 256);
		m_OrthoShadowMapSize.y = XMLNode.GetIntProperty("directional_light_camara_size_h", 256);

		m_NumCascades = XMLNode.GetIntProperty("cascade_num", 3);

		m_CascadeViewShadowMap = new Mat44f[m_NumCascades];
		ZeroMemory(m_CascadeViewShadowMap, sizeof(Mat44f) * m_NumCascades);

		m_CascadeProjectionShadowMap = new Mat44f[m_NumCascades];
		ZeroMemory(m_CascadeProjectionShadowMap, sizeof(Mat44f) * m_NumCascades);

		m_CascadeViewProjShadowMap = new Mat44f[m_NumCascades];
		ZeroMemory(m_CascadeViewProjShadowMap, sizeof(Mat44f) * m_NumCascades);

		m_CascadeShadowPositions = new Vect3f[m_NumCascades];
		ZeroMemory(m_CascadeShadowPositions, sizeof(Vect3f) * m_NumCascades);

		m_SplitDepthsTmp = new float[m_NumCascades + 1];
		ZeroMemory(m_SplitDepthsTmp, sizeof(float) * (m_NumCascades + 1));

		m_ShadowFrustums = new CFrustum*[m_NumCascades];
		for(uint32 i = 0; i < m_NumCascades; ++i)
		{
			m_ShadowFrustums[i] = new CFrustum();
		}
	}

	ZeroMemory(m_FrustumCornersVS, sizeof(Vect3f) * 8);
	ZeroMemory(m_SplitFrustumCornersVS, sizeof(Vect3f) * 8);

	//Extract Common Info
	ExtractCommonLightInfo(XMLNode);
}

CDirectionalLight::CDirectionalLight()
	: m_Direction(v3fZERO)
	, m_DirectionNormalize(v3fZERO)
	, m_OrthoShadowMapSize(Vect2i(256, 256))
	, m_ShadowFrustums(NULL)
	, m_NumCascades(0)
	, m_CascadeViewShadowMap(NULL)
	, m_CascadeViewProjShadowMap(NULL)
	, m_CascadeProjectionShadowMap(NULL)
	, m_SplitDepthsTmp(NULL)
	, m_CascadeShadowPositions(NULL)
	, m_DynamicShadowMapSize(v2iZERO)
	, m_StaticShadowMapSize(v2iZERO)
	, m_ShadowMapPixelSize(v2fZERO)
	, m_CascadeDistances(v3fZERO)
{
	ZeroMemory(m_FrustumCornersVS, sizeof(Vect3f) * 8);
	ZeroMemory(m_SplitFrustumCornersVS, sizeof(Vect3f) * 8);

	m_Type = DIRECTIONAL;
}

CDirectionalLight::~CDirectionalLight()
{
	for(uint32 i = 0; i < m_NumCascades; ++i)
	{
		CHECKED_DELETE(m_ShadowFrustums[i]);
	}

	CHECKED_DELETE_ARRAY(m_ShadowFrustums);
	CHECKED_DELETE_ARRAY(m_CascadeViewShadowMap);
	CHECKED_DELETE_ARRAY(m_CascadeProjectionShadowMap);
	CHECKED_DELETE_ARRAY(m_SplitDepthsTmp);
	CHECKED_DELETE_ARRAY(m_CascadeShadowPositions);
	CHECKED_DELETE_ARRAY(m_CascadeViewProjShadowMap);
}

void CDirectionalLight::Render(CRenderManager *RM)
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
	RM->DrawTower(0.4f, colYELLOW);

	mat.SetIdentity();
	RM->SetTransform(mat);
	Vect3f l_Dir = l_Pos + (m_Direction);
	RM->DrawLine( l_Pos , l_Dir );

	RM->SetTransform(m44fIDENTITY);

	if(m_LightFrustum != NULL)
	{
		//m_LightFrustum->DrawFrustum();

		for(uint32 i = 0; i < m_NumCascades; ++i)
		{
			m_ShadowFrustums[2]->DrawFrustum();
		}
	}
}

void CDirectionalLight::CreateShadowTextures(bool staticMap, CTexture::TFormatType formatStatic, uint32 staticWidth, uint32 staticHeight, bool dynamicMap, CTexture::TFormatType formatDynamic, uint32 dynamicWidth, uint32 dynamicHeight)
{
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	if(dynamicMap)
	{
		std::string l_TextureName = m_Name + "_dynamic";
		l_pTextureManager->RemoveResource( l_TextureName );

		m_pDynamicShadowMap = new CTexture();

		m_pDynamicShadowMap->Create(l_TextureName, dynamicWidth * m_NumCascades, dynamicHeight, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, formatDynamic);

		l_pTextureManager->AddResource(m_pDynamicShadowMap->GetName(), m_pDynamicShadowMap);

		m_DynamicShadowMapSize = Vect2i(dynamicWidth, dynamicHeight);

		m_ShadowMapPixelSize = Vect2f(0.5f / dynamicWidth * m_NumCascades, 0.5f / dynamicHeight);

		//////Depth Stencil

		//Elimina la textura si ya existía
		std::string l_TextureNameDS = m_Name + "_dynamic_ds";
		l_pTextureManager->RemoveResource( l_TextureNameDS );

		m_DynamicDepthStencil = new CTexture();
		m_DynamicDepthStencil->CreateDepthStencil(dynamicWidth * m_NumCascades, dynamicHeight, CTexture::D24S8, static_cast<D3DMULTISAMPLE_TYPE>(m_MultiSamples));
		l_pTextureManager->AddResource(l_TextureNameDS, m_DynamicDepthStencil);
	}

	if(staticMap)
	{
		std::string l_TextureName = m_Name + "_static";
		l_pTextureManager->RemoveResource( l_TextureName );

		m_pStaticShadowMap = new CTexture();
		m_pStaticShadowMap->Create(l_TextureName, staticWidth * m_NumCascades, staticHeight, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, formatStatic);

		m_StaticShadowMapSize = Vect2i(staticWidth, staticHeight);

		l_pTextureManager->AddResource(m_pStaticShadowMap->GetName(), m_pStaticShadowMap);

		m_MustUpdateStaticShadowMap = true;

		//////Depth Stencil

		//Elimina la textura si ya existía
		std::string l_TextureNameDS = m_Name + "_static_ds";
		l_pTextureManager->RemoveResource( l_TextureNameDS );

		m_StaticDepthStencil = new CTexture();
		m_StaticDepthStencil->CreateDepthStencil(staticWidth * m_NumCascades, staticHeight, CTexture::D24S8, static_cast<D3DMULTISAMPLE_TYPE>(m_MultiSamples));
		l_pTextureManager->AddResource(l_TextureNameDS, m_StaticDepthStencil);
	}
}

void CDirectionalLight::SetShadowMap()
{
	D3DXMATRIX l_View;
	D3DXMATRIX l_Projection;

	CEffectManager* l_EffectManager = CORE->GetEffectManager();

	D3DXVECTOR3 l_Eye = D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z);

	D3DXVECTOR3 l_LookAt(m_Direction.x, m_Direction.y, m_Direction.z);

	Vect3f vup(0, 1, 0);
	D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);

	//Setup Matrix view
	D3DXMatrixLookAtLH( &l_View, &l_Eye, &l_LookAt, &l_VUP);

	//Setup Matrix projection
	D3DXMatrixOrthoLH( &l_Projection, static_cast<float>(m_OrthoShadowMapSize.x), static_cast<float>(m_OrthoShadowMapSize.y), 1.0f, m_EndRangeAttenuation);
	m_ViewShadowMap = Mat44f(l_View);
	m_ProjectionShadowMap= Mat44f(l_Projection);

	m_LightFrustum->Update((m_ProjectionShadowMap * m_ViewShadowMap).GetD3DXMatrix());

	l_EffectManager->SetShadowCameraEye(m_Position);

	CalculateCascadeFrustums();
}

void CDirectionalLight::CalculateCascadeFrustums()
{	
	//Step 1: Get Frustum Corners & transform them to Camera View Space
	Vect3f* frustumCornersWS = CORE->GetRenderManager()->GetFrustum()->GetCorners();
	D3DXMATRIX cameraView = CORE->GetCamera()->GetViewMatrixDX();

	for(uint32 i = 0; i < 8; ++i)
	{
		D3DXVECTOR4 transVec;
		D3DXVECTOR3 vec(frustumCornersWS[i].x, frustumCornersWS[i].y, frustumCornersWS[i].z);

		D3DXVec3Transform(&transVec, &vec, &cameraView);

		m_FrustumCornersVS[i] = Vect3f(transVec.x, transVec.y, transVec.z);
	}

	//Step 2: Get Near Clip & Minimum Far Clip
	float nearClip = CORE->GetCamera()->GetZn();
	float farClip = mathUtils::Min(CORE->GetCamera()->GetZf(), m_EndRangeAttenuation);

	//Step 3: Compute Far Distance for each Shadow Frustum
	m_SplitDepthsTmp[0] = nearClip;
	m_SplitDepthsTmp[m_NumCascades] = farClip;

	/*for (uint32 i = 1; i < m_NumCascades; ++i)
	{
		m_SplitDepthsTmp[i] = nearClip + ( ( farClip - nearClip ) * mathUtils::PowN( ( (float)i / (float)m_NumCascades) , 3 ) );
	}*/

 	m_SplitDepthsTmp[1] = nearClip + (farClip - nearClip) * 0.07f;
	m_SplitDepthsTmp[2] = nearClip + (farClip - nearClip) * 0.40f;

	m_CascadeDistances.x = m_SplitDepthsTmp[0];
	m_CascadeDistances.y = m_SplitDepthsTmp[1] * 0.5f;
	m_CascadeDistances.z = m_SplitDepthsTmp[2] * 0.75f;

	for (uint32 i = 0; i < m_NumCascades; ++i)
	{
		D3DXMATRIX view;
		D3DXMATRIX proj;
		D3DXMATRIX viewProj;
		Vect3f shPos = v3fZERO;

		CalculateCascadeFrustumsVP(farClip, m_SplitDepthsTmp[i], m_SplitDepthsTmp[i + 1], view, proj, shPos);
		
		D3DXMatrixMultiply(&viewProj, &view, &proj);

		m_ShadowFrustums[i]->Update(viewProj);
		m_CascadeViewProjShadowMap[i] = Mat44f(viewProj);
		m_CascadeViewShadowMap[i] = Mat44f(view);
		m_CascadeProjectionShadowMap[i] = Mat44f(proj);
		m_CascadeShadowPositions[i] = shPos;
	}
}

void CDirectionalLight::CalculateCascadeFrustumsVP(float farClip, float frustumNearClip, float frustumFarClip, D3DXMATRIX& view, D3DXMATRIX& proj, Vect3f& shPos)
{
	CCamera* camera = CORE->GetCamera();

	for(uint32 i = 0; i < 4; ++i)
	{
		m_SplitFrustumCornersVS[i] = m_FrustumCornersVS[i + 4] * (frustumNearClip / camera->GetZf());
	}

	for (uint32 i = 4; i < 8; ++i)
	{
		m_SplitFrustumCornersVS[i] = m_FrustumCornersVS[i] * (frustumFarClip / camera->GetZf());
	}

	Vect3f frustumCornersCameraPos[8];
	ZeroMemory(frustumCornersCameraPos, sizeof(Vect3f) * 8);

	D3DXMATRIX cameraTranslation = camera->GetObject3D()->GetTransform().GetD3DXMatrix();
	//Vect3f cameraPos = camera->GetPosition();
	//D3DXMatrixTranslation(&cameraTranslation, cameraPos.x, cameraPos.y, cameraPos.z);

	for(uint32 i = 0; i < 8; ++i)
	{
		D3DXVECTOR4 transPos;
		D3DXVECTOR3 vectPos(m_SplitFrustumCornersVS[i].x, m_SplitFrustumCornersVS[i].y, m_SplitFrustumCornersVS[i].z);

		D3DXVec3Transform(&transPos, &vectPos, &cameraTranslation);

		frustumCornersCameraPos[i] = Vect3f(transPos.x, transPos.y, transPos.z);
	}

	Vect3f cameraUpVector = Vect3f(0.0f, 1.0f, 0.0f);

	if(mathUtils::Abs(cameraUpVector.Dot(m_DirectionNormalize)) > 0.9f)
	{
		cameraUpVector = Vect3f(0.0f, 0.0f, 1.0f);
	}

	D3DXMATRIX lightRotation;
	D3DXVECTOR3 eye(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 target(m_DirectionNormalize.x, m_DirectionNormalize.y, m_DirectionNormalize.z);
	D3DXVECTOR3 upVect(cameraUpVector.x, cameraUpVector.y, cameraUpVector.z);

	D3DXMatrixLookAtLH(&lightRotation, &eye, &target, &upVect);
	
	for (uint32 i = 0; i < 8; ++i)
	{
		D3DXVECTOR4 trans;
		D3DXVECTOR3 vect(frustumCornersCameraPos[i].x, frustumCornersCameraPos[i].y, frustumCornersCameraPos[i].z);

		D3DXVec3Transform(&trans, &vect, &lightRotation);

		frustumCornersCameraPos[i] = Vect3f(trans.x, trans.y, trans.z);
	}

	TBoundingBox lightBox(frustumCornersCameraPos);

	bool fixShadowJittering = true;
	if (fixShadowJittering)
	{
		float diagonalLength = (frustumCornersCameraPos[0] - frustumCornersCameraPos[6]).Length();
		diagonalLength += 2; //Without this, the shadow map isn't big enough in the world.
		float worldsUnitsPerTexel = diagonalLength/(float) m_DynamicShadowMapSize.x;

		Vect3f vBorderOffset = ( Vect3f(diagonalLength, diagonalLength, diagonalLength) - (lightBox.m_MaxPos - lightBox.m_MinPos)) * 0.5f;
		lightBox.m_MaxPos += vBorderOffset;
		lightBox.m_MinPos -= vBorderOffset;

		lightBox.m_MinPos /= worldsUnitsPerTexel;
		lightBox.m_MinPos.z = (float) mathUtils::Floor(lightBox.m_MinPos.z);
		lightBox.m_MinPos.y = (float) mathUtils::Floor(lightBox.m_MinPos.y);
		lightBox.m_MinPos.z = (float) mathUtils::Floor(lightBox.m_MinPos.z);
		lightBox.m_MinPos *= worldsUnitsPerTexel;

		lightBox.m_MaxPos /= worldsUnitsPerTexel;
		lightBox.m_MaxPos.x = (float) mathUtils::Floor(lightBox.m_MaxPos.x);
		lightBox.m_MaxPos.y = (float) mathUtils::Floor(lightBox.m_MaxPos.y);
		lightBox.m_MaxPos.z = (float) mathUtils::Floor(lightBox.m_MaxPos.z);
		lightBox.m_MaxPos *= worldsUnitsPerTexel;
	}


	Vect3f boxSize = lightBox.m_MaxPos - lightBox.m_MinPos;

	if (boxSize.x == 0 || boxSize.y == 0 || boxSize.z == 0)
	{
		boxSize = v3fONE;
	}

	Vect3f halfBoxSize = boxSize * 0.5f;
 
	Vect3f lightPosition = lightBox.m_MinPos + halfBoxSize;
	lightPosition.z = lightBox.m_MinPos.z;

	D3DXMatrixInverse(&lightRotation, NULL, &lightRotation);

	D3DXVECTOR4 transLight;
	D3DXVECTOR3 vectLight(lightPosition.x, lightPosition.y, lightPosition.z);

	D3DXVec3Transform(&transLight, &vectLight, &lightRotation);

	vectLight = D3DXVECTOR3(transLight.x, transLight.y - 20, transLight.z - 25);
	shPos.x = vectLight.x;
	shPos.y = vectLight.y;
	shPos.z = vectLight.z;

	target = vectLight + target;

	// Create the view matrix for the light
	D3DXMatrixLookAtLH(&view, &vectLight, &target, &upVect);

	D3DXMatrixOrthoLH(&proj, boxSize.x, boxSize.y, -boxSize.z, boxSize.z);
	//D3DXMatrixOrthoLH(&proj, boxSize.x, boxSize.y, 0, boxSize.z);
}

void CDirectionalLight::RenderShadows( CRenderManager *RM )
{
	CEffectManager* l_EffectManager = CORE->GetEffectManager();

	if( m_GenerateStaticShadowMap && m_MustUpdateStaticShadowMap )
	{
		//m_pStaticShadowMap->SetAsRenderTarget(0);
		//m_StaticDepthStencil->SetAsDepthStencil();

		//RM->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0 );

		//CORE->GetEffectManager()->SetLightShadowLinNearFar(m_LightLinNearFar);

		//for(size_t i=0; i<m_StaticShadowMapRenderableObjectsManagers.size(); ++i)
		//{
		//	m_StaticShadowMapRenderableObjectsManagers[i]->Render(RM);
		//}

		//m_MustUpdateStaticShadowMap = false;

		//m_pStaticShadowMap->UnsetAsRenderTarget(0);
		//m_StaticDepthStencil->UnsetAsDepthStencil();
	}

	if( m_DynamicShadowMapRenderableObjectsManagers.size() > 0)
	{
		D3DVIEWPORT9 defaultVP;
		RM->GetDevice()->GetViewport(&defaultVP);

		D3DVIEWPORT9 shadowVP;
		shadowVP.MinZ = 0.0f;
		shadowVP.MaxZ = 1.0f;
		shadowVP.X = 0;
		shadowVP.Y = 0;
		shadowVP.Height = m_DynamicShadowMapSize.y;
		shadowVP.Width = m_DynamicShadowMapSize.x;

		m_pDynamicShadowMap->SetAsRenderTarget(0);
		m_DynamicDepthStencil->SetAsDepthStencil();

		RM->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0 );

		for(uint32 i =0; i < m_NumCascades; ++i)
		{
			shadowVP.X = i * m_DynamicShadowMapSize.x;

			RM->GetDevice()->SetViewport(&shadowVP);

			l_EffectManager->SetShadowProjectionMatrix(m_CascadeProjectionShadowMap[i]);
			l_EffectManager->SetShadowViewMatrix(m_CascadeViewShadowMap[i]);
			l_EffectManager->SetShadowCameraEye(m_CascadeShadowPositions[i]);
			
			CORE->GetLightManager()->SetCurrentFrustum(m_ShadowFrustums[i]);

			CORE->GetEffectManager()->SetLightShadowLinNearFar(m_LightLinNearFar);

			for(size_t i=0; i<m_DynamicShadowMapRenderableObjectsManagers.size(); ++i)
			{
				m_DynamicShadowMapRenderableObjectsManagers[i]->Render(RM);
			}
		}

		m_pDynamicShadowMap->UnsetAsRenderTarget(0);
		m_DynamicDepthStencil->UnsetAsDepthStencil();

		RM->GetDevice()->SetViewport(&defaultVP);
	}

	CORE->GetLightManager()->SetCurrentFrustum(NULL);
}

void CDirectionalLight::CalculateOrientationDebugRender()
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