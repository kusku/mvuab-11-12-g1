#include "SkySphere.h"
#include "Math\Color.h"
#include "Effects\EffectTechnique.h"
#include "XML\XMLTreeNode.h"
#include "Textures\TextureCube.h"
#include "Core.h"
#include "Effects\EffectManager.h"
#include "Cameras\Camera.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSkySphere::CSkySphere()
	: m_SphereMesh(0, colRED)
	, m_SkySphereTechnique(NULL)
	, m_SkyTexture(NULL)
	, m_SkyTextureParameter(NULL)
	, m_World(m44fIDENTITY)
{
	m_World.Scale(1000, 900, 1000);
}

CSkySphere::~CSkySphere()
{
	CHECKED_DELETE(m_SkyTexture);
}

bool CSkySphere::Load(CXMLTreeNode &XMLNode)
{
	//Clean up before loading
	CHECKED_DELETE(m_SkyTexture);
	m_SkySphereTechnique = NULL;
	m_SkyTextureParameter = NULL;

	std::string skyTextureFile = XMLNode.GetPszProperty("SkyTexture", "");
	std::string skyEffectTechnique = XMLNode.GetPszProperty("SkyEffectTechnique", "");

	m_SkyTexture = new CTextureCube();
	
	if(!m_SkyTexture->Load(skyTextureFile))
	{
		CHECKED_DELETE(m_SkyTexture);
		return false;
	}

	m_SkySphereTechnique = CORE->GetEffectManager()->GetEffectTechnique(skyEffectTechnique);

	if(m_SkySphereTechnique == NULL)
	{
		CHECKED_DELETE(m_SkyTexture);
		return false;
	}

	m_SkySphereTechnique->GetEffect()->GetParameterBySemantic("SKY_TEXTURECUBE", m_SkyTextureParameter);

	if(m_SkyTextureParameter == NULL)
	{
		CHECKED_DELETE(m_SkyTexture);
		return false;
	}

	return true;
}

bool CSkySphere::Render()
{
	if(m_SkyTexture == NULL)
	{
		return false;
	}

	//Set Sky Texture
	m_SkySphereTechnique->GetEffect()->GetD3DEffect()->SetTexture(m_SkyTextureParameter, m_SkyTexture->GetDXCubeTexture());

	//Set World Matrix
	Mat44f pos(m44fIDENTITY);
	pos.Translate(CORE->GetCamera()->GetPosition());
	CORE->GetEffectManager()->SetWorldMatrix(pos * m_World);

	m_SphereMesh.Render(CORE->GetRenderManager(), m_SkySphereTechnique);

	return true;
}