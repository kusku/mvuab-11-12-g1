#include "Billboard.h"
#include "Cameras\Camera.h" 
#include "RenderManager.h"
#include <d3d9.h>
#include <string>

#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Vertexs\VertexType.h"

#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CBillboard::CBillboard(void)
	: m_vPosition	( Vect3f(0.f,0.f,0.f) )
	, m_bIsVisible	( false )
	, m_fWidth		( 1.f )
	, m_fHeight		( 1.f )
	, m_PointA		( Vect3f(0.f,0.f,0.f) )
	, m_PointB		( Vect3f(0.f,0.f,0.f) )
	, m_PointC		( Vect3f(0.f,0.f,0.f) )
	, m_PointD		( Vect3f(0.f,0.f,0.f) )
	, m_Texture		( NULL )
{
	m_Texture = CORE->GetTextureManager()->GetNoTexture();
}


CBillboard::~CBillboard(void)
{
	Destroy();
}


// -----------------------------------------
//			   MÈTODES PRINCIPALS
// -----------------------------------------
void CBillboard::Destroy ( void )
{
	m_Texture = NULL;
}

void CBillboard::Render ( CRenderManager &_RM, const CColor &_Color )
{
	//RenderByHardware ( _RM, _Color );
	RenderBySoftware ( _RM, _Color );
}

void CBillboard::RenderBySoftware ( CRenderManager &_RM, const CColor &_Color )
{
	TCOLOREDTEXTURE1_VERTEX l_Points[4];
    unsigned short l_Indexes[6]={0,2,3,1};
	
	unsigned long color_aux = _Color.GetUint32Argb();

    l_Points[0].x		= m_PointA.x;
    l_Points[0].y		= m_PointA.y;
    l_Points[0].z		= m_PointA.z;
    l_Points[0].tu		= 0.0f;
    l_Points[0].tv		= 0.0f;
    l_Points[0].color	= color_aux;
        
    l_Points[1].x		= m_PointB.x;
    l_Points[1].y		= m_PointB.y;
    l_Points[1].z		= m_PointB.z;
    l_Points[1].tu		= 1.0f;
    l_Points[1].tv		= 0.0f;
    l_Points[1].color	= color_aux;

    l_Points[2].x		= m_PointC.x;
    l_Points[2].y		= m_PointC.y;
    l_Points[2].z		= m_PointC.z;
    l_Points[2].tu		= 0.0f;
    l_Points[2].tv		= 1.0f;
    l_Points[2].color	= color_aux;

    l_Points[3].x		= m_PointD.x;
    l_Points[3].y		= m_PointD.y;
    l_Points[3].z		= m_PointD.z;
    l_Points[3].tu		= 1.0f;
    l_Points[3].tv		= 1.0f;        
    l_Points[3].color	= color_aux;
	
	LPDIRECT3DDEVICE9 Device = _RM.GetDevice();
	Device->SetTexture( 0, m_Texture->GetDXTexture() );
    Device->SetFVF((DWORD ) TCOLOREDTEXTURE1_VERTEX::GetFVF() );
    Device->DrawIndexedPrimitiveUP( D3DPT_TRIANGLEFAN, 0, 4, 2, l_Indexes ,D3DFMT_INDEX16, l_Points, sizeof(TCOLOREDTEXTURE1_VERTEX) );
}

