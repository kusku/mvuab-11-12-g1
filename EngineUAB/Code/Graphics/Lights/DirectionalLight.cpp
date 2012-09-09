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
#include "Utils\BoundingBox.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDirectionalLight::CDirectionalLight( CXMLTreeNode &XMLNode )
{
	m_Type = DIRECTIONAL;
	m_Direction = XMLNode.GetVect3fProperty("dir", Vect3f(0.0f, 0.0f, 0.0f));
	//m_Direction.Normalize();

	m_OrthoShadowMapSize.x = XMLNode.GetIntProperty("directional_light_camara_size_w", 256);
	m_OrthoShadowMapSize.y = XMLNode.GetIntProperty("directional_light_camara_size_h", 256);

	//Extract Common Info
	ExtractCommonLightInfo(XMLNode);
}

CDirectionalLight::CDirectionalLight()
	: m_Direction( Vect3f(0.0f, 0.0f, 0.0f) )
	, m_OrthoShadowMapSize(Vect2i(256, 256))
{
	m_Type = DIRECTIONAL;
}

CDirectionalLight::~CDirectionalLight()
{
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
	Vect3f l_Dir = l_Pos + m_Direction;
	RM->DrawLine( l_Pos , l_Dir );

}

void CDirectionalLight::SetShadowMap()
{
	D3DXMATRIX l_View;
	D3DXMATRIX l_Projection;

	CEffectManager* l_EffectManager = CORE->GetEffectManager();

	D3DXVECTOR3 l_Eye = D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z);

	//static Vect3f temp = (-m_Direction).GetNormalized();
	//m_Direction = -temp;

	Vect3f lookat = m_Direction.GetNormalized();
	//lookat = m_Direction;
	D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);

	Vect3f vup(0, 1, 0);
	D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);

	//Setup Matrix view
	D3DXMatrixLookAtLH( &l_View, &l_Eye, &l_LookAt, &l_VUP);

	//Setup Matrix projection
	D3DXMatrixOrthoLH( &l_Projection, static_cast<float>(m_OrthoShadowMapSize.x), static_cast<float>(m_OrthoShadowMapSize.y), 1.0f, 1000);
	m_ViewShadowMap = Mat44f(l_View);
	m_ProjectionShadowMap= Mat44f(l_Projection);

	m_LightFrustum->Update((m_ProjectionShadowMap * m_ViewShadowMap).GetD3DXMatrix());

	l_EffectManager->SetShadowCameraEye(m_Position);
	l_EffectManager->SetShadowProjectionMatrix(m_ProjectionShadowMap);
	l_EffectManager->SetShadowViewMatrix(m_ViewShadowMap);

	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

	//CEffectManager* l_EffectManager = CORE->GetEffectManager();
	//D3DXMATRIX view;
	//D3DXMATRIX projection;
	//D3DXVECTOR3 cameraEye(0, 0, 0);
	//D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	//Vect3f lightDirNor = -m_Direction;
	//lightDirNor.Normalize();
	//D3DXVECTOR3 lightDir(lightDirNor.x, lightDirNor.y, lightDirNor.z);
	////D3DXVECTOR3 lightDir(-0.3333333f, 0.6666667f, 0.6666667f);
	//D3DXVECTOR3 temp = -lightDir;

	//// Matrix with that will rotate in points the direction of the light
	//D3DXMATRIX lightRotation;
	//D3DXMatrixLookAtLH(&lightRotation, &cameraEye, &temp, &up);
	////D3DXMatrixLookAtRH(&lightRotation, &cameraEye, &temp, &up);
	//Mat44f lightRotationUAB(lightRotation);

	//////Get the corners of the frustum
	////D3DXMATRIX newVP;
	////newVP._11 = 1.44852793f;
	////newVP._12 = 0.0f;
	////newVP._13 = 0.0f;
	////newVP._14 = 0.0f;
	////newVP._21 = 0.0f;
	////newVP._22 = 2.159338f;
	////newVP._23 = -0.447661281f;
	////newVP._24 = -0.44721362f;
	////newVP._31 = 0.0f;
	////newVP._32 = -1.079669f;
	////newVP._33 = -0.895322561f;
	////newVP._34 = -0.89442724f;
	////newVP._41 = 0.0f;
	////newVP._42 = -43.1867638f;
	////newVP._43 = 119.867546f;
	////newVP._44 = 120.747681f;
	////Mat44f newUABVP(newVP);
	////CORE->GetRenderManager()->GetFrustum()->Update(newVP);

	//const Vect3f *frustumCornersConst = CORE->GetRenderManager()->GetFrustum()->GetCorners();
	//Vect3f frustumCorners[8];

	//// Transform the positions of the corners into the direction of the light
	//for (int i = 0; i < 8; ++i)
	//{
	//	frustumCorners[i] = frustumCornersConst[i];
	//	lightRotationUAB.TransformVector(frustumCorners[i]);
	//}

	//// Find the smallest box around the points
	//CBoundingBox lightBox;
	//lightBox.InitFromPoints(frustumCorners);

	//Vect3f boxSize = lightBox.GetMax() - lightBox.GetMin();
	//Vect3f halfBoxSize = boxSize * 0.5f;

	//Vect3f lightPosition = lightBox.GetMin() + halfBoxSize;
	//lightPosition.z = lightBox.GetMin().z;

	//lightRotationUAB.Invert();
	//lightRotationUAB.TransformVector(lightPosition);

	//cameraEye = D3DXVECTOR3(lightPosition.x, lightPosition.y, lightPosition.z);
	//temp = (cameraEye + lightDir);
	//
	//D3DXMatrixLookAtLH(&view, &cameraEye, &temp, &up);
	////D3DXMatrixLookAtRH(&view, &cameraEye, &temp, &up);

	//D3DXMatrixOrthoLH(&projection, boxSize.x, boxSize.y, -boxSize.z, boxSize.z);
	////D3DXMatrixOrthoRH(&projection, boxSize.x, boxSize.y, -boxSize.z, boxSize.z);

	//m_ProjectionShadowMap = Mat44f(projection);
	//m_ViewShadowMap = Mat44f(view);

	//// Compute linear (distance to light) near and min/max for this perspective matrix
	//m_LightLinNearFar.x = CORE->GetCamera()->GetZn();
	//m_LightLinNearFar.y = m_EndRangeAttenuation;

	//m_Position = lightPosition;
	//m_LightFrustum->Update((m_ProjectionShadowMap * m_ViewShadowMap).GetD3DXMatrix());
	//l_EffectManager->SetShadowCameraEye(m_Position);
	//l_EffectManager->SetShadowProjectionMatrix(m_ProjectionShadowMap);
	//l_EffectManager->SetShadowViewMatrix(m_ViewShadowMap);
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