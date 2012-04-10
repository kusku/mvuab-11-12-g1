#include "Billboard.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Vertexs\VertexType.h"
#include "Utils\Types.h"
#include "Textures\TextureManager.h"
#include "Textures\Texture.h"
#include "Math\Matrix33.h"
#include "Base.h"
#include "Core.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CBillboard::CBillboard()
	: m_vPosition( Vect3f(0.0f, 0.0f, 0.0f) )
	, m_fHeight(0.f)
	, m_fWidth(0.f)
	, m_fRotation(0.f)
	, m_Color(colWHITE)
	, m_pTexture(NULL)
	, m_APoint( Vect3f(0.f, 0.f, 0.f) )
	, m_BPoint( Vect3f(0.f, 1.f, 0.f) )
	, m_CPoint( Vect3f(1.f, 0.f, 0.f) )
	, m_DPoint( Vect3f(1.f, 1.f, 0.f) )
{
}

CBillboard::CBillboard(float height, float width, const Vect3f &position, float rotation, const CColor& color, const std::string &textureName)
	: m_vPosition(position)
	, m_fHeight(height)
	, m_fWidth(width)
	, m_fRotation(rotation)
	, m_Color(color)
	, m_APoint( Vect3f(0.f, 0.f, 0.f) )
	, m_BPoint( Vect3f(0.f, 1.f, 0.f) )
	, m_CPoint( Vect3f(1.f, 0.f, 0.f) )
	, m_DPoint( Vect3f(1.f, 1.f, 0.f) )
{
	m_pTexture = CORE->GetTextureManager()->GetTexture(textureName);
	assert(m_pTexture);
}

CBillboard::~CBillboard()
{
}

void CBillboard::Update(CCamera &Camera)
{
	Vect3f l_vUp = Camera.GetVecUp();
	Vect3f l_vDirection = -Camera.GetDirection();
	Vect3f l_vRight = l_vUp ^ l_vDirection;
	l_vRight.Normalize(1.f);
	
	//Crea una matriz de rotación
	Mat33f l_Rotation;
	l_Rotation.SetIdentity();
	l_Rotation.FromAxisAngle(l_vDirection, m_fRotation);
	l_vUp = l_Rotation * l_vUp;

	m_APoint = m_vPosition + (m_fHeight / 2 * l_vUp) - (m_fWidth / 2 * l_vRight);
	m_BPoint = m_vPosition + (m_fHeight / 2 * l_vUp) + (m_fWidth / 2 * l_vRight);
	m_CPoint = m_vPosition - (m_fHeight / 2 * l_vUp) - (m_fWidth / 2 * l_vRight);
	m_DPoint = m_vPosition - (m_fHeight / 2 * l_vUp) + (m_fWidth / 2 * l_vRight);
}

void CBillboard::Render(CRenderManager &RM)
{
	TCOLOREDTEXTURE1_VERTEX l_Points[4];
	uint16 l_Indexes[6] = { 0, 2, 3, 1 };
	D3DCOLOR l_Color = D3DCOLOR_COLORVALUE( m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha() );

	//Rellena el vector con la información del billboard
	l_Points[0].x = m_APoint.x;
	l_Points[0].y = m_APoint.y;
	l_Points[0].z = m_APoint.z;
	l_Points[0].tu = 1.f;
	l_Points[0].tv = 0.f;
	l_Points[0].color = l_Color;

	l_Points[1].x = m_BPoint.x;
	l_Points[1].y = m_BPoint.y;
	l_Points[1].z = m_BPoint.z;
	l_Points[1].tu = 0.f;
	l_Points[1].tv = 0.f;
	l_Points[1].color = l_Color;

	l_Points[2].x = m_CPoint.x;
	l_Points[2].y = m_CPoint.y;
	l_Points[2].z = m_CPoint.z;
	l_Points[2].tu = 1.f;
	l_Points[2].tv = 1.f;
	l_Points[2].color = l_Color;

	l_Points[3].x = m_DPoint.x;
	l_Points[3].y = m_DPoint.y;
	l_Points[3].z = m_DPoint.z;
	l_Points[3].tu = 0.f;
	l_Points[3].tv = 1.f;
	l_Points[3].color = l_Color;

	Mat44f mat;
	mat.SetIdentity();
	RM.SetTransform(mat);

	//Renderiza el billboard
	assert(m_pTexture);
	LPDIRECT3DDEVICE9 l_Device = RM.GetDevice();
	l_Device->SetTexture(0, m_pTexture->GetDXTexture());
	l_Device->SetFVF(TCOLOREDTEXTURE1_VERTEX::GetFVF());
	l_Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLEFAN, 0, 4, 2, l_Indexes, D3DFMT_INDEX16, l_Points, sizeof(TCOLOREDTEXTURE1_VERTEX));
}

void CBillboard::SetTexture(const std::string &name)
{
	m_pTexture = CORE->GetTextureManager()->GetTexture(name);
	assert(m_pTexture);
}