void CBillboard::RenderByHardware ( CRenderManager &_RM, const CColor &_Color )
{
	//CORE->GetRenderableObjectsTechniqueManager()->GetPoolRenderableObjectTechniques().GetResource(
	std::string l_TechniqueName = "DrawSingleTextureTechnique"; // CORE->GetEffectsManager()->GetTechniqueEffectNameByVertexDefault( ( int ) TGEOMETRYCOLORTEXTURA1VERTEX::GetVertexType() );
	CEffectTechnique* l_pTechnique = CORE->GetEffectManager()->GetEffectTechnique ( l_TechniqueName );

	CEffect* l_pEffect = l_pTechnique->GetEffect();
	l_pTechnique->BeginRender();
	LPD3DXEFFECT l_Effect = l_pTechnique->GetEffect()->GetD3DEffect();

	if ( l_Effect != NULL )
	{
		l_Effect->SetTechnique ( l_pTechnique->GetD3DTechnique() );
		UINT l_NumPasses;
		if ( SUCCEEDED ( l_Effect->Begin ( &l_NumPasses, 0 ) ) )
		{
			
			for ( UINT iPass = 0; iPass < l_NumPasses; iPass++ )
			{
				l_Effect->BeginPass(iPass);
	
				//_RM->DrawColoredQuad2DTexturedInPixels ( _Color, _U0, _V0, _U1, _V1 );

				TCOLOREDTEXTURE1_VERTEX l_Points[4];
				unsigned short l_Indexes[6]={0, 2, 3, 1};
	
				unsigned long color_aux = _Color.GetUint32Argb();

				l_Points[0].x		= m_PointA.x;
				l_Points[0].y		= m_PointA.y;
				l_Points[0].z		= m_PointA.z;
				l_Points[0].tu		= 0.0f;
				l_Points[0].tv		= 0.0f;
				l_Points[0].color	= color_aux;
        
				l_Points[1].x		= m_PointB.x;
				l_Points[1].y		= m_PointB.y;
				l_Points[1].z		= m_PointB.z;
				l_Points[1].tu		= 1.0f;
				l_Points[1].tv		= 0.0f;
				l_Points[1].color	= color_aux;

				l_Points[2].x		= m_PointC.x;
				l_Points[2].y		= m_PointC.y;
				l_Points[2].z		= m_PointC.z;
				l_Points[2].tu		= 0.0f;
				l_Points[2].tv		= 1.0f;
				l_Points[2].color	= color_aux;

				l_Points[3].x		= m_PointD.x;
				l_Points[3].y		= m_PointD.y;
				l_Points[3].z		= m_PointD.z;
				l_Points[3].tu		= 1.0f;
				l_Points[3].tv		= 1.0f;        
				l_Points[3].color	= color_aux;

				Mat44f mat, rotYaw, rotPitch, rotRoll;          
	  
				//mat.SetIdentity ( );         
				//rotYaw.SetIdentity ( );         
				//rotPitch.SetIdentity ( );         
				//rotRoll.SetIdentity ( );          
				//mat.Translate( GetPosition ( ) );                          
				///*rotPitch.SetRotByAngleX( mathUtils::Deg2Rad<float> ( GetPitch()) );           
				//rotYaw.SetRotByAngleY ( mathUtils::Deg2Rad<float> ( GetYaw()) );         
				//rotRoll.SetRotByAngleZ ( mathUtils::Deg2Rad<float> ( GetRoll()) ); */                         
				//mat = mat * rotYaw * rotPitch * rotRoll;                          
	  
				//_RM->SetTransform ( mat );          


				LPDIRECT3DDEVICE9 Device = _RM.GetDevice();
				Device->SetTexture( 0, m_Texture->GetDXTexture() );
				if ( SUCCEEDED ( Device->SetVertexDeclaration( TCOLOREDTEXTURE1_VERTEX::GetVertexDeclaration() ) ) )
				{
					Device->DrawIndexedPrimitiveUP( D3DPT_TRIANGLEFAN, 0, 4 ,2 , l_Indexes, D3DFMT_INDEX16, l_Points, sizeof( TCOLOREDTEXTURE1_VERTEX ) );
				}

				l_Effect->EndPass();
			}
			l_Effect->End();
		}
	}
}

void CBillboard::Update ( CCamera * _pCamera, float _Angle )
{
	Mat33f l_Rotation;
	l_Rotation.FromAxisAngle ( _pCamera->GetDirection(), _Angle );
	Vect3f l_UpVector = l_Rotation * _pCamera->GetVecUp();

	Vect3f l_Right = l_UpVector^( - _pCamera->GetDirection() );
	l_Right.Normalize();
	m_PointA = m_vPosition + m_fHeight/2 * _pCamera->GetVecUp() - m_fWidth/2 * l_Right;
	m_PointB = m_vPosition + m_fHeight/2 * _pCamera->GetVecUp() + m_fWidth/2 * l_Right;
	m_PointC = m_vPosition - m_fHeight/2 * _pCamera->GetVecUp() - m_fWidth/2 * l_Right;
	m_PointD = m_vPosition - m_fHeight/2 * _pCamera->GetVecUp() + m_fWidth/2 * l_Right;
}
	
// -----------------------------------------
//				MÈTODES PRIVATS
// -----------------------------------------

// -----------------------------------------
//				MÈTODES PUBLICS
// -----------------------------------------

// -----------------------------------------
//				PROPIEDADES
// -----------------------------------------